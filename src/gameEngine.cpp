#include <cmath>
#include <stdlib.h>
#include "gameEngine.hpp"
#include "controls.hpp"
#include "colors.hpp"
#include "gameBase.hpp"
#include "menu.hpp"
#include "infoPanel.hpp"

CGameEngine::CGameEngine(WINDOW * w) :
CInterfaceElement(w), showTutorial(false), m_MapHeight(m_Height - 8), m_MenuHeight(6), g_RadioTransmitted(false) {
    m_MapWin = newwin(m_MapHeight, m_Width, 0, 0);
    m_BottomWin = newwin(m_MenuHeight, m_Width, m_MapHeight + 3, 0);
    m_WorldMap = new CWorldMap(m_MapWin, this);

}

CGameEngine::~CGameEngine() {
    delwin(m_MapWin);
    delwin(m_BottomWin);
    delete m_WorldMap;
}

void CGameEngine::Show() {
    // Render map
    m_WorldMap->Show();

    if (showTutorial) {
        ShowTutorial();
        showTutorial = false;
    }

    // Horizontal line above resources
    wmove(m_Window, m_MapHeight, 0);
    for (size_t i = 0; i < m_Width; i++) {
        waddch(m_Window, ACS_HLINE);
    }

    wmove(m_Window, m_MapHeight + 1, 0);
    wclrtoeol(m_Window);
    ShowResourcesBar();

    // Horizontal line under resources
    wmove(m_Window, m_MapHeight + 2, 0);
    for (size_t i = 0; i < m_Width; i++) {
        waddch(m_Window, ACS_HLINE);
    }
    wrefresh(m_Window);
}

void CGameEngine::RegisterKeys() {
    // Register keys for map
    m_WorldMap->RegisterKeys();

    // Key for menu
    CControls & c = CControls::getInstance();
    c.AssignKey(KEY_F(9), [this]() {
        CGameBase::get().AddInterfaceElement(new CMenuPause(m_Window));
    });
    c.AssignKey('b', [this]() {
        CGameBase::get().AddInterfaceElement(new CMenuBuild(m_BottomWin, this));
    });
    c.AssignKey(' ', [this]() {
        NextDay();
    });
    c.AssignKey('+', [this]() {
        if (selectedBuilding && !selectedBuilding.get()->habitable) {
            SetEmployees(selectedBuilding, selectedBuilding.get()->workers + 1);
        }
    });
    c.AssignKey('-', [this]() {
        if (selectedBuilding && !selectedBuilding.get()->habitable && selectedBuilding.get()->workers > 0) {
            SetEmployees(selectedBuilding, selectedBuilding.get()->workers - 1);
        }
    });
}

void CGameEngine::NextDay() {
    g_Day++;

    // Check if rescue ship arrived
    if (g_IsRadioTowerBuilt && g_RadioTransmitted && --g_DaysToRescue == 0) {
        FinishGame(true);
        return;
    }

    if (g_Day == 50) {
        FinishGame(false);
        return;
    }

    unsigned int generatedPower = 0;
    unsigned int deaths = 0;
    std::shared_ptr<CWorldAction> action = CGameBase::get().DailyAction[g_Day];
    generatedPower = GetPowerEarnings() * (1 - action.get()->powerDrop / 100.0);

    // Kill people without habitation
    if (g_People > g_HabitationCapacity) {
        deaths += g_People - g_HabitationCapacity;
        RemovePeople(g_People - g_HabitationCapacity);
    }

    // Kill people in case there's not enough power
    for (std::shared_ptr<CBuilding> building : g_Habitation) {
        unsigned int consumption = building.get()->GetPowerConsumption();
        if (consumption <= generatedPower) {
            generatedPower -= consumption;
        } else {
            unsigned int victims = std::ceil(building.get()->workers * (1 - generatedPower / (1.0 * consumption)));
            RemovePeople(victims);
            deaths += victims;
            generatedPower = 0;
        }
    }

    // If someone survived, get metal and concrete income and continue the game
    if (g_People > 0) {
        std::vector<std::string> daily = DoDailyAction(action);
        if (deaths == 1) {
            daily.push_back(std::to_string(deaths) + CGameBase::get().TextStory["PEOPLE_DIED_SING"]);
        } else if (deaths > 1) {
            daily.push_back(std::to_string(deaths) + CGameBase::get().TextStory["PEOPLE_DIED_PLUR"]);
        }

        GetExpectedResourceIncome();
        g_Concrete += g_ConcreteIncome;
        g_Metal += g_MetalIncome;
        GetExpectedResourceIncome(false);

        m_WorldMap->RemoveShipTiles(2);

        Show();
        CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_BottomWin, daily, false));
        std::string story = CGameBase::get().TextStory["DAY_" + std::to_string(g_Day)];
        if (story.size() > 0) {
            CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_BottomWin, story, false));
        }
    }
}

