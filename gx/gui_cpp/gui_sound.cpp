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
    if (!sound || !length)
        return;
    
    // TODO: Integrate with ASND audio system
    // ASND_SetVoice(voice, VOICE_MONO_16BIT, frequency, 0, sound, length, volume, volume, NULL);
}

void GuiSound::Stop()
{
    // TODO: Stop ASND voice
    // ASND_StopVoice(voice);
}

void GuiSound::Pause()
{
    // TODO: Pause ASND voice
    // ASND_PauseVoice(voice, 1);
}

void GuiSound::Resume()
{
    // TODO: Resume ASND voice
    // ASND_PauseVoice(voice, 0);
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
