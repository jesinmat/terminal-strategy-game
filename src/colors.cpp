#include "colors.hpp"
#include <curses.h>

void InitColors(){
    // Foreground, background
    init_pair(CColor::MENU_TITLE, COLOR_BLUE, COLOR_BLACK);
    init_pair(CColor::MENU_ITEM, COLOR_WHITE, COLOR_BLACK);
    init_pair(CColor::MENU_ITEM_SELECTED, COLOR_BLACK, COLOR_WHITE);
    init_pair(CColor::RESOURCE_INCOME_POSITIVE, COLOR_GREEN, COLOR_BLACK);
    init_pair(CColor::RESOURCE_INCOME_NEGATIVE, COLOR_RED, COLOR_BLACK);
    init_pair(CColor::WORLDTILE_DUST, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CColor::WORLDTILE_CONCRETE, COLOR_BLACK, COLOR_WHITE);
    init_pair(CColor::WORLDTILE_SHIP, COLOR_BLACK, COLOR_BLUE);
    init_pair(CColor::SYMBOL_CONCRETE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CColor::SYMBOL_POWER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CColor::SYMBOL_METAL, COLOR_BLUE, COLOR_BLACK);
    init_pair(CColor::BUILDING, COLOR_WHITE, COLOR_GREEN);
    init_pair(CColor::BUILDING_BLUEPRINT, COLOR_BLUE, COLOR_WHITE);
    init_pair(CColor::WARNING, COLOR_RED, COLOR_BLACK);
}