void CGameEngine::GetExpectedResourceIncome(bool includeDailyAction) {
    unsigned int generatedPower;
    g_NetPowerEarnings = GetPowerEarnings();
    generatedPower = g_NetPowerEarnings;

    if (includeDailyAction) {
        generatedPower *= (1 - CGameBase::get().DailyAction[g_Day].get()->powerDrop / 100.0);
    }

    // Get power consumption for all buildings
    for (std::shared_ptr<CBuilding> building : g_Habitation) {
        unsigned int consumption = building.get()->GetPowerConsumption();
        if (consumption <= generatedPower) {
            generatedPower -= consumption;
        } else {
            generatedPower = 0;
        }
        g_NetPowerEarnings -= consumption;
    }

    ComputeConcreteMetalIncome(generatedPower, g_NetPowerEarnings);
    g_Power = generatedPower;
}

bool CGameEngine::Deserialize(std::vector<std::string> src) {
    // Load buildings
    std::vector<std::string> buildingData = CGameBase::get().Load(CFileIO::dataFolder + "building");
    if (buildingData.size() == 0) {
        // Failed to load, handled by CGameBase
        return false;
    }

    // Create templates from loaded data
    for (unsigned int i = 0; i < buildingData.size(); i += 13) {
        try {
            char symbol = buildingData[i + 2][0];
            short int width = std::stoi(buildingData[i + 3], NULL, 10);
            short int height = std::stoi(buildingData[i + 4], NULL, 10);
            short int capacity = std::stoi(buildingData[i + 5], NULL, 10);
            short int powerCons = std::stoi(buildingData[i + 6], NULL, 10);
            short int powerInc = std::stoi(buildingData[i + 7], NULL, 10);
            short int concreteInc = std::stoi(buildingData[i + 8], NULL, 10);
            short int metalInc = std::stoi(buildingData[i + 9], NULL, 10);
            short int priceConc = std::stoi(buildingData[i + 10], NULL, 10);
            short int priceMet = std::stoi(buildingData[i + 11], NULL, 10);
            bool habitable = std::stoi(buildingData[i + 12], NULL, 10) == 1;
            g_BuildingTemplates.push_back(std::make_shared<CBuilding>(buildingData[i], buildingData[i + 1], width, height, capacity, powerCons, powerInc, concreteInc, metalInc, priceConc, priceMet, symbol, habitable));
        } catch (...) {
            return false;
        }
    }

    // Load resources
    if (src.size() == 0) {
        m_WorldMap->Deserialize(src);
        g_Day = 1;
        g_HabitationCapacity = 0;
        g_People = g_Unemployed = 4;
        g_NetPowerEarnings = 0;
        g_IsRadioTowerBuilt = false;
        g_DaysToRescue = 7;

        // Starter buildings
        std::shared_ptr<CBuilding> building = std::make_shared<CBuilding>(*(g_BuildingTemplates[3]));
        m_WorldMap->Build(building, 30, 6);

        building = std::make_shared<CBuilding>(*(g_BuildingTemplates[3]));
        m_WorldMap->Build(building, 27, 8);

        building = std::make_shared<CBuilding>(*(g_BuildingTemplates[4]));
        m_WorldMap->Build(building, 36, 10);

        GetExpectedResourceIncome();

        g_Concrete = 45;
        g_Metal = 10;

        return true;
    }

    try {
        g_Day = std::stoi(src[0], NULL, 10);
        g_Concrete = std::stoi(src[1], NULL, 10);
        g_Metal = std::stoi(src[2], NULL, 10);
        g_People = g_Unemployed = std::stoi(src[3], NULL, 10);
        g_HabitationCapacity = 0;
        g_DaysToRescue = std::stoi(src[4], NULL, 10);
        g_IsRadioTowerBuilt = false;
        unsigned short buildingsCount = std::stoi(src[5], NULL, 10);
        if (!m_WorldMap->Deserialize(std::vector<std::string>(src.begin() + 6 + buildingsCount * 4, src.end()))) {
            return false;
        }

        // Load saved buildings
        for (unsigned short i = 6; i < buildingsCount * 4 + 6; i += 4) {
            unsigned short ID = std::stoi(src[i], NULL, 10);
            unsigned short workers = std::stoi(src[i + 1], NULL, 10);
            unsigned short posX = std::stoi(src[i + 2], NULL, 10);
            unsigned short posY = std::stoi(src[i + 3], NULL, 10);
            if (ID >= g_BuildingTemplates.size()) {
                return false;
            }
            if (ID == 2) {
                g_IsRadioTowerBuilt = true;
            }
            std::shared_ptr<CBuilding> building = std::make_shared<CBuilding>(*(g_BuildingTemplates[ID]));
            g_Concrete += building.get()->priceConcrete;
            g_Metal += building.get()->priceMetal;
            m_WorldMap->Build(building, posX, posY);

            SetEmployees(building, workers);
        }

    } catch (...) {
        return false;
    }

    return true;
}

