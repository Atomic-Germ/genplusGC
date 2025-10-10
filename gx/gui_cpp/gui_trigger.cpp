/****************************************************************************
 * Genesis Plus GX - GUI System
 * gui_trigger.cpp
 *
 * GuiTrigger - Controller input handler
 ***************************************************************************/

#include "gui.h"

GuiTrigger::GuiTrigger()
{
    chan = -1;
    wiibtns = 0;
    gcbtns = 0;
    type = TRIGGER_SIMPLE;
}

GuiTrigger::~GuiTrigger()
{
}

void GuiTrigger::SetSimpleTrigger(s32 ch, u32 wiibtns, u16 gcbtns)
{
    this->chan = ch;
    this->wiibtns = wiibtns;
    this->gcbtns = gcbtns;
    type = TRIGGER_SIMPLE;
}

void GuiTrigger::SetHeldTrigger(s32 ch, u32 wiibtns, u16 gcbtns)
{
    this->chan = ch;
    this->wiibtns = wiibtns;
    this->gcbtns = gcbtns;
    type = TRIGGER_HELD;
}

void GuiTrigger::SetButtonOnlyTrigger(s32 ch, u32 wiibtns, u16 gcbtns)
{
    this->chan = ch;
    this->wiibtns = wiibtns;
    this->gcbtns = gcbtns;
    type = TRIGGER_BUTTON_ONLY;
}

void GuiTrigger::SetButtonOnlyInFocusTrigger(s32 ch, u32 wiibtns, u16 gcbtns)
{
    this->chan = ch;
    this->wiibtns = wiibtns;
    this->gcbtns = gcbtns;
    type = TRIGGER_BUTTON_ONLY_IN_FOCUS;
}

bool GuiTrigger::Left()
{
    // Placeholder - will integrate with actual controller input later
    return false;
}

bool GuiTrigger::Right()
{
    // Placeholder - will integrate with actual controller input later
    return false;
}

bool GuiTrigger::Up()
{
    // Placeholder - will integrate with actual controller input later
    return false;
}

bool GuiTrigger::Down()
{
    // Placeholder - will integrate with actual controller input later
    return false;
}

/**
 * Update trigger state based on controller input.
 */
void GuiTrigger::Update()
{
    // Scan controllers
    PAD_ScanPads();
    #ifdef HW_RVL
    WPAD_ScanPads();
    #endif
    
    // Track button states
    u32 padButtons = 0;
    u32 padButtonsDown = 0;
    
    #ifdef HW_RVL
    u32 wpadButtons = 0;
    u32 wpadButtonsDown = 0;
    #endif
    
    if (chan >= 0)
    {
        // Specific channel
        padButtons = PAD_ButtonsHeld(chan);
        padButtonsDown = PAD_ButtonsDown(chan);
        
        #ifdef HW_RVL
        wpadButtons = WPAD_ButtonsHeld(chan);
        wpadButtonsDown = WPAD_ButtonsDown(chan);
        #endif
    }
    else
    {
        // Any channel
        for (int i = 0; i < 4; i++)
        {
            padButtons |= PAD_ButtonsHeld(i);
            padButtonsDown |= PAD_ButtonsDown(i);
            
            #ifdef HW_RVL
            wpadButtons |= WPAD_ButtonsHeld(i);
            wpadButtonsDown |= WPAD_ButtonsDown(i);
            #endif
        }
    }
    
    // Update based on trigger type
    if (type == TRIGGER_SIMPLE || type == TRIGGER_BUTTON_ONLY)
    {
        // Check for button press
        if ((padButtonsDown & gcbtns) || 
            #ifdef HW_RVL
            (wpadButtonsDown & wiibtns)
            #else
            0
            #endif
           )
        {
            // Trigger activated (handled by buttons)
        }
    }
    else if (type == TRIGGER_HELD)
    {
        // Check for button held
        if ((padButtons & gcbtns) || 
            #ifdef HW_RVL
            (wpadButtons & wiibtns)
            #else
            0
            #endif
           )
        {
            // Trigger held (handled by buttons)
        }
    }
}
