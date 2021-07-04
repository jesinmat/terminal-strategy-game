#include <curses.h>
#include <unistd.h>
#include <iostream>
#include "controls.hpp"

CControls & CControls::getInstance() {
    static CControls a;
    return a;
}

CControls::CControls() {
}

void CControls::AssignKey(int key, std::function<void() > func) {
    m_KeyMap[key] = func;
}

void CControls::UnassignKey(int key) {
    auto it = m_KeyMap.find(key);
    if (it != m_KeyMap.end())
        m_KeyMap.erase(it);
}

void CControls::ClearAssignedKeys() {
    m_KeyMap.clear();
}

void CControls::ReadInput() const{
    noecho();
    int keyCode;
    nodelay(stdscr, TRUE);
    while ((keyCode = getch()) > 0) {
        if (m_KeyMap.find(keyCode) != m_KeyMap.end()) {
            (*(m_KeyMap.find(keyCode))).second();
        }
    }
}