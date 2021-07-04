#ifndef MENU_HPP
#define MENU_HPP

#include <ncurses.h>
#include <vector>
#include <functional>
#include "interfaceElement.hpp"
#include "gameEngine.hpp"

/**
 * Base class for various menus.
 */
class CMenu : public CInterfaceElement {
public:
    /**
     * Constructor.
     * @param w Window which will contain the menu
     */
    CMenu(WINDOW * w = stdscr);
    virtual ~CMenu();
    
    /**
     * Called when pressing up arrow.
     */
    virtual void onUp();
    /**
     * Called when pressing down arrow.
     */
    virtual void onDown();
    /**
     * Called when pressing left arrow.
     */
    virtual void onLeft();
    /**
     * Called when pressing right arrow.
     */
    virtual void onRight();
    /**
     * Called when pressing Enter.
     */
    virtual void onEnter() = 0;
protected:
    /*! Selected option index. */
    unsigned int m_Selected;
    /*! A list of available options. */
    std::vector<std::string> m_Items;
    /*! Menu title. */
    std::string m_Title;
};

/**
 * Main menu of the game.
 */
class CMenuMain : public CMenu {
public:
    /**
     * Constructor.
     * @param w Window which will contain the menu.
     */
    CMenuMain(WINDOW * w = stdscr);
    void Show() override;
    void RegisterKeys() override;
protected:
    void onEnter() override;
    
};

/**
 * Menu for loading previously saved games.
 */
class CMenuLoadGame : public CMenu {
public:
    /**
     * Constructor.
     * @param w Window which will contain the menu.
     */
    CMenuLoadGame(WINDOW * w = stdscr);
    ~CMenuLoadGame();
    void Show() override;
    void RegisterKeys() override;
protected:
    void onDown() override;
    void onEnter() override;
    /**
     * Called when user returns from Load game menu back to Main menu.
     */
    void onReturn();
    
    /*! Width of the Saved Games bounding box. */
    size_t m_SavedWinWidth = 40;
    /*! Height of the Saved Games bounding box. */
    size_t m_SavedWinHeight = 10;
    /*! Saved Games window with a bounding box. */
    WINDOW * m_SavedWin;
    
    /*! Short info text describing this menu. */
    std::string m_Info;
    
};

/**
 * In-game "Build structure" menu.
 */
class CMenuBuild : public CMenu {
public:
    /**
     * Constructor.
     * @param w Window which will contain the menu.
     * @param game Game container.
     */
    CMenuBuild(WINDOW * w = stdscr, CGameEngine * game = NULL);
    ~CMenuBuild();
    void Show() override;
    void RegisterKeys() override;
protected:   
    void onUp() override;
    void onDown() override;
    void onLeft() override;
    void onRight() override;
    void onEnter() override;
    
    /*! Active CGameEngine instance. */
    CGameEngine * m_GameEngine;
    /*! Buildings that can be built at this point in the game. */
    std::vector<std::shared_ptr < CBuilding>> m_Available;
};

/**
 * Pause game menu.
 */
class CMenuPause : public CMenu {
public:
    /**
     * Constructor.
     * @param w Window which will contain the menu.
     */
    CMenuPause(WINDOW * w = stdscr);
    ~CMenuPause();
    void Show() override;
    void RegisterKeys() override;
protected:
    void onEnter() override;
};

#endif /* MENU_HPP */


