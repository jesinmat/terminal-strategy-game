#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <string>
#include <stddef.h>
#include <ncurses.h>
class CBuilding;
#include "worldMap.hpp"

/**
 * Base class for all buildings.
 */
class CBuilding {
public:
    /**
     * Creates a new building with given parameters.
     * @param name Name
     * @param desc Description
     * @param width Width
     * @param height Height
     * @param capacity Highest number of people that can work inside
     * @param powerConsumption Power consumption per day
     * @param powerIncome Power income per day
     * @param concreteIncome Concrete consumption per day
     * @param metalIncome Metal consumption per day
     * @param concreteCost Amount of concrete required to build it
     * @param metalCost Amount of metal required to build it
     * @param symbol Symbol for displaying this on world map
     * @param habitable Whether this building is designed to provide habitation
     */
    CBuilding(std::string name, std::string desc, 
            unsigned short width, unsigned short height, 
            unsigned short capacity, unsigned short powerConsumption, 
            unsigned short powerIncome, unsigned short concreteIncome, 
            unsigned short metalIncome, unsigned short concreteCost, 
            unsigned short metalCost, char symbol, bool habitable);
    ~CBuilding();
    /**
     * Determines how much Concrete this building generates per day.
     * @return Amount of Concrete generated.
     */
    unsigned int GetConcreteEarnings() const;
    /**
     * Determines how much Metal this building generates per day.
     * @return Amount of Metal generated.
     */
    unsigned int GetMetalEarnings() const;
    /**
     * Determines how much Electricity this building generates per day.
     * @return Amount of Electricity generated.
     */
    unsigned int GetPowerEarnings() const;
    /**
     * Determines how much Power this building needs per day.
     * @return Amount of Power needed.
     */
    unsigned int GetPowerConsumption() const;
    /**
     * Determines if this building can be built at given coordinates.
     * @param x X coordinate of Top Left corner
     * @param y Y coordinate of Top Left corner
     * @param map World map containing this building.
     * @return True if it's possible to build it there, false otherwise.
     */
    bool CanBeBuiltAt(const unsigned short x, const unsigned short y, CWorldMap * map) const;
    
    /**
     * Prints building info including resource production and consumption
     * @param win
     * @param buildingMenu
     */
    void Print(WINDOW * win, bool buildingMenu = false) const;

    /*! Name of the building */
    const std::string name;
    /*! Description of the building */
    const std::string description;
    /*! Width of the building */
    const unsigned short width;
    /*! Height of the building */
    const unsigned short height;
    /*! Top-left X position */
    unsigned short posX;
    /*! Top-left Y position */
    unsigned short posY;

    /*! Amount of Concrete required to build this building */
    const unsigned short priceConcrete;
    /*! Amount of Metal required to build this building */
    const unsigned short priceMetal;
    /*! How many workers/inhabitants can be put inside */
    const unsigned short capacity;   
    /*! Current number of workers inside */
    unsigned short workers = 0;
    /*! Power consumption with max workers */
    const unsigned short powerConsumption;
    /*! Concrete production with max workers */
    const unsigned short maxConcrete;
    /*! Metal production with max workers */
    const unsigned short maxMetal;
    /*! Power production with max workers */
    const unsigned short maxPower;
    /*! Symbol for displaying this on world map */
    const char symbol;
    /*! Whether this building is designed to provide habitation */
    const bool habitable;
};

#endif /* BUILDING_HPP */

