#ifndef GAMESCREEN_HPP
#define GAMESCREEN_HPP

#include <string>
#include <vector>
#include <memory>
class CGameEngine;

#include "interfaceElement.hpp"
#include "worldMap.hpp"
#include "building.hpp"
#include "dailyAction.hpp"

/**
 * Main game engine displaying World map, various Building menus, Story messages and other elements.
 */
class CGameEngine : public CInterfaceElement {
public:
    /**
     * Constructor.
     * @param w Window which will contain the game.
     */
    CGameEngine(WINDOW * w);
    ~CGameEngine();

    void Show() override;
    void RegisterKeys() override;

    /**
     * Loads a saved game instance from a given string.
     * @param src Source string. If none is supplied, new game will be created.
     * @return True if loaded successfully, false otherwise.
     */
    bool Deserialize(std::vector<std::string> src);
    /**
     * Serializes current game, converting it to a string. Includes everything needed for loading.
     * @return Serialized saved game.
     */
    std::vector<std::string> Serialize() const;
    /**
     * Displays tile info.
     * @param tile Tile to be displayed
     */
    void ShowTileInfo(CWorldTile * tile) const;
    /**
     * Shows info about a building that is currently being built.
     * @param info The info string to be displayed.
     */
    void ShowBlueprintInfo(std::string info) const;
    /**
     * Displays tutorial.
     */
    void ShowTutorial() const;
    /**
     * Gets available buildings for the current day regardless of resources.
     * @return Buildings that can be built this day.
     */
    std::vector<std::shared_ptr<CBuilding>> GetAvailableBuildings() const;
    /**
     * Switches world map into building mode.
     * @param building New building that a player wants to build.
     */
    void PrepareBuild(std::shared_ptr<CBuilding> building) const;
    /**
     * Adds the new building to the list of built buildings.
     * @param building New building.
     */
    void Build(std::shared_ptr<CBuilding> building);
    /**
     * New turn.
     */
    void NextDay();
    /**
     * Gets current metal balance.
     * @return Current metal amount.
     */
    unsigned int GetMetal() const;
    /**
     * Gets current concrete balance.
     * @return Current concrete amount.
     */
    unsigned int GetConcrete() const;
    /**
     * Gets power earnings unaffected by disasters.
     * @return Amount of generated power.
     */
    unsigned int GetPowerEarnings() const;
    /**
     * Computes metal and concrete income based on available power.
     * @param availablePower Power available for production buildings.
     * @param totalPowerBalance Calculated as Available power - Total power consumption
     */
    void ComputeConcreteMetalIncome(unsigned int & availablePower, short & totalPowerBalance);
    /**
     * Computes power, metal, and concrete income. Optionally takes current disaster into account.
     * @param includeDailyAction Whether to account for the current disaster.
     */
    void GetExpectedResourceIncome(bool includeDailyAction = true);
    
    /*! A building currently placed under the cursor. */
    std::shared_ptr<CBuilding> selectedBuilding;
    
    /*! Whether a tutorial should be shown next time Show() is called. */
    bool showTutorial;

protected:
    /**
     * Completes daily action.
     * @param action Action to be done.
     * @return Messages generated while completing the action.
     */
    std::vector<std::string> DoDailyAction(std::shared_ptr<CWorldAction> action);
    /**
     * Displays resources bar.
     */
    void ShowResourcesBar() const;
    /**
     * Sets the amount of employees in a building.
     * @param building Building to be changed.
     * @param amount New amount of employees.
     */
    void SetEmployees(std::shared_ptr<CBuilding> building, unsigned short amount);
    /**
     * Adds new people to the colony.
     * @param amount Amount of new people.
     */
    void AddPeople(unsigned short amount);
    /**
     * Removes people from the colony.
     * @param amount Amount of removed people.
     */
    void RemovePeople(unsigned short amount);
    /**
     * Prints one resource type.
     * @param name Resource name 
     * @param current Current amount of resource.
     * @param next Resource income per day.
     * @param color Color of the resource text.
     */
    void PrintResource(const std::string & name, unsigned int current, int next, const CColor & color) const;
    /**
     * Ends the game.
     * @param victory True if game was won, false if not.
     */
    void FinishGame(bool victory) const;

    /*! World map. */
    CWorldMap * m_WorldMap;

    /*! Height of the map window. */
    size_t m_MapHeight;
    /*! Height of the bottom menu window. */
    size_t m_MenuHeight;

    /*! Map window. */
    WINDOW * m_MapWin;
    /*! Bottom menu window. */
    WINDOW * m_BottomWin;

    /*! Current day. */
    unsigned short g_Day;
    /*! Number of people in the colony. */
    unsigned short g_People;
    /*! Max capacity of cubicles. */
    unsigned short g_HabitationCapacity;
    /*! Amount of unemployed people. */
    unsigned short g_Unemployed;
    /*! Current metal balance. */
    unsigned short g_Metal;
    /*! Current power balance. */
    unsigned short g_Power;
    /*! Current concrete balance. */
    unsigned short g_Concrete;
    /*! Difference between Amount of generated power and Amount of used power */
    short g_NetPowerEarnings;
    /*! Daily metal income. */
    unsigned short g_MetalIncome;
    /*! Daily concrete income. */
    unsigned short g_ConcreteIncome;
    /*! Days left until help arrives. */
    unsigned short g_DaysToRescue;
    /*! Whether a radio tower is built. */
    bool g_IsRadioTowerBuilt;
    /*! Whether there was enough power for the radio to transmit signals. */
    bool g_RadioTransmitted;

    /*! Contains all habitable buildings. */
    std::vector<std::shared_ptr<CBuilding>> g_Habitation;
    /*! Contains all non-habitable buildings. */
    std::vector<std::shared_ptr<CBuilding>> g_Buildings;
    /*! Contains templates for all buildings. */
    std::vector<std::shared_ptr<CBuilding>> g_BuildingTemplates;


};

#endif /* GAMESCREEN_HPP */

