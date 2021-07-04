#ifndef WORLDMAP_HPP
#define WORLDMAP_HPP
#include <string>
#include <map>
class CWorldTile;
class CWorldMap;
#include "colors.hpp"
#include "interfaceElement.hpp"
#include "building.hpp"
#include "gameEngine.hpp"

/**
 * World map. Grid of MapTiles where the game takes place.
 */
class CWorldMap : public CInterfaceElement {
public:
    /**
     * 
     * @param w Window which will contain the map.
     * @param g CGameEngine instance.
     */
    CWorldMap(WINDOW * w, CGameEngine * g);
    ~CWorldMap();

    /**
     * Loads a world map from a given string.
     * @param src Source string. If none is supplied, default world map will be created.
     * @return True if loaded successfully, false otherwise.
     */
    bool Deserialize(std::vector<std::string> src);
    /**
     * Serializes current world map, converting it to a string. Doesn't include buildings.
     * @return Serialized world map.
     */
    std::vector<std::string> Serialize() const;

    void Show() override;
    void RegisterKeys() override;
    /**
     * Placing mode for a new building.
     * @param building Building to be placed.
     */
    void BlueprintMode(std::shared_ptr<CBuilding> building);
    /**
     * Assigns a building to world tiles.
     * @param building New building.
     * @param posX Top-left X position of the building.
     * @param posY Top-left Y position of the building.
     */
    void Build(std::shared_ptr<CBuilding> building, size_t posX, size_t posY);
    /**
     * Removes a building from world tiles.
     * @param building Removed building.
     */
    void RemoveBuilding(std::shared_ptr<CBuilding> building);
    /**
     * Removes ship tiles from the map and replaces them with dust.
     * @param amount Amount of ship tiles to replace.
     */
    void RemoveShipTiles(short int amount);
    /**
     * Gets a tile with given coordinates.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @return Tile with the given position.
     */
    CWorldTile & GetTile(size_t x, size_t y) const;
private:
    /**
     * Moves cursor relative to current position.
     * @param x Relative X coordinate.
     * @param y Relative Y coordinate.
     */
    void MoveCursor(short int x, short int y);
    /**
     * Builds the blueprint building in the current position.
     */
    void Build();
    
    /*! World map tiles. */
    CWorldTile *** m_Tiles;
    /*! Ship tiles. */
    std::vector<CWorldTile *> m_ShipTiles;
    CGameEngine * m_gameScr;
    /*! Current cursor position (X). */
    size_t m_CursorX;
    /*! Current cursor position (Y). */
    size_t m_CursorY;
    
    /*! If true, building is being placed instead of the standard cursor. */
    bool m_BuildingMode = false;
    /*! A building that is being placed before building.  */
    std::shared_ptr<CBuilding> m_BuildingBlueprint;
};

/**
 * Type of world tile.
 */
enum class CWorldTileType {
    DUST, REGOLITH, SHIP
};

/**
 * Represents 1x1 world map tile.
 */
class CWorldTile {
public:
    /**
     * Constructor.
     * @param t Type of tile.
     */
    CWorldTile(CWorldTileType t = CWorldTileType::DUST);
    /**
     * Get color pair for the current CWorldTile instance.
     * @return Color pair.
     */
    CColor GetColorPair() const;
    /**
     * Gets foreground character to be displayed for this element.
     * @return Code of the character.
     */
    int GetForegroundCharacter() const;
    
    /*! Type of the tile. */
    CWorldTileType type;
    /*! Building present on the tile.  */
    std::shared_ptr<CBuilding> building;
    
    /*! Colors and symbol for each world tile type. */
    static std::map<CWorldTileType, std::pair<CColor, int>> CWorldTileData;
};

#endif /* WORLDMAP_HPP */

