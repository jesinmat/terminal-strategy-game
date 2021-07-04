/*! \mainpage Main Page
 *
 * \section intro_sec Info
 *
 * This game was made by Matyáš Ješina in 2018 as a semestral work for PA2 at CTU FIT.
 *
 * \section install_sec Assignment in Czech:
 *
 * \subsection name Budovatelská strategie
 * 
 * \subsection engine Váš engine:
 * 
 *  - ze souboru nahraje definici světa (mapu, vzhled objektů na mapě, zdroje, lokace protihráčů, ...) <br />
 *  - ze souboru nahraje upravitelné parametry možných budov (vstupy, výstupy, cena stavby, ...) <br />
 *  - ze souboru nahraje definici akcí (katastrofy, úkoly globální změny, ...) <br />
 *  - umožní simulovat běh světa - přidávání budouv, těžba, ... (uživatel/AI) <br />
 *  - implementuje jednoduchý hodnotící systém (počet lidí, zdrojů, úkoly, ...) <br />
 *  - umožňuje ukládat a načítat rozehrané hry
 * 
 * Engine může fungovat jako real-time, či tahová hra. Ve znakové podobě, či graficky.
 * 
 * \subsection poly Kde lze využít polymorfismus? (doporučené)
 * 
 * Parametry budov: cena, vstupní zdroje, výstupní zdroje, ... <br />
 * Efekty prostředí: vichřice, sucha, hladomor, velká úroda, ... <br />
 * Uživatelské rozhraní: konzole, ncurses, SDL, OpenGL (různé varianty), ... <br />
 * Může fungovat jako kampaň pro jednoho hráče, hráč proti počítači nebo hráč proti hráči. <br />
 * 
 * \subsection examples Ukázky:
 * 
 *  - https://en.wikipedia.org/wiki/The_Settlers_II <br />
 *  - https://is.fit.cvut.cz/group/intranet/zp/list#u_laubedan <br />
 *  - https://en.wikipedia.org/wiki/Lords_of_the_Realm <br />
 *  - http://www.mobygames.com/game/lords-of-the-realm/screenshots <br />
 *  - https://en.wikipedia.org/wiki/SimCity <br />
 */

#include "gameBase.hpp"

using namespace std;


/*
 * 
 */
int main(void) {
    CGameBase & g = CGameBase::get();
    g.Start();
    return 0;
}
