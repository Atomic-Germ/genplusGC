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
