#include "dataStore.hpp"
#include "gameBase.hpp"
#include <iostream>

CTextUI::CTextUI() {
    m_Data = {
        {"MENU_MAIN_TITLE", "Building Strategy game"},
        {"MENU_MAIN_NEWGAME", "New game"},
        {"MENU_MAIN_LOAD", "Load game"},
        {"MENU_MAIN_EXIT", "Quit"},

        {"MENU_LOAD_TITLE", "Load game"},
        {"MENU_LOAD_INFO", "Press Enter to load, press Space to return."},

        {"MENU_PAUSE_TITLE", "Game paused"},
        {"MENU_PAUSE_CONTINUE", "Continue"},
        {"MENU_PAUSE_CONTROLS", "Controls"},
        {"MENU_PAUSE_SAVE", "Save"},
        {"MENU_PAUSE_QUIT", "Exit to Main menu"},

        {"PANEL_OK", "OK"},
        {"PANEL_CONTINUE", "Press [Enter] to continue."},

        {"PANEL_RESIZE_TITLE", "This game only works with 80x24 terminal resolution."},
        {"PANEL_RESIZE_INFO", "Please resize your terminal window."},
        {"PANEL_RESIZE_CURRENTSIZE", "Current size:"},

        {"PANEL_SAVE_SUCCESS", "Game saved successfully."},
        {"PANEL_SAVE_ERROR", "An error occurred while attempting to save your game."},

        {"PANEL_LOAD_ERROR", "An error occurred while trying to load required files. Game can not continue."},

        {"PANEL_HELP_TEXT", "Controls\n\nArrow keys - Moving around the map\n[M]     -    Return to Map  \n  "
            "[B]     -  Open Buildings menu\n[Space]   -      Next day       \n[F10]    -     Pause menu     "},

        {"INFO_CONTROLS_MAP", "Press B to open Building menu, [Space] to end the day, [F9] to pause the game."},
        {"INFO_CONTROLS_BUILDING", "Use arrow keys to choose, press [Enter] to build, M to return to Map."},
        {"INFO_CONTROLS_BLUEPRINT", "Use arrow keys to choose location, press [Enter] to build or [x] to cancel."},
        {"INFO_NOT_ENOUGH_RESOURCES", "Not enough resources to build "},

        {"GENERAL_DAY", "Day"},
        {"GENERAL_COST", "Cost"},
        {"GENERAL_CAPACITY", "Capacity"},
        {"GENERAL_INHABITANTS", "Inhabitants"},
        {"GENERAL_WORKERS", "Workers"},
        {"GENERAL_PEOPLE", "People"},
        {"GENERAL_POWER", "Power"},
        {"GENERAL_CONCRETE", "Concrete"},
        {"GENERAL_METAL", "Metal"},

        {"GENERAL_CAN_BUILD", " can be built here."},
        {"GENERAL_CAN_NOT_BUILD", " can NOT be built here."},

        {"ENDGAME_DISASTER", "Day 50\nA massive asteroid hit the planet, killing everyone in your colony."},
        {"ENDGAME_GAMEOVER", "Game over\nEveryone died. Better luck next time.\nScore: "},
        {"ENDGAME_VICTORY", "Victory\nYou were rescued and won the game.\nScore: "},

    };
}

std::string CTextUI::operator[](const std::string& s) {
    return m_Data[s];
}

//------------------------------------------------------------------------------