std::vector<std::string> CGameEngine::Serialize() const {
    std::vector<std::string> res;
    // Save resources
    res.push_back(std::to_string(g_Day));
    res.push_back(std::to_string(g_Concrete));
    res.push_back(std::to_string(g_Metal));
    res.push_back(std::to_string(g_People));
    res.push_back(std::to_string(g_DaysToRescue));

    std::vector<std::shared_ptr < CBuilding>> buildings;
    buildings.insert(buildings.end(), g_Habitation.begin(), g_Habitation.end());
    buildings.insert(buildings.end(), g_Buildings.begin(), g_Buildings.end());

    res.push_back(std::to_string(buildings.size()));

    // Save all buildings
    for (auto building : buildings) {
        for (unsigned int index = 0; index < g_BuildingTemplates.size(); index++) {
            if (building.get()->name == g_BuildingTemplates[index].get()->name) {
                res.push_back(std::to_string(index));
                res.push_back(std::to_string(building.get()->workers));
                res.push_back(std::to_string(building.get()->posX));
                res.push_back(std::to_string(building.get()->posY));
                break;
            }
        }
    }

    // Save map
    std::vector<std::string> serializedMap = m_WorldMap->Serialize();
    res.insert(res.end(), serializedMap.begin(), serializedMap.end());

    return res;
}

void CGameEngine::ShowTileInfo(CWorldTile * tile) const {
    werase(m_BottomWin);
    wmove(m_BottomWin, 0, 0);
    if (tile->building) {
        tile->building.get()->Print(m_BottomWin);
    } else {
        switch (tile->type) {
            case CWorldTileType::DUST:
                wprintw(m_BottomWin, "Dust.");
                break;
            case CWorldTileType::REGOLITH:
                wprintw(m_BottomWin, "Regolith.\n");
                break;
            case CWorldTileType::SHIP:
                wprintw(m_BottomWin, "Ship parts.");
                break;
        }
    }

    wmove(m_BottomWin, 4, 0);
    wprintw(m_BottomWin, CGameBase::get().TextUI["INFO_CONTROLS_MAP"].c_str());
    wrefresh(m_BottomWin);
}

void CGameEngine::ShowBlueprintInfo(std::string info) const {
    werase(m_BottomWin);
    wmove(m_BottomWin, 0, 0);
    wprintw(m_BottomWin, info.c_str());

    unsigned int offset = 1;
    wmove(m_BottomWin, offset, 0);

    wmove(m_BottomWin, 4, 0);
    wprintw(m_BottomWin, CGameBase::get().TextUI["INFO_CONTROLS_BLUEPRINT"].c_str());
    wrefresh(m_BottomWin);
}

void CGameEngine::ShowTutorial() const {
    std::string text = CGameBase::TextStory["TUTORIAL_1"] +
            CGameBase::TextUI["GENERAL_DAY"] + CGameBase::TextStory["TUTORIAL_DAY_DESC"] + "\n \n" +
            CGameBase::TextUI["GENERAL_PEOPLE"] + CGameBase::TextStory["TUTORIAL_PEOPLE_DESC"] + "\n \n" +
            CGameBase::TextUI["GENERAL_POWER"] + CGameBase::TextStory["TUTORIAL_POWER_DESC"] + "\n \n" +
            CGameBase::TextUI["GENERAL_CONCRETE"] + CGameBase::TextStory["TUTORIAL_CONCRETE_DESC"] + "\n \n" +
            CGameBase::TextUI["GENERAL_METAL"] + CGameBase::TextStory["TUTORIAL_METAL_DESC"] + " \n";
    CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_BottomWin, text, false));
}

