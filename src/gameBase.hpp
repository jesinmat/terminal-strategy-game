#ifndef GAMEBASE_HPP
#define GAMEBASE_HPP

#include <vector>
#include <string>
#include "interfaceElement.hpp"
#include "fileio.hpp"
#include "dataStore.hpp"
#include "gameEngine.hpp"

/**
 * Base for handling user input and creating games. Represented as a singleton.
 * @return 
 */
class CGameBase {
public:
    /**
     * Getter for singleton instance
     * @return CGameBase instance
     */
    static CGameBase & get();
    /**
     * Starts ncurses and displays Main menu.
     */
    void Start();
    /**
     * Prepares the program to be ended.
     */
    void Quit();
    /**
     * Starts a new game.
     */
    void NewGame();
    /**
     * Loads a game from filename.
     * @param filename Name of the save file inside saved games folder.
     */
    void LoadGame(std::string filename);
    /**
     * Saves the current game with a timestamp.
     */
    void Save();
    /**
     * Gets file contents and returns them
     * \sa CFileIO::Load
     * @param filename Name of the file
     * @return File contents in lines
     */
    std::vector<std::string> Load(std::string filename) const;

    /**
     * Adds active UI element to the front, without deleting the previous one.
     * @param x The element to be added
     */
    void AddInterfaceElement(CInterfaceElement * x);
    /**
     * Deletes all UI elements and replaces them with given x.
     * @param x The element to be added
     */
    void ReplaceInterfaceElements(CInterfaceElement * x);
    /**
     * Removes most recent UI element. If no other element is present, ends the program.
     */
    void RemoveInterfaceElement();

    /**
     * Gets filenames of saved games using CFileIO.
     * \sa CFileIO::GetSavedGamesFilenames
     * @return Vector of filenames
     */
    std::vector<std::string> GetSavedGamesFilenames() const;

    /*! Texts for UI elements. */
    static CTextUI TextUI;
    /*! Texts for story. */
    static CTextStory TextStory;
    /*! Daily actions such as disasters and retrieving new people */
    static CDailyActionData DailyAction;
    /*! Active CGameEngine instance. */
    CGameEngine * m_GameEngine = NULL;

private:
    CGameBase();
    CGameBase(const CGameBase &) = delete;
    CGameBase(CGameBase &&) = delete;
    void operator=(const CGameBase &) = delete;

    /*! Control variable for drawing UI and listening for input. */
    bool shouldQuit = false;

    /*! Interface elements currently being active. */
    std::vector<CInterfaceElement *> m_Elements;
    /*! Old interface elements ready for deletion. */
    std::vector<CInterfaceElement *> m_OldElements;

    /*! Name of the last save file. */
    std::string m_SavefileName;
};


#endif /* GAMEBASE_HPP */

