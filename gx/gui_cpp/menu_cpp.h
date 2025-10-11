/****************************************************************************
 * Genesis Plus GX - C++ Menu System
 *
 * menu_cpp.h - C++ Menu Interface
 *
 * Simple menu implementation using C++ GUI classes
 ***************************************************************************/

#ifndef _MENU_CPP_H_
#define _MENU_CPP_H_

#ifdef __cplusplus
extern "C" {
#endif

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

/* Main C++ menu entry point - callable from C */
int ShowMainMenu(void);

#ifdef __cplusplus
}

/* Individual menu functions (C++ only) */
int MenuMain(void);
int MenuLoadGame(void);
int MenuSettings(void);

#endif

#endif /* _MENU_CPP_H_ */
