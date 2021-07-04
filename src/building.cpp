#include "building.hpp"
#include "gameBase.hpp"

CBuilding::CBuilding(std::string name, std::string desc,
        unsigned short width, unsigned short height,
        unsigned short capacity, unsigned short powerConsumption,
        unsigned short powerIncome, unsigned short concreteIncome,
        unsigned short metalIncome, unsigned short concreteCost,
        unsigned short metalCost, char symbol, bool habitable)
: name(name), description(desc), width(width),
height(height), priceConcrete(concreteCost), priceMetal(metalCost),
capacity(capacity), powerConsumption(powerConsumption), maxConcrete(concreteIncome),
maxMetal(metalIncome), maxPower(powerIncome), symbol(symbol), habitable(habitable) {

}

CBuilding::~CBuilding() {

}

unsigned int CBuilding::GetPowerEarnings() const {
    return maxPower * workers / capacity;
}

unsigned int CBuilding::GetConcreteEarnings() const {
    return maxConcrete * workers / capacity;
}

unsigned int CBuilding::GetMetalEarnings() const {
    return maxMetal * workers / capacity;
}

unsigned int CBuilding::GetPowerConsumption() const {
    return powerConsumption * workers / capacity;
}

bool CBuilding::CanBeBuiltAt(const unsigned short x, const unsigned short y, CWorldMap * map) const {
    int regolithTiles = 0;

    // Extractor can only be built on Regolith tiles.
    // No other building can be built there. No building can be built over Spaceship parts.
    for (unsigned int pX = x; pX < x + width; pX++) {
        for (unsigned int pY = y; pY < y + height; pY++) {
            CWorldTile & tile = map->GetTile(pX, pY);
            if (tile.building) {
                return false;
            }
            if (tile.type == CWorldTileType::SHIP) {
                return false;
            } else if (tile.type == CWorldTileType::REGOLITH) {
                regolithTiles++;
            }
        }
    }

    if ((maxConcrete > 0 && regolithTiles == 0) || (maxConcrete == 0 && regolithTiles > 0)) {
        return false;
    }

    return true;
}

void CBuilding::Print(WINDOW* win, bool buildingMenu) const {
    unsigned int offset = 0;
    std::string text;

    // Printing for Build menu shows max workers, max production and price
    if (buildingMenu) {
        offset++;
        mvwprintw(win, offset++, 0, description.c_str());
        text = (habitable) ?
                CGameBase::get().TextUI["GENERAL_CAPACITY"] :
                CGameBase::get().TextUI["GENERAL_WORKERS"];
        text += ": " + std::to_string(capacity);
        mvwprintw(win, offset++, 0, text.c_str());

        if (maxConcrete > 0) {
            text = CGameBase::get().TextUI["GENERAL_CONCRETE"] + ": ";
            mvwprintw(win, offset++, 0, text.c_str());
            wattron(win, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
            wprintw(win, "+%s", std::to_string(maxConcrete).c_str());
            wattroff(win, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
        }
        if (maxMetal > 0) {
            text = CGameBase::get().TextUI["GENERAL_METAL"] + ": ";
            mvwprintw(win, offset++, 0, text.c_str());
            wattron(win, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
            wprintw(win, "+%s", std::to_string(maxMetal).c_str());
            wattroff(win, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
        }

        int width, height;
        getmaxyx(win, height, width);

        for (int y = 0; y < height; y++) {
            mvwaddch(win, y, width - 10, ACS_VLINE);
        }

        offset = 0;
        mvwprintw(win, offset++, width - 9, "%s:", CGameBase::get().TextUI["GENERAL_COST"].c_str());
        wattron(win, COLOR_PAIR(CColor::SYMBOL_CONCRETE));
        mvwaddch(win, offset, width - 9, CGameBase::get().TextUI["GENERAL_CONCRETE"][0]);
        wattroff(win, COLOR_PAIR(CColor::SYMBOL_CONCRETE));
        wprintw(win, ": %s", std::to_string(priceConcrete).c_str());

        if (priceMetal > 0) {
            wattron(win, COLOR_PAIR(CColor::SYMBOL_METAL));
            mvwaddch(win, ++offset, width - 9, CGameBase::get().TextUI["GENERAL_METAL"][0]);
            wattroff(win, COLOR_PAIR(CColor::SYMBOL_METAL));
            wprintw(win, ": %s", std::to_string(priceMetal).c_str());
        }

        wattron(win, COLOR_PAIR(CColor::SYMBOL_POWER));
        mvwaddch(win, ++offset, width - 9, CGameBase::get().TextUI["GENERAL_POWER"][0]);
        wattroff(win, COLOR_PAIR(CColor::SYMBOL_POWER));
        wprintw(win, ": ");
        std::string power;
        if (maxPower > 0){
            wattron(win, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
            power = "+" + std::to_string(maxPower);
        }
        else {
            wattron(win, COLOR_PAIR(CColor::RESOURCE_INCOME_NEGATIVE));
            power = "-" + std::to_string(powerConsumption);
        }
        wprintw(win, "%s", power.c_str());
        wattroff(win, COLOR_PAIR(CColor::RESOURCE_INCOME_POSITIVE));
        wattroff(win, COLOR_PAIR(CColor::RESOURCE_INCOME_NEGATIVE));
    } 
    // Printing existing buildings prints current amount of workers and current production
    else {
        mvwprintw(win, offset++, 0, name.c_str());
        text = ((habitable) ?
                CGameBase::get().TextUI["GENERAL_INHABITANTS"] :
                CGameBase::get().TextUI["GENERAL_WORKERS"]) + " : ";
        std::string work = std::to_string(workers) + "/" + std::to_string(capacity);
        if (habitable) {
            mvwprintw(win, offset++, 0, "%s%s", text.c_str(), work.c_str());
        } else {
            mvwprintw(win, offset++, 0, text.c_str());
            if (workers > 0) {
                wattron(win, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
                wprintw(win, "-");
                wattroff(win, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
                wprintw(win, " ");
            } else {
                wprintw(win, "- ");
            }
            wprintw(win, work.c_str());
            if (workers < capacity) {
                wprintw(win, " ");
                wattron(win, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
                wprintw(win, "+");
                wattroff(win, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
            } else {
                wprintw(win, " +");
            }
        }
        if (maxPower > 0) {
            text = CGameBase::get().TextUI["GENERAL_POWER"] + ": +" + std::to_string(GetPowerEarnings());
            mvwprintw(win, offset++, 0, text.c_str());
        }
        if (maxConcrete > 0) {
            text = CGameBase::get().TextUI["GENERAL_CONCRETE"] + ": +" + std::to_string(GetConcreteEarnings());
            mvwprintw(win, offset++, 0, text.c_str());
        }
        if (maxMetal > 0) {
            text = CGameBase::get().TextUI["GENERAL_METAL"] + ": +" + std::to_string(GetMetalEarnings());
            mvwprintw(win, offset++, 0, text.c_str());
        }
        if (powerConsumption > 0) {
            text = CGameBase::get().TextUI["GENERAL_POWER"] + 
                    ((GetPowerConsumption() > 0) ? ": -" : ": ") 
                    + std::to_string(GetPowerConsumption());
            mvwprintw(win, offset++, 0, text.c_str());
        }

    }
}
