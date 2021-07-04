#include <string>
#include <iostream>
#include <ncurses.h>
#include "dataStore.hpp"
#include "menu.hpp"
#include "infoPanel.hpp"
#include "colors.hpp"
#include "controls.hpp"
#include "gameBase.hpp"

#define ENTER_KEY '\n'

CMenu::CMenu(WINDOW * w) : CInterfaceElement(w), m_Selected(0) {

}

CMenu::~CMenu() {
}

void CMenu::onUp() {
    if (m_Selected > 0) {
        m_Selected--;
    }
}

void CMenu::onDown() {
    if (m_Selected < m_Items.size() - 1) {
        m_Selected++;
    }
}

void CMenu::onLeft() {
}

void CMenu::onRight() {
}

//------------------------------------------------------------------------------

CMenuMain::CMenuMain(WINDOW * w) : CMenu(w) {
    m_Items.push_back(CGameBase::get().TextUI["MENU_MAIN_NEWGAME"]);
    m_Items.push_back(CGameBase::get().TextUI["MENU_MAIN_LOAD"]);
    m_Items.push_back(CGameBase::get().TextUI["MENU_MAIN_EXIT"]);
    m_Title = CGameBase::get().TextUI["MENU_MAIN_TITLE"];
}

void CMenuMain::Show() {
    werase(m_Window);

    short int offset = -(m_Items.size() + 1) / 2;

    // Print title
    wattron(m_Window, COLOR_PAIR(CColor::MENU_TITLE));
    wattron(m_Window, A_BOLD);
    wmove(m_Window, m_Height / 2 + offset * 2, m_Width / 2 - m_Title.length() / 2);
    wprintw(m_Window, m_Title.c_str());

    wattroff(m_Window, A_BOLD);
    offset++;

    // Print menu items
    for (unsigned int i = 0; i < m_Items.size(); i++) {
        std::string text = m_Items[i];
        if (i == m_Selected) {
            wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
            text = "[" + text + "]";
        } else
            wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
        wmove(m_Window, m_Height / 2 + offset * 2, m_Width / 2 - text.length() / 2);
        wprintw(m_Window, text.c_str());
        offset++;
    }
    wrefresh(m_Window);

}

void CMenuMain::RegisterKeys() {
    CControls & c = CControls::getInstance();
    c.ClearAssignedKeys();
    c.AssignKey(KEY_UP, [this]() {
        this->onUp();
    });
    c.AssignKey(KEY_DOWN, [this]() {
        this->onDown();
    });
    c.AssignKey(ENTER_KEY, [this]() {
        this->onEnter();
    });
}

void CMenuMain::onEnter() {
    if (m_Selected == 0)
        CGameBase::get().NewGame();
    if (m_Selected == 1)
        CGameBase::get().ReplaceInterfaceElements(new CMenuLoadGame(m_Window));
    if (m_Selected == 2)
        CGameBase::get().Quit();
}

//------------------------------------------------------------------------------

CMenuLoadGame::CMenuLoadGame(WINDOW * w) : CMenu(w) {
    std::vector<std::string> games = CGameBase::get().GetSavedGamesFilenames();
    m_Items.insert(m_Items.begin(), games.begin(), games.end());

    m_Title = CGameBase::get().TextUI["MENU_LOAD_TITLE"];
    m_Info = CGameBase::get().TextUI["MENU_LOAD_INFO"];

    // Load items and show border
    m_SavedWin = newwin(m_SavedWinHeight, m_SavedWinWidth, (m_Height - m_SavedWinHeight) / 2, (m_Width - m_SavedWinWidth) / 2);
    box(m_SavedWin, ACS_VLINE, ACS_HLINE);
    werase(m_Window);

}

CMenuLoadGame::~CMenuLoadGame() {
    delwin(m_SavedWin);
}

void CMenuLoadGame::Show() {
    wattron(m_SavedWin, COLOR_PAIR(CColor::MENU_ITEM));
    short int offset = -(m_SavedWinHeight) / 2;

    // Print title and instructions
    wattron(m_Window, A_BOLD);
    wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
    wmove(m_Window, m_Height / 2 + offset * 2, m_Width / 2 - m_Title.length() / 2);
    wprintw(m_Window, m_Title.c_str());

    offset++;

    wattroff(m_Window, A_BOLD);

    wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
    wmove(m_Window, m_Height / 2 + offset * 2, m_Width / 2 - m_Info.length() / 2);
    wprintw(m_Window, m_Info.c_str());

    offset++;

    // Print menu items
    offset = 1;
    for (unsigned int i = 0; i < m_Items.size() && i < m_SavedWinHeight - 2; i++) {
        std::string text = m_Items[i];
        text.append(m_SavedWinWidth - 2 - text.length(), ' ');
        if (i == m_Selected) {
            wattron(m_SavedWin, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));

        } else {
            wattron(m_SavedWin, COLOR_PAIR(CColor::MENU_ITEM));
        }
        wmove(m_SavedWin, offset, 1);
        wprintw(m_SavedWin, text.c_str());
        offset++;
    }

    wrefresh(m_Window);
    wrefresh(m_SavedWin);

}

void CMenuLoadGame::RegisterKeys() {
    CControls & c = CControls::getInstance();
    c.ClearAssignedKeys();
    c.AssignKey(KEY_UP, [this]() {
        this->onUp();
    });
    c.AssignKey(KEY_DOWN, [this]() {
        this->onDown();
    });
    c.AssignKey(ENTER_KEY, [this]() {
        this->onEnter();
    });
    c.AssignKey(' ', [this]() {
        this->onReturn();
    });
}