std::vector<std::shared_ptr<CBuilding>> CGameEngine::GetAvailableBuildings() const {
    std::vector<std::shared_ptr < CBuilding>> res;
    // Cubicle
    res.push_back(g_BuildingTemplates[3]);
    // Basic Generator
    res.push_back(g_BuildingTemplates[4]);
    // Concrete extractor
    if (g_Day >= 2) {
        res.insert(res.end(), g_BuildingTemplates.begin() + 5, g_BuildingTemplates.end());
    }
    // Advanced Power Generator
    if (g_Day >= 12) {
        res.push_back(g_BuildingTemplates[0]);
    }
    // Metal Factory
    if (g_Day >= 16) {
        res.push_back(g_BuildingTemplates[1]);
    }
    // Radio Tower - only when none is built yet
    if (g_Day >= 24 && !g_IsRadioTowerBuilt) {
        res.push_back(g_BuildingTemplates[2]);
    }
    return res;
}

void CGameEngine::PrepareBuild(std::shared_ptr<CBuilding> building) const {
    m_WorldMap->BlueprintMode(std::make_shared<CBuilding>(*building));
}

void CGameEngine::Build(std::shared_ptr<CBuilding> building) {
    if (building.get()->habitable) {
        if (g_People > g_HabitationCapacity) {
            building.get()->workers = std::min((int) building.get()->capacity, g_People - g_HabitationCapacity);
            g_NetPowerEarnings -= building.get()->GetPowerConsumption();
        }
        g_Habitation.push_back(building);
        g_HabitationCapacity += building.get()->capacity;
    } else {
        g_Buildings.push_back(building);
        SetEmployees(building, building.get()->capacity);
    }

    g_Concrete -= building.get()->priceConcrete;
    g_Metal -= building.get()->priceMetal;

    // If player built a Radio Tower
    if (building.get()->name == g_BuildingTemplates[2].get()->name) {
        g_IsRadioTowerBuilt = true;
        CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_BottomWin, CGameBase::TextStory["RADIO_BUILT"], false));
    }

    werase(m_BottomWin);
    wrefresh(m_BottomWin);
}

unsigned int CGameEngine::GetMetal() const {
    return g_Metal;
}

unsigned int CGameEngine::GetConcrete() const {
    return g_Concrete;
}

unsigned int CGameEngine::GetPowerEarnings() const {
    unsigned int generatedPower = 0;
    for (std::shared_ptr<CBuilding> building : g_Buildings) {
        generatedPower += building.get()->GetPowerEarnings();
    }
    return generatedPower;
}

void CGameEngine::ComputeConcreteMetalIncome(unsigned int & availablePower, short & totalPowerBalance) {
    g_ConcreteIncome = 0;
    g_MetalIncome = 0;
    g_RadioTransmitted = false;

    std::shared_ptr<CBuilding> radio;
    for (std::shared_ptr<CBuilding> building : g_Buildings) {
        if (building.get()->name == g_BuildingTemplates[2].get()->name) {
            radio = building;
            continue;
        }
        unsigned int consumption = building.get()->GetPowerConsumption();
        // If enough power, full income is added
        if (consumption <= availablePower) {
            g_ConcreteIncome += building.get()->GetConcreteEarnings();
            g_MetalIncome += building.get()->GetMetalEarnings();
            availablePower -= consumption;
        }

        // Otherwise only part of income is added
        else {
            g_ConcreteIncome += (unsigned int) (building.get()->GetConcreteEarnings() * (1.0 * availablePower / consumption));
            g_MetalIncome += (unsigned int) (building.get()->GetMetalEarnings() * (1.0 * availablePower / consumption));
            availablePower = 0;
        }
        totalPowerBalance -= consumption;
    }

    // If there's a worker in Radio Tower and there's enough power, call for help is made
    if (radio && radio.get()->workers == radio.get()->capacity) {
        unsigned int consumption = radio.get()->GetPowerConsumption();
        totalPowerBalance -= consumption;
        if (availablePower >= consumption) {
            availablePower -= consumption;
            g_RadioTransmitted = true;
        }
    }

}

