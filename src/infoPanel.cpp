#include <string>
#include "infoPanel.hpp"
#include "gameBase.hpp"
#include "menu.hpp"
#include "controls.hpp"

#define ENTER_KEY '\n'

CInfoPanel::CInfoPanel(WINDOW* w) : CInterfaceElement(w) {
}

CInfoPanel::~CInfoPanel() {
}

void CInfoPanel::RegisterKeys() {
}

//------------------------------------------------------------------------------

CInfoMessagePanel::CInfoMessagePanel(WINDOW* w, std::string message, bool center)
: CInfoPanel(w), m_Centered(center) {
    werase(m_Window);

    char delimiter = '\n';
    std::size_t newLine = message.find(delimiter);
    // Split by newlines
    while (newLine != std::string::npos) {
        std::string part = message.substr(0, newLine);
        m_Message.push_back(part);
        message = message.substr(part.size() + 1, message.size() - part.size());
        newLine = message.find(delimiter);
    }
    m_Message.push_back(message);

    std::vector<std::string> modified;
    int margin = (m_Centered) ? 20 : 3;
    // If lines are too long, split them in smaller pieces
    for (auto it = m_Message.begin(); it != m_Message.end(); it++) {
        std::string part = *it;
        while (part.size() > m_Width - margin) {
            std::string m_row = part.substr(0, m_Width - margin);
            std::size_t lastSpace = m_row.find_last_of(" ");
            if (part.size() > m_Width - margin && lastSpace != std::string::npos && lastSpace != 0)
                m_row = part.substr(0, lastSpace);

            part = part.substr(m_row.size() + 1, part.size() - m_row.size());
            modified.push_back(m_row);
        }
        if (part.size() > 0) {
            modified.push_back(part);
        }
    }
    m_Message = modified;
}

CInfoMessagePanel::CInfoMessagePanel(WINDOW* w, std::vector<std::string> message, bool center)
: CInfoPanel(w), m_Centered(center) {
    werase(m_Window);

    m_Message.insert(m_Message.begin(), message.begin(), message.end());
}

CInfoMessagePanel::~CInfoMessagePanel() {
}

void CInfoMessagePanel::Show() {
    // Display the whole message if it fits on the screen
    if (m_Height > m_Message.size() + 1) {
        m_Row = m_Message.size();
        short int offset = (m_Centered) ? (-(m_Message.size() + 4) / 2) : 0;

        for (const std::string & msg : m_Message) {
            if (m_Centered) {
                wmove(m_Window, m_Height / 2 + offset, (m_Width - msg.size()) / 2);
            } else {
                wmove(m_Window, offset, 0);
            }
            wprintw(m_Window, msg.c_str());
            offset++;
        }

        offset++;

        std::string ok = "[" + CGameBase::get().TextUI["PANEL_OK"] + "]";
        if (m_Centered) {
            wmove(m_Window, m_Height / 2 + offset, (m_Width - ok.size()) / 2);
        } else {
            wmove(m_Window, std::min(offset, (short int) (m_Height - 2)), 0);
        }
        wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
        wprintw(m_Window, ok.c_str());
        wattroff(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
    }
        // Display only part of the message and Continue text
    else {
        werase(m_Window);
        std::vector<std::string> displayable;
        unsigned int length = std::min(m_Message.size() - m_Row, m_Height - 2);
        displayable.insert(displayable.begin(), m_Message.begin() + m_Row, m_Message.begin() + m_Row + length);
        unsigned int offset = 0;

        for (const std::string & msg : displayable) {
            wprintw(m_Window, msg.c_str());
            offset++;
            wmove(m_Window, offset, 0);
        }

        std::string ok = CGameBase::get().TextUI["PANEL_CONTINUE"];
        if (m_Row + m_Height == m_Message.size() + 2) {
            ok = "[" + CGameBase::get().TextUI["PANEL_OK"] + "]";
        }
        wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
        wprintw(m_Window, ok.c_str());
        wattroff(m_Window, COLOR_PAIR(CColor::MENU_ITEM_SELECTED));
    }
    wrefresh(m_Window);
}

void CInfoMessagePanel::RegisterKeys() {
    CControls & c = CControls::getInstance();
    c.ClearAssignedKeys();
    c.AssignKey(ENTER_KEY, [this]() {
        this->onEnter();
    });
}

void CInfoMessagePanel::onEnter() {
    werase(m_Window);
    if (m_Height > m_Message.size() + 1 || m_Row + m_Height == m_Message.size() + 2) {
        CGameBase::get().RemoveInterfaceElement();
    } else {
        m_Row++;
    }
}

//------------------------------------------------------------------------------

CInfoResizeTerminalPanel::CInfoResizeTerminalPanel(WINDOW* w) : CInfoPanel(w) {

}

CInfoResizeTerminalPanel::~CInfoResizeTerminalPanel() {

}

void CInfoResizeTerminalPanel::Show() {
    // Once the size is correct, display Main menu
    if (COLS == 80 && LINES == 24) {
        CGameBase::get().ReplaceInterfaceElements(new CMenuMain(m_Window));
        return;
    }

    int width, height;
    getmaxyx(m_Window, height, width);
    werase(m_Window);

    std::string title = CGameBase::get().TextUI["PANEL_RESIZE_TITLE"];
    std::string info = CGameBase::get().TextUI["PANEL_RESIZE_INFO"];
    std::string size(CGameBase::get().TextUI["PANEL_RESIZE_CURRENTSIZE"] + std::to_string(COLS) + " x " + std::to_string(LINES));

    wmove(m_Window, height / 2 - 2, width / 2 - title.length() / 2);
    wprintw(m_Window, title.c_str());
    wmove(m_Window, height / 2 - 1, width / 2 - info.length() / 2);
    wprintw(m_Window, info.c_str());
    wmove(m_Window, height / 2 + 1, width / 2 - size.length() / 2);
    wprintw(m_Window, size.c_str());

    wrefresh(m_Window);
}