void CMenuLoadGame::onDown() {
    if (m_Selected < m_Items.size() - 1 && m_Selected < m_SavedWinHeight - 3) {
        m_Selected++;
    }
}

void CMenuLoadGame::onEnter() {
    if (m_Selected < m_Items.size()) {
        CGameBase::get().LoadGame(m_Items[m_Selected]);
    }
}

void CMenuLoadGame::onReturn() {
    CGameBase::get().ReplaceInterfaceElements(new CMenuMain(m_Window));
}

//------------------------------------------------------------------------------

CMenuPause::CMenuPause(WINDOW * w) : CMenu(w) {
    m_Items.push_back(CGameBase::get().TextUI["MENU_PAUSE_CONTINUE"]);
    m_Items.push_back(CGameBase::get().TextUI["MENU_PAUSE_CONTROLS"]);
    m_Items.push_back(CGameBase::get().TextUI["MENU_PAUSE_SAVE"]);
    m_Items.push_back(CGameBase::get().TextUI["MENU_PAUSE_QUIT"]);

    m_Title = CGameBase::get().TextUI["MENU_PAUSE_TITLE"];
}

CMenuPause::~CMenuPause() {
}

void CMenuPause::Show() {

    werase(m_Window);

    short int offset = -(m_Items.size() + 2) / 2;

    // Print title and instructions
    wattron(m_Window, A_BOLD);
    wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
    wmove(m_Window, m_Height / 2 + offset * 2 + 1, m_Width / 2 - m_Title.length() / 2);
    wprintw(m_Window, m_Title.c_str());

    wattroff(m_Window, A_BOLD);

    offset += 2;

    // Print menu items
    for (unsigned int i = 0; i < m_Items.size(); i++) {
        std::string text = m_Items[i];
        if (i == m_Selected) {
            wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
            text = "[" + text + "]";
        } else
            wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
        wmove(m_Window, m_Height / 2 + offset * 2, m_Width / 2 - text.length() / 2);
        wprintw(m_Window, text.c_str());
        offset++;
    }
    wrefresh(m_Window);
}

void CMenuPause::RegisterKeys() {
    CControls & c = CControls::getInstance();
    c.ClearAssignedKeys();
    c.AssignKey(KEY_UP, [this]() {
        this->onUp();
    });
    c.AssignKey(KEY_DOWN, [this]() {
        this->onDown();
    });
    c.AssignKey(ENTER_KEY, [this]() {
        this->onEnter();
    });

}

void CMenuPause::onEnter() {
    if (m_Selected == 0)
        CGameBase::get().RemoveInterfaceElement();
    if (m_Selected == 1)
        CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_Window, CGameBase::get().TextUI["PANEL_HELP_TEXT"]));
    if (m_Selected == 2)
        CGameBase::get().Save();
    if (m_Selected == 3)
        CGameBase::get().ReplaceInterfaceElements(new CMenuMain(m_Window));
}

//------------------------------------------------------------------------------

CMenuBuild::CMenuBuild(WINDOW * w, CGameEngine * gameScreen) : CMenu(w), m_GameEngine(gameScreen) {
    // Load buildings
}

CMenuBuild::~CMenuBuild() {
}

void CMenuBuild::Show() {
    werase(m_Window);
    m_Available = m_GameEngine->GetAvailableBuildings();
    std::shared_ptr<CBuilding> chosen = m_Available[m_Selected];

    wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
    // Show left and right arrow in color if more buildings are available
    if (m_Selected > 0) {
        wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
    }
    wmove(m_Window, 0, 0);
    wprintw(m_Window, "<");
    wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
    // Show building name
    wprintw(m_Window, ((std::string)(" " + chosen.get()->name + " ")).c_str());
    if (m_Selected < m_Available.size() - 1) {
        wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
    }
    wprintw(m_Window, ">");

    // Print building description
    wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
    chosen.get()->Print(m_Window, true);

    wmove(m_Window, m_Height - 2, 0);
    wattron(m_Window, A_BOLD);
    wprintw(m_Window, CGameBase::get().TextUI["INFO_CONTROLS_BUILDING"].c_str());
    wattroff(m_Window, A_BOLD);
    wrefresh(m_Window);
}

void CMenuBuild::RegisterKeys() {
    CControls & c = CControls::getInstance();
    c.AssignKey('m', []() {
        CGameBase::get().RemoveInterfaceElement();
    });
    c.AssignKey(KEY_LEFT, [this]() {
        this->onLeft();
    });
    c.AssignKey(KEY_RIGHT, [this]() {
        this->onRight();
    });
    c.AssignKey(ENTER_KEY, [this]() {
        this->onEnter();
    });
}

void CMenuBuild::onDown() {
}

void CMenuBuild::onUp() {
}

void CMenuBuild::onLeft() {
    if (m_Selected > 0) {
        m_Selected--;
    }
}

void CMenuBuild::onRight() {
    if (m_Selected < m_Available.size() - 1) {
        m_Selected++;
    }
}

void CMenuBuild::onEnter() {
    // Check if player has enough resources for building
    if (m_GameEngine->GetConcrete() >= m_Available[m_Selected].get()->priceConcrete
            && m_GameEngine->GetMetal() >= m_Available[m_Selected].get()->priceMetal) {
        m_GameEngine->PrepareBuild(m_Available[m_Selected]);
        CGameBase::get().RemoveInterfaceElement();
    } else {
        CGameBase::get().AddInterfaceElement(new CInfoMessagePanel(m_Window, CGameBase::get().TextUI["INFO_NOT_ENOUGH_RESOURCES"] + m_Available[m_Selected].get()->name, false));
    }
}