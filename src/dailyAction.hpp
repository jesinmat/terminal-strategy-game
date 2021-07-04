#ifndef DAILYACTION_HPP
#define DAILYACTION_HPP

#include <string>

/**
 * Class for storing info about daily actions.
 */
class CWorldAction {
public:
    /**
     * Constructor.
     * @param people Number of people retrieved from the ship
     * @param metal Metal found in the ship
     * @param powerDrop Power decrease during disasters
     * @param destructionChance Chance of each building being destroyed by disaster
     * @param name Name of the disaster
     */
    CWorldAction(unsigned short people = 0, unsigned short metal = 0, unsigned short powerDrop = 0, unsigned short destructionChance = 0, std::string name = "");
    /*! Name of the disaster */
    std::string name;
    /*! Amount of metal found in the ship */
    unsigned short metal;
    /*! Number of people retrieved from the ship */
    unsigned short people;
    /*! Power decrease during disasters */
    unsigned short powerDrop;
    /*! Chance of each building being destroyed by disaster */
    unsigned short destructionChance;

};

#endif /* DAILYACTION_HPP */

