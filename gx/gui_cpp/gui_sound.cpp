/****************************************************************************
 * Genesis Plus GX - GUI System
 * gui_sound.cpp
 *
 * GuiSound - Sound playback wrapper
 ***************************************************************************/

#include "gui.h"

// ASND is available on both GameCube and Wii
extern "C" {
#include <asndlib.h>
}

GuiSound::GuiSound(const u8 * s, s32 l, int t)
{
    sound = s;
    length = l;
    type = t;
    voice = -1;
    volume = 255;  // Max volume (0-255)
    loop = false;
}

GuiSound::~GuiSound()
{
    Stop();
}

void GuiSound::Play()
{
    if (!sound || !length)
        return;
    
    if (type == SOUND_PCM)
    {
        // Get an unused voice channel
        voice = ASND_GetFirstUnusedVoice();
        if (voice >= 0)
        {
            // Stop any existing playback on this voice
            ASND_StopVoice(voice);
            
            // Set up voice for PCM playback
            // VOICE_MONO_16BIT, 48kHz sample rate
            ASND_SetVoice(voice, 
                         VOICE_MONO_16BIT, 
                         48000,  // Sample rate
                         0,      // Delay
                         (u8*)sound, 
                         length, 
                         volume,  // Left volume
                         volume,  // Right volume
                         NULL);   // Callback
        }
    }
}

void GuiSound::Stop()
{
    if (voice < 0)
        return;
    
    ASND_StopVoice(voice);
    voice = -1;
}

void GuiSound::Pause()
{
    if (voice < 0)
        return;
    
    ASND_PauseVoice(voice, 1);
}

void GuiSound::Resume()
{
    if (voice < 0)
        return;
    
    ASND_PauseVoice(voice, 0);
}

bool GuiSound::IsPlaying()
{
    if (voice < 0)
        return false;
    
    return (ASND_StatusVoice(voice) == SND_WORKING);
}

void GuiSound::SetVolume(int v)
{
    volume = v;
    
    // Update voice volume if playing
    if (voice >= 0)
    {
        ASND_ChangeVolumeVoice(voice, volume, volume);
    }
}

void GuiSound::SetLoop(bool l)
{
    loop = l;
    // TODO: Implement looping with callback
}