CTextStory::CTextStory() {
    m_Data = {
        {"DISASTER", " just hit your colony!"},
        {"BUILDINGS_DESTROYED_1", "It destroyed "},
        {"BUILDINGS_DESTROYED_2", " of your buildings."},
        {"POWER_DROP_1", "Generators produced "},
        {"POWER_DROP_2", "%% less power today."},
        {"FOUND_ITEM", "Scouts have returned from the ship and found the following:"},
        {"NOTHING", "Nothing"},
        {"PEOPLE_DIED_SING", " person died overnight."},
        {"PEOPLE_DIED_PLUR", " people died overnight."},
        
        {"RADIO_BUILT", "You've built a Radio Tower! Once you put a worker inside, he will try to "
            "call for help as long as there's enough Power. It might take a few days for a rescue ship to arrive."},

        {"INTRO_1", "Year 2315. A group of brave explorers went into the deep space to "
            "look for interesting planets. They were scanning each one of them for scientific "
            "purposes. This last planet was very uninteresting. No valuable materials, "
            "heavy electromagnetic storms, earthquakes and other disasters were happening "
            "very often. Research shows that a huge asteroid is going to hit the planet, "
            "approximately in 50 days."},
        {"INTRO_2", "Since the research hasn't shown anything interesting, the captain decided "
            "to fly away and search for more planets. Just when they were about to leave, "
            "a massive solar flare hit the ship, temporarily disabling every electronic device. "
            "After a few unsuccessful attempts to start the engines, the ship got pulled by "
            "the planet's gravitation and crashed into the surface."},
        {"INTRO_3", "After the crash, some people managed to get out of the ship and "
            "craft a basic cubicle to keep them safe. They also built a simple power generator and "
            "use it for generating oxygen inside cubicles. There might be more survivors "
            "still in the ship. Once the sun sets, a group of people will go into the wreck "
            "and look for them."},
        {"INTRO_4", "Since this planet has no useful materials except regolith deposits, "
            "the options are limited. Some people in the ship might be capable of building "
            "a concrete extractor to extract concrete from regolith deposits. It can later be used to "
            "build other structures. Buildings also need metal parts. Looks like the only way "
            "to obtain them is to dismantle the ship."},
        {"TUTORIAL_1", "Tutorial\n"
            "At the top you can see the world map. Moving the '+' cursor over any place shows "
            "more detailed info about it.\n"
            "Below the world map is Resources panel, containing the following items:\n"},
        {"TUTORIAL_DAY_DESC", ": 1 - Days since the crash."},
        {"TUTORIAL_PEOPLE_DESC", ": total/unemployed/accommodated - People without accommodation die overnight."},
        {"TUTORIAL_POWER_DESC", ": current level (next day's level) - Leftover power is not transferred to the next day. "
            "Power is used to make other buildings work. If any cubicle runs out of power, people inside die."},
        {"TUTORIAL_CONCRETE_DESC", ": amount (mined per day) - Created from Regolith using Extractors. "
            "Used as a primary building material."},
        {"TUTORIAL_METAL_DESC", ": amount (created per day) - Acquired by dismantling the ship. Used as a building material."},

        {"DAY_2", "One of the survivors knows how to build a Concrete Extractor. You can find it in the Building menu."},
        {"DAY_4", "A small electromagnetic storm is approaching. It will probably lower your power income."},
        {"DAY_8", "People remembered there is an asteroid incoming. According to previous "
            "simulations, no life form has a chance to survive the impact. You need to escape before Day 50."},
        {"DAY_9", "The atmosphere is very unsteady today. Expect another event soon."},
        {"DAY_12", "A group of engineers found a way to make a better Power Generator. "
            "You can find it in the Building menu."},
        {"DAY_14", "Scientists are investigating the surprisingly high amount of natural disasters. Looks like you might "
            "experience another one tomorrow."},
        {"DAY_16", "Dust can be useful after all! Turns out it contains small amounts of metal pieces. "
            "Using a very energy-intensive process you will be able to create Metal. Metal Factory was unlocked, "
            "you can find it in the Building menu."},
        {"DAY_19", "According to a very accurate scientific research, there's on average one disaster every 5 days. "
            "And the next one is incoming soon."},
        {"DAY_24", "While dismantling the ship, one of the scouts found a radio transmitter. "
            "It's not powerful enough on it's own, but you could build a huge antenna and use it to call for help!\n"
            "Radio Tower unlocked, you can find it in the Building menu."}
    };
}

std::string CTextStory::operator[](const std::string& s) {
    return m_Data[s];
}

//------------------------------------------------------------------------------

CDailyActionData::CDailyActionData() {
    // Load action data from file
    std::vector<std::string> data = CGameBase::get().Load(CFileIO::dataFolder + "action");
    try {
        std::string name;
        int day = 2;
        for (unsigned int i = 0; i < data.size(); i += 4) {
            name = "";
            unsigned short people = std::stoi(data[i], NULL, 10);
            unsigned short metal = std::stoi(data[i + 1], NULL, 10);
            unsigned short power = std::stof(data[i + 2]);
            unsigned short demolition = std::stof(data[i + 3]);
            if (power > 0 || demolition > 0) {
                name = data[i + 4];
                i++;
            }
            m_Data[day++] = std::make_shared<CWorldAction>(people, metal, power, demolition, name);
        }
    } catch (...) {
        return;
    }

}

std::shared_ptr<CWorldAction> CDailyActionData::operator[](unsigned short day) {
    if (!m_Data[day]) {
        m_Data[day] = std::make_shared<CWorldAction>(0, 0, 0, 0, "");
    }
    return m_Data[day];
}
