#include <vector>
#include <cmath>
#include <list>
#include <stdexcept>

#include "worldMap.hpp"
#include "controls.hpp"
#include "gameBase.hpp"

#define ENTER_KEY '\n'

CWorldMap::CWorldMap(WINDOW * w, CGameEngine * g) : CInterfaceElement(w) {
    getmaxyx(m_Window, m_Height, m_Width);
    m_CursorX = m_Width / 2;
    m_CursorY = m_Height / 2;
    m_gameScr = g;

    // Initialized here because ncurses has problems otherwise
    CWorldTile::CWorldTileData = {
        {CWorldTileType::DUST, std::make_pair(CColor::WORLDTILE_DUST, ' ')},
        {CWorldTileType::REGOLITH, std::make_pair(CColor::WORLDTILE_CONCRETE, ACS_CKBOARD)},
        {CWorldTileType::SHIP, std::make_pair(CColor::WORLDTILE_SHIP, ACS_CKBOARD)}
    };
}

CWorldMap::~CWorldMap() {
    for (unsigned int y = 0; y < m_Height; y++) {
        for (unsigned int x = 0; x < m_Width; x++) {
            delete m_Tiles[y][x];
        }
        delete [] m_Tiles[y];
    }
    delete [] m_Tiles;
}

std::vector<std::string> CWorldMap::Serialize() const {
    std::vector<std::string> res;
    // Dust tiles are not serialized. Other have X, Y and Type
    for (unsigned int y = 0; y < m_Height; y++) {
        for (unsigned int x = 0; x < m_Width; x++) {
            if (m_Tiles[y][x]->type != CWorldTileType::DUST) {
                res.push_back(std::to_string(x));
                res.push_back(std::to_string(y));
                res.push_back(std::to_string((int) (m_Tiles[y][x]->type)));
            }
        }
    }
    return res;
}

bool CWorldMap::Deserialize(std::vector<std::string> src) {
    // New map
    if (src.size() == 0) {
        m_Tiles = new CWorldTile**[m_Height];
        // Create map with Ship parts
        for (unsigned int y = 0; y < m_Height; y++) {
            m_Tiles[y] = new CWorldTile * [m_Width];
            for (unsigned int x = 0; x < m_Width; x++) {
                if ((x - 70)*(x - 70) / 3 + (y - 10) * (y - 10) < 18 && (x * y) % (x + y) < 70) {
                    CWorldTile * ship = new CWorldTile(CWorldTileType::SHIP);
                    m_Tiles[y][x] = ship;
                    m_ShipTiles.push_back(ship);
                } else {
                    m_Tiles[y][x] = new CWorldTile();
                }
            }
        }

        // Locations of regolith
        std::list<std::pair<int, int>> regolith = {
            std::make_pair(4, 5),
            std::make_pair(3, 6),
            std::make_pair(14, 13),
            std::make_pair(15, 13),
            std::make_pair(25, 11),
            std::make_pair(35, 2),
            std::make_pair(36, 3),
            std::make_pair(50, 8),
            std::make_pair(50, 7)
        };

        // Replace Dust with Regolith
        for (auto & pair : regolith) {
            m_Tiles[pair.second][pair.first]->type = CWorldTileType::REGOLITH;
            m_Tiles[pair.second + 1][pair.first]->type = CWorldTileType::REGOLITH;
            if (pair.second % 3 == 0) {
                m_Tiles[pair.second][pair.first + 1]->type = CWorldTileType::REGOLITH;
            }
            m_Tiles[pair.second + 1][pair.first + 1]->type = CWorldTileType::REGOLITH;
        }

        return true;
    }

    // Load map from file

    m_Tiles = new CWorldTile**[m_Height];
    for (unsigned int y = 0; y < m_Height; y++) {
        m_Tiles[y] = new CWorldTile * [m_Width];
        for (unsigned int x = 0; x < m_Width; x++) {
            m_Tiles[y][x] = new CWorldTile();
        }
    }

    try {
        for (unsigned int i = 0; i < src.size(); i += 3) {
            unsigned short posX = std::stoi(src[i], NULL, 10);
            unsigned short posY = std::stoi(src[i + 1], NULL, 10);
            unsigned short type = std::stoi(src[i + 2], NULL, 10);
            if (posX < m_Width && posY < m_Height && type < 3) {
                m_Tiles[posY][posX]->type = (CWorldTileType) type;
                if ((CWorldTileType) type == CWorldTileType::SHIP) {
                    m_ShipTiles.push_back(m_Tiles[posY][posX]);
                }
            } else {
                return false;
            }
        }
    } catch (...) {
        return false;
    }

    return true;
}

void CWorldMap::BlueprintMode(std::shared_ptr<CBuilding> building) {
    m_BuildingBlueprint = building;
    m_BuildingMode = true;
    // Move cursor if it was in the corner
    if (m_CursorX + building.get()->width >= m_Width) {
        m_CursorX = m_Width - building.get()->width;
    }
    if (m_CursorY + building.get()->height >= m_Height) {
        m_CursorY = m_Height - building.get()->height;
    }
}

CWorldTile & CWorldMap::GetTile(size_t x, size_t y) const {
    if (x < m_Width && y < m_Height) {
        return *m_Tiles[y][x];
    }

    throw std::invalid_argument("Tile index out of range");
    return *m_Tiles[0][0];
}

