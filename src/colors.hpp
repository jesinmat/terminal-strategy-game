#ifndef COLORS_HPP
#define COLORS_HPP

/**
 * Color enums for color pairs used in the game.
 */
enum CColor {
    MENU_TITLE = 1,
    MENU_ITEM,
    MENU_ITEM_SELECTED,
    RESOURCE_INCOME_POSITIVE,
    RESOURCE_INCOME_NEGATIVE,
    WORLDTILE_DUST,
    WORLDTILE_CONCRETE,
    WORLDTILE_SHIP,
    SYMBOL_POWER,
    SYMBOL_CONCRETE,
    SYMBOL_METAL,
    BUILDING,
    BUILDING_BLUEPRINT,
    WARNING
};

/**
 * Creates color pairs.
 */

void InitColors();


#endif /* COLORS_HPP */
