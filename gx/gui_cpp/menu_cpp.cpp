/****************************************************************************
 * Genesis Plus GX - C++ Menu System
 *
 * menu_cpp.cpp - C++ Menu Implementation
 *
 * Simple functional menu using C++ GUI classes
 ***************************************************************************/

#include "gui.h"
#include "menu_cpp.h"

extern "C" {
#include "../gui/font.h"
#include <ogc/video.h>
}

/* External from gx_video.c */
extern "C" {
extern GXRModeObj *vmode;
extern void gxClearScreen(GXColor color);
extern void gxSetScreen();
}

/* Dummy sound data - will use real PCM data later */
static const u8 dummy_sound[] = {0};

/**
 * ShowMainMenu
 * 
 * Main entry point for C++ menu system
 * Declared as extern "C" for C linkage (defined in header)
 */
int ShowMainMenu()
{
    return MenuMain();
}

/**
 * MenuMain
 * 
 * Main menu implementation
 */
int MenuMain()
{
    int menu = MENU_NONE;
    
    /* Create main window */
    GuiWindow mainWindow(640, 480);
    mainWindow.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    
    /* Title text */
    GuiText titleText("Genesis Plus GX", 28, (GXColor){255, 255, 255, 255});
    titleText.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    titleText.SetPosition(0, 50);
    
    /* Subtitle */
    GuiText subtitleText("C++ GUI System", 18, (GXColor){200, 200, 200, 255});
    subtitleText.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
    subtitleText.SetPosition(0, 90);
    
    /* Create trigger for A button */
    GuiTrigger trigA;
#ifdef HW_RVL
    trigA.SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
#else
    trigA.SetSimpleTrigger(-1, 0, PAD_BUTTON_A);
#endif
    
    /* Create sound effects */
    GuiSound btnSoundOver(dummy_sound, sizeof(dummy_sound), SOUND_PCM);
    GuiSound btnSoundClick(dummy_sound, sizeof(dummy_sound), SOUND_PCM);
    
    /* Load Game button */
    GuiText loadGameText("Load Game", 20, (GXColor){255, 255, 255, 255});
    GuiButton loadGameBtn(200, 48);
    loadGameBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    loadGameBtn.SetPosition(0, -60);
    loadGameBtn.SetLabel(&loadGameText);
    loadGameBtn.SetTrigger(&trigA);
    loadGameBtn.SetSoundOver(&btnSoundOver);
    loadGameBtn.SetSoundClick(&btnSoundClick);
    
    /* Settings button */
    GuiText settingsText("Settings", 20, (GXColor){255, 255, 255, 255});
    GuiButton settingsBtn(200, 48);
    settingsBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    settingsBtn.SetPosition(0, 0);
    settingsBtn.SetLabel(&settingsText);
    settingsBtn.SetTrigger(&trigA);
    settingsBtn.SetSoundOver(&btnSoundOver);
    settingsBtn.SetSoundClick(&btnSoundClick);
    
    /* Quit button */
    GuiText quitText("Exit", 20, (GXColor){255, 255, 255, 255});
    GuiButton quitBtn(200, 48);
    quitBtn.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
    quitBtn.SetPosition(0, 60);
    quitBtn.SetLabel(&quitText);
    quitBtn.SetTrigger(&trigA);
    quitBtn.SetSoundOver(&btnSoundOver);
    quitBtn.SetSoundClick(&btnSoundClick);
    
    /* Add elements to window */
    mainWindow.Append(&titleText);
    mainWindow.Append(&subtitleText);
    mainWindow.Append(&loadGameBtn);
    mainWindow.Append(&settingsBtn);
    mainWindow.Append(&quitBtn);
    
    /* Set default focus */
    mainWindow.SetFocus(1);
    loadGameBtn.SetState(STATE_SELECTED);
    
    /* Main menu loop */
    while (menu == MENU_NONE)
    {
        /* Wait for VSync */
        VIDEO_WaitVSync();
        
        /* Update GUI */
        mainWindow.Update(&trigA);
        
        /* Check button states */
        if (loadGameBtn.GetState() == STATE_CLICKED)
        {
            loadGameBtn.ResetState();
            // TODO: Call MenuLoadGame() when implemented
            // For now, just stay in menu
        }
        else if (settingsBtn.GetState() == STATE_CLICKED)
        {
            settingsBtn.ResetState();
            // TODO: Call MenuSettings() when implemented
            // For now, just stay in menu
        }
        else if (quitBtn.GetState() == STATE_CLICKED)
        {
            quitBtn.ResetState();
            menu = MENU_EXIT;
        }
        
        /* Clear screen */
        gxClearScreen((GXColor){0, 0, 0, 255});
        
        /* Draw GUI */
        mainWindow.Draw();
        
        /* Render frame */
        gxSetScreen();
    }
    
    return menu;
}

/**
 * MenuLoadGame
 * 
 * Load game menu (placeholder for now)
 */
int MenuLoadGame()
{
    /* TODO: Implement file browser */
    /* For now, return to main menu */
    return MENU_EXIT;
}

/**
 * MenuSettings
 * 
 * Settings menu (placeholder for now)
 */
int MenuSettings()
{
    /* TODO: Implement settings browser */
    /* For now, return to main menu */
    return MENU_EXIT;
}
