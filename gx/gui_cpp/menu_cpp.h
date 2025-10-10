/****************************************************************************
 * Genesis Plus GX - C++ Menu System
 *
 * menu_cpp.h - C++ Menu Interface
 *
 * Simple menu implementation using C++ GUI classes
 ***************************************************************************/

#ifndef _MENU_CPP_H_
#define _MENU_CPP_H_

/* Menu return values */
enum
{
    MENU_NONE = 0,
    MENU_EXIT,
    MENU_LOAD_GAME,
    MENU_SETTINGS,
    MENU_RESET,
    MENU_QUIT
};

/* Main C++ menu entry point */
int ShowMainMenu();

/* Individual menu functions */
int MenuMain();
int MenuLoadGame();
int MenuSettings();

#endif /* _MENU_CPP_H_ */
