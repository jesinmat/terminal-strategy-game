#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "gameBase.hpp"
#include "menu.hpp"
#include "colors.hpp"
#include "controls.hpp"
#include "dataStore.hpp"
#include "infoPanel.hpp"
#include "gameEngine.hpp"

CTextUI CGameBase::TextUI;
CTextStory CGameBase::TextStory;
CDailyActionData CGameBase::DailyAction;

CGameBase & CGameBase::get() {
    static CGameBase g;
    return g;
}

CGameBase::CGameBase() {
}

void CGameBase::Start() {
    initscr(); /* Start curses mode */
    start_color(); /* Start color */
    curs_set(0); /* Hide cursor */
    keypad(stdscr, TRUE); /* Accept keyboard input */
    InitColors(); /* Init color pairs */
    srand(time(NULL)); /* Init random number generator */

    AddInterfaceElement(new CInfoResizeTerminalPanel(stdscr));

    while (!shouldQuit) {
        usleep(40000);
        // Delete old elements from previous render
        for (CInterfaceElement * x : m_OldElements) {
            m_OldElements.erase(m_OldElements.begin());
            delete x;
        }
        // Display newest element
        (*(m_Elements.begin()))->Show();
        // Read keyboard input
        CControls::getInstance().ReadInput();
    }

    // Clear assigned keys
    CControls::getInstance().ClearAssignedKeys();

    // Delete all elements
    for (CInterfaceElement * x : m_OldElements) {
        m_OldElements.erase(m_OldElements.begin());
        delete x;
    }

    for (CInterfaceElement * x : m_Elements) {
        m_Elements.erase(m_Elements.begin());
        delete x;
    }
    
    // End ncurses
    endwin();
}

void CGameBase::AddInterfaceElement(CInterfaceElement * x) {
    m_Elements.insert(m_Elements.begin(), x);
    CControls::getInstance().ClearAssignedKeys();
    x->RegisterKeys();
}

void CGameBase::ReplaceInterfaceElements(CInterfaceElement * x) {
    CControls::getInstance().ClearAssignedKeys();
    // Copy element pointers to removing vector and clear active elements vector
    m_OldElements.insert(m_OldElements.end(), m_Elements.begin(), m_Elements.end());
    m_Elements.clear();
    
    m_Elements.insert(m_Elements.begin(), x);
    x->RegisterKeys();
}

void CGameBase::RemoveInterfaceElement() {
    CControls::getInstance().ClearAssignedKeys();
    if (m_Elements.size() > 0) {
        CInterfaceElement * a = *(m_Elements.begin());
        m_Elements.erase(m_Elements.begin());
        m_OldElements.push_back(a);
    }
    
    // If the last element was removed, end the game
    if (m_Elements.size() > 0) {
        ((CInterfaceElement *)*(m_Elements.begin()))->RegisterKeys();
    } else {
        shouldQuit = true;
    }
}

std::vector<std::string> CGameBase::GetSavedGamesFilenames() const {
    CFileIO * c = new CFileIO();
    std::vector<std::string> cpy = c->GetSavedGamesFilenames();
    delete c;
    return cpy;
}

void CGameBase::Quit(){
    shouldQuit = true;
}

void CGameBase::NewGame() {
    m_SavefileName = "";
    m_GameEngine = new CGameEngine(stdscr);
    if (!m_GameEngine->Deserialize(std::vector<std::string>())) {
        AddInterfaceElement(new CInfoMessagePanel(stdscr, TextUI["PANEL_LOAD_ERROR"]));
        return;
    }
    ReplaceInterfaceElements(m_GameEngine);
    m_GameEngine->showTutorial = true;
    AddInterfaceElement(new CInfoMessagePanel(stdscr, TextUI["PANEL_HELP_TEXT"]));

    AddInterfaceElement(new CInfoMessagePanel(stdscr, TextStory["INTRO_4"]));
    AddInterfaceElement(new CInfoMessagePanel(stdscr, TextStory["INTRO_3"]));
    AddInterfaceElement(new CInfoMessagePanel(stdscr, TextStory["INTRO_2"]));
    AddInterfaceElement(new CInfoMessagePanel(stdscr, TextStory["INTRO_1"]));
}

void CGameBase::LoadGame(std::string filename) {
    m_SavefileName = filename;
    m_GameEngine = new CGameEngine(stdscr);
    // Show error message if savefile was corrupted
    if (!m_GameEngine->Deserialize(Load(CFileIO::saveFolder + filename))) {
        delete m_GameEngine;
        ReplaceInterfaceElements(new CMenuLoadGame(stdscr));
        AddInterfaceElement(new CInfoMessagePanel(stdscr, TextUI["PANEL_LOAD_ERROR"]));
        return;
    }
    ReplaceInterfaceElements(m_GameEngine);
}

void CGameBase::Save() {
    std::vector<std::string> data = m_GameEngine->Serialize();
    CFileIO * c = new CFileIO();
    
    // Create timestamp
    auto t = std::time(nullptr);
    char mbstr[20];
    std::strftime(mbstr, sizeof (mbstr), "%y.%m.%d %H:%M:%S", std::localtime(&t));
    std::string filename(mbstr);

    // Show error message if save was not successful
    if (!c->Save(c->saveFolder + filename, data)) {
        AddInterfaceElement(new CInfoMessagePanel(stdscr, TextUI["PANEL_SAVE_ERROR"]));
        delete c;
        return;
    }

    AddInterfaceElement(new CInfoMessagePanel(stdscr, TextUI["PANEL_SAVE_SUCCESS"]));

    // Delete old save
    if (m_SavefileName.length() > 0) {        
        c->Delete(c->saveFolder + m_SavefileName);
    }
    delete c;
    m_SavefileName = filename;
}

std::vector<std::string> CGameBase::Load(std::string filename) const {
    CFileIO * fileio = new CFileIO();
    std::vector<std::string> res = fileio->Load(filename);
    delete fileio;
    return res;
}