void CWorldMap::Show() {
    // Print all tiles
    for (unsigned int y = 0; y < m_Height; y++) {
        for (unsigned int x = 0; x < m_Width; x++) {
            wmove(m_Window, y, x);
            wattron(m_Window, COLOR_PAIR(m_Tiles[y][x]->GetColorPair()));
            if (m_Tiles[y][x]->building)
                wattron(m_Window, COLOR_PAIR(CColor::BUILDING));
            waddch(m_Window, (m_Tiles[y][x]->GetForegroundCharacter()));
        }
    }

    // Print Building blueprint or cursor
    if (m_BuildingMode) {
        wattron(m_Window, COLOR_PAIR(CColor::BUILDING_BLUEPRINT));
        for (unsigned int y = m_CursorY; y < m_CursorY + m_BuildingBlueprint.get()->height; y++) {
            for (unsigned int x = m_CursorX; x < m_CursorX + m_BuildingBlueprint.get()->width; x++) {
                wmove(m_Window, y, x);
                waddch(m_Window, m_BuildingBlueprint->symbol);
            }
        }
        std::string res;
        if (m_BuildingBlueprint.get()->CanBeBuiltAt(m_CursorX, m_CursorY, this)) {
            res = m_BuildingBlueprint.get()->name + CGameBase::get().TextUI["GENERAL_CAN_BUILD"];
        } else {
            res = m_BuildingBlueprint.get()->name + CGameBase::get().TextUI["GENERAL_CAN_NOT_BUILD"];
        }

        m_gameScr->ShowBlueprintInfo(res);
    } else {
        wattron(m_Window, COLOR_PAIR(CColor::MENU_ITEM));
        wmove(m_Window, m_CursorY, m_CursorX);
        waddch(m_Window, '+');
        m_gameScr->ShowTileInfo(m_Tiles[m_CursorY][m_CursorX]);
    }

    wrefresh(m_Window);

}

void CWorldMap::RegisterKeys() {
    CControls & c = CControls::getInstance();
    c.ClearAssignedKeys();
    c.AssignKey(KEY_UP, [this]() {
        this->MoveCursor(0, -1);
    });
    c.AssignKey(KEY_DOWN, [this]() {
        this->MoveCursor(0, 1);
    });
    c.AssignKey(KEY_LEFT, [this]() {
        this->MoveCursor(-1, 0);
    });
    c.AssignKey(KEY_RIGHT, [this]() {
        this->MoveCursor(1, 0);
    });
    c.AssignKey(ENTER_KEY, [this]() {
        this->Build();
    });
    c.AssignKey('x', [this]() {
        m_BuildingMode = false;
    });

}

void CWorldMap::MoveCursor(short int x, short int y) {
    if (m_CursorX + x < m_Width)
        if (!m_BuildingMode || m_CursorX + x + m_BuildingBlueprint.get()->width <= m_Width) {
            m_CursorX += x;
        }
    if (m_CursorY + y < m_Height) {
        if (!m_BuildingMode || m_CursorY + y + m_BuildingBlueprint.get()->height <= m_Height) {
            m_CursorY += y;
        }
    }

    m_gameScr->selectedBuilding = m_Tiles[m_CursorY][m_CursorX]->building;
}

void CWorldMap::Build(std::shared_ptr<CBuilding> building, size_t posX, size_t posY) {
    // Build and set tiles as occupied
    building.get()->posX = posX;
    building.get()->posY = posY;
    m_gameScr->Build(building);

    for (unsigned int yy = posY; yy < posY + building.get()->height; yy++) {
        for (unsigned int xx = posX; xx < posX + building.get()->width; xx++) {
            m_Tiles[yy][xx]->building = building;
        }
    }
}

void CWorldMap::Build() {
    // Check if building mode and then build
    if (!m_BuildingMode)
        return;

    if (!m_BuildingBlueprint.get()->CanBeBuiltAt(m_CursorX, m_CursorY, this))
        return;

    Build(m_BuildingBlueprint, m_CursorX, m_CursorY);
    m_gameScr->selectedBuilding = m_BuildingBlueprint;
    m_BuildingMode = false;
}

void CWorldMap::RemoveBuilding(std::shared_ptr<CBuilding> building) {
    // Remove from tiles
    for (unsigned int y = building.get()->posY; y < building.get()->posY + building.get()->height; y++) {
        for (unsigned int x = building.get()->posX; x < building.get()->posX + building.get()->width; x++) {
            m_Tiles[y][x]->building.reset();
        }
    }
}

void CWorldMap::RemoveShipTiles(short int amount) {
    while (amount > 0 && m_ShipTiles.size() > 0) {
        m_ShipTiles.back()->type = CWorldTileType::DUST;
        m_ShipTiles.pop_back();
        amount--;
    }
}

//------------------------------------------------------------------------------

std::map<CWorldTileType, std::pair<CColor, int>> CWorldTile::CWorldTileData = {};

CWorldTile::CWorldTile(CWorldTileType t) : type(t) {
}

CColor CWorldTile::GetColorPair() const {
    return CWorldTile::CWorldTileData[type].first;
}

int CWorldTile::GetForegroundCharacter() const {
    if (building) {
        return building.get()->symbol;
    }
    return CWorldTile::CWorldTileData[type].second;
}
