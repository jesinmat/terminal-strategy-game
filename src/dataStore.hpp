#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <map>
#include <string>
#include <memory>

#include "dailyAction.hpp"

/**
 * Container for all UI texts displayed in the game.
 */
class CTextUI {
public:
    CTextUI();
    /**
     * Operator for easier access to data container.
     * @param key Key for the map.
     * @return Corresponding value from map.
     */
    std::string operator[](const std::string & key);
protected:
    /*! Map for holding data. */
    std::map<std::string, std::string> m_Data;
};

/**
 * Container for all story and gameplay related texts displayed in the game.
 */
class CTextStory {
public:
    CTextStory();
    /**
     * Operator for easier access to data container.
     * @param key Key for the map.
     * @return Corresponding value from map.
     */
    std::string operator[](const std::string & key);
protected:
    /*! Map for holding data. */
    std::map<std::string, std::string> m_Data;
};

/**
 * Container of all World Actions.
 */
class CDailyActionData {
public:
    CDailyActionData();
    /**
     * Operator for easier access to data container.
     * @param day Requested day.
     * @return Daily World Action for the given day.
     */
    std::shared_ptr<CWorldAction> operator[](unsigned short day);
protected:
    /*! Map for holding data. */
    std::map<unsigned short, std::shared_ptr<CWorldAction>> m_Data;
};

#endif /* DATASTORE_HPP */