void CGameEngine::SetEmployees(std::shared_ptr<CBuilding> building, unsigned short amount) {
    amount = std::min(amount, building.get()->capacity);
    // Removing workers only adds unemployed people
    if (amount <= building.get()->workers) {
        g_Unemployed += building.get()->workers - amount;
        building.get()->workers = amount;
    } else {
        unsigned short needed = amount - building.get()->workers;
        unsigned short available = std::min(g_Unemployed, needed);
        g_Unemployed -= available;
        building.get()->workers += available;
    }
    GetExpectedResourceIncome(false);
}

void CGameEngine::RemovePeople(unsigned short amount) {
    // If removing all of them, end the game
    if (amount >= g_People) {
        g_People = 0;
        FinishGame(false);
        return;
    }

    // Remove from homes
    if (g_People > g_HabitationCapacity && g_People - g_HabitationCapacity >= amount) {
        g_People -= amount;
    } else {
        unsigned short left = amount;
        if (g_People > g_HabitationCapacity) {
            left -= g_People - g_HabitationCapacity;
            g_People = g_HabitationCapacity;
        }
        for (unsigned int index = 0; index < g_Habitation.size() && left > 0; index++) {
            unsigned int remove = std::min(left, g_Habitation[index].get()->workers);
            g_Habitation[index].get()->workers -= remove;
            g_People -= remove;
            left -= remove;
        }
    }

    // Remove from jobs
    if (g_Unemployed >= amount) {
        g_Unemployed -= amount;
    } else {
        amount -= g_Unemployed;
        g_Unemployed = 0;
        for (unsigned int index = 0; index < g_Buildings.size() && amount > 0 && g_People > 0; index++) {
            unsigned int remove = std::min(amount, g_Buildings[index].get()->workers);
            g_Buildings[index].get()->workers -= remove;
            amount -= remove;
        }
    }

}

void CGameEngine::AddPeople(unsigned short amount) {
    g_Unemployed += amount;

    if (g_People >= g_HabitationCapacity) {
        g_People += amount;
        return;
    }

    g_People += amount;

    // Assign people to cubicles if possible
    for (unsigned int index = 0; index < g_Habitation.size() && amount > 0; index++) {
        unsigned int add = std::min((int) amount, g_Habitation[index].get()->capacity - g_Habitation[index].get()->workers);
        g_Habitation[index].get()->workers += add;
        amount -= add;
    }

}

std::vector<std::string> CGameEngine::DoDailyAction(std::shared_ptr<CWorldAction> action) {
    std::vector<std::string> res;
    // Check if there are any disasters
    if (action.get()->destructionChance > 0 || action.get()->powerDrop > 0) {
        res.push_back(action.get()->name + CGameBase::get().TextStory["DISASTER"]);
        if (action.get()->destructionChance > 0) {
            int destroyed = 0;
            for (auto iter = g_Buildings.begin(); iter != g_Buildings.end();) {
                int chance = rand() % 100 + 1;
                if (action.get()->destructionChance >= chance) {
                    // Destroy building
                    g_Unemployed += (*iter).get()->workers;
                    m_WorldMap->RemoveBuilding((*iter));
                    if (selectedBuilding && selectedBuilding.get()->posX == (*iter).get()->posX
                            && selectedBuilding.get()->posY == (*iter).get()->posY) {
                        selectedBuilding.reset();
                    }
                    // If destroyed Radio Tower, change the status
                    if ((*iter).get()->name == g_BuildingTemplates[2].get()->name) {
                        g_IsRadioTowerBuilt = false;
                    }
                    iter = g_Buildings.erase(iter);
                    destroyed++;
                } else {
                    iter++;
                }
            }
            std::string amountDestroyed = (destroyed == 0) ? "none" : std::to_string(destroyed);
            res.push_back(CGameBase::get().TextStory["BUILDINGS_DESTROYED_1"] + amountDestroyed + CGameBase::get().TextStory["BUILDINGS_DESTROYED_2"]);
        } else {
            res.push_back(CGameBase::get().TextStory["POWER_DROP_1"] + std::to_string(action.get()->powerDrop) + CGameBase::get().TextStory["POWER_DROP_2"]);
        }
    }
    res.push_back(CGameBase::get().TextStory["FOUND_ITEM"]);

    g_Metal += action.get()->metal;
    // If a player has no Concrete income, give him some
    bool giveConcrete = g_ConcreteIncome == 0 && g_Day > 5 && rand() % 100 < 50;
    if (giveConcrete) {
        res.push_back(CGameBase::get().TextUI["GENERAL_CONCRETE"] + ": 2");
        g_Concrete += 2;
    }
    if (action.get()->metal > 0) {
        res.push_back(CGameBase::get().TextUI["GENERAL_METAL"] + ": " + std::to_string(action.get()->metal));
    }
    if (action.get()->people > 0) {
        res.push_back(CGameBase::get().TextUI["GENERAL_PEOPLE"] + ": " + std::to_string(action.get()->people));
    } else if (action.get()->metal == 0 && !giveConcrete) {
        res.push_back(CGameBase::get().TextStory["NOTHING"]);
    }
    AddPeople(action.get()->people);
    return res;
}

