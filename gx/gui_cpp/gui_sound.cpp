/****************************************************************************
 * Genesis Plus GX - GUI System
 * gui_sound.cpp
 *
 * GuiSound - Sound playback wrapper
 ***************************************************************************/

#include "gui.h"

GuiSound::GuiSound(const u8 * s, s32 l, int t)
{
    sound = s;
    length = l;
    type = t;
    volume = 100;
    loop = false;
}

GuiSound::~GuiSound()
{
}

void GuiSound::Play()
{
    // Placeholder - will integrate with actual audio system later
}

void GuiSound::Stop()
{
    // Placeholder
}

void GuiSound::Pause()
{
    // Placeholder
}

void GuiSound::Resume()
{
    // Placeholder
}

bool GuiSound::IsPlaying()
{
    // Placeholder
    return false;
}

void GuiSound::SetVolume(int v)
{
    volume = v;
}

void GuiSound::SetLoop(bool l)
{
    loop = l;
}