void CGameEngine::ShowResourcesBar() const {
    // Display day
    std::string tmp = " " + CGameBase::TextUI["GENERAL_DAY"] + " " + std::to_string(g_Day);
    wprintw(m_Window, "%-10s", tmp.c_str());

    // Display people
    tmp = "| " + CGameBase::TextUI["GENERAL_PEOPLE"] + ": ";
    if (g_People > g_HabitationCapacity) {
        wprintw(m_Window, tmp.c_str());
        tmp = std::to_string(g_People);
        wattron(m_Window, COLOR_PAIR(CColor::WARNING));
        wprintw(m_Window, tmp.c_str());
        wattroff(m_Window, COLOR_PAIR(CColor::WARNING));
        tmp = "/" + std::to_string(g_Unemployed) + "/" + std::to_string(g_HabitationCapacity);
        wprintw(m_Window, tmp.c_str());
    } else {
        wprintw(m_Window, "%s%s/%s/%s", tmp.c_str(), std::to_string(g_People).c_str(), std::to_string(g_Unemployed).c_str(), std::to_string(g_HabitationCapacity).c_str());
    }

    wmove(m_Window, m_MapHeight + 1, 30);
    PrintResource(std::string(1, CGameBase::TextUI["GENERAL_POWER"][0]), g_Power, g_NetPowerEarnings, CColor::SYMBOL_POWER);

    wmove(m_Window, m_MapHeight + 1, 46);
    PrintResource(std::string(1, CGameBase::TextUI["GENERAL_CONCRETE"][0]), g_Concrete, g_ConcreteIncome, CColor::SYMBOL_CONCRETE);

    wmove(m_Window, m_MapHeight + 1, 62);
    PrintResource(std::string(1, CGameBase::TextUI["GENERAL_METAL"][0]), g_Metal, g_MetalIncome, CColor::SYMBOL_METAL);
}

void CGameEngine::PrintResource(const std::string& s, unsigned int current, int next, const CColor & color) const {
    wprintw(m_Window, "| ");
    wattron(m_Window, COLOR_PAIR(color));
    wprintw(m_Window, s.c_str());
    wattroff(m_Window, COLOR_PAIR(color));
    wprintw(m_Window, ":");

    std::string next_str = std::to_string(next);
    if (next > 0 && s != std::string(1, CGameBase::TextUI["GENERAL_POWER"][0])) {
        next_str = "+" + next_str;
    }
    wprintw(m_Window, ((std::string)(" " + std::to_string(current) + " (")).c_str());

    if (next > 0) {
        wattron(m_Window, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
    } else if (next < 0 || (next == 0 && s == std::string(1, CGameBase::TextUI["GENERAL_POWER"][0]))) {
        wattron(m_Window, COLOR_PAIR(CColor::RESOURCE_INCOME_NEGATIVE));
    }
    wprintw(m_Window, next_str.c_str());
    wattroff(m_Window, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
    wattroff(m_Window, COLOR_PAIR(CColor::RESOURCE_INCOME_NEGATIVE));
    wprintw(m_Window, ")");
}

void CGameEngine::FinishGame(bool victory) const {
    std::string text;
    int score = 10 * (g_Day + ((victory) ? (50 - g_Day) : 0)) + 5 * g_People + (g_Buildings.size() + g_Habitation.size());
    
    if (victory) {
        text = CGameBase::get().TextUI["ENDGAME_VICTORY"] + std::to_string(score);
    } else {
        text = CGameBase::get().TextUI["ENDGAME_GAMEOVER"] + std::to_string(score);
    }
    
    // Display result and then Main menu
    CGameBase::get().ReplaceInterfaceElements(new CMenuMain(m_Window));
    CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_Window, text));

    if (g_Day == 50 && !victory) {
        CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_Window, CGameBase::get().TextUI["ENDGAME_DISASTER"]));
    }
}

