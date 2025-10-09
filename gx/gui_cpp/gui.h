/****************************************************************************
 * Genesis Plus GX - GUI System
 *
 * Based on libwiigui from snes9xGC (Tantric 2009)
 * Adapted for Genesis Plus GX (2024)
 *
 * gui.h - Master GUI header
 ***************************************************************************/

#ifndef _GUICPP_H_
#define _GUICPP_H_

#include <gccore.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <wchar.h>
#include <math.h>

#ifdef HW_RVL
#include <asndlib.h>
#include <wiiuse/wpad.h>
#endif

// Forward declarations
class GuiElement;
class GuiWindow;
class GuiTrigger;

// Alignment
enum { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTRE, ALIGN_TOP, ALIGN_BOTTOM, ALIGN_MIDDLE };

// States
enum { STATE_DEFAULT, STATE_SELECTED, STATE_CLICKED, STATE_HELD, STATE_DISABLED };

// Sound/Image/Trigger types
enum { SOUND_PCM, SOUND_OGG };
enum { IMAGE_TEXTURE, IMAGE_COLOR, IMAGE_DATA };
enum { TRIGGER_SIMPLE, TRIGGER_HELD, TRIGGER_BUTTON_ONLY, TRIGGER_BUTTON_ONLY_IN_FOCUS };

// Effects
#define EFFECT_SLIDE_TOP        1
#define EFFECT_SLIDE_BOTTOM     2
#define EFFECT_SLIDE_RIGHT      4
#define EFFECT_SLIDE_LEFT       8
#define EFFECT_SLIDE_IN         16
#define EFFECT_SLIDE_OUT        32
#define EFFECT_FADE             64
#define EFFECT_SCALE            128

// Callback type
typedef void (*UpdateCallback)(void * e);

//! Sound playback wrapper
class GuiSound
{
public:
    GuiSound(const u8 * s, s32 l, int t);
    ~GuiSound();
    void Play();
    void Stop();
    void Pause();
    void Resume();
    bool IsPlaying();
    void SetVolume(int v);
    void SetLoop(bool l);

protected:
    const u8 * sound;
    s32 length;
    int type;
    int volume;
    bool loop;
};

//! Controller input handler
class GuiTrigger
{
public:
    GuiTrigger();
    ~GuiTrigger();
    void SetSimpleTrigger(s32 ch, u32 wiibtns, u16 gcbtns);
    void SetHeldTrigger(s32 ch, u32 wiibtns, u16 gcbtns);
    void SetButtonOnlyTrigger(s32 ch, u32 wiibtns, u16 gcbtns);
    void SetButtonOnlyInFocusTrigger(s32 ch, u32 wiibtns, u16 gcbtns);
    bool Left();
    bool Right();
    bool Up();
    bool Down();

    s32 chan;
    u32 wiibtns;
    u16 gcbtns;
    u8 type;
};

//! Base class for all GUI elements
class GuiElement
{
public:
    GuiElement();
    virtual ~GuiElement();
    
    void SetParent(GuiElement * e);
    GuiElement * GetParent();
    int GetLeft();
    int GetTop();
    int GetWidth();
    int GetHeight();
    void SetSize(int w, int h);
    bool IsVisible();
    bool IsSelectable();
    bool IsClickable();
    bool IsHoldable();
    void SetSelectable(bool s);
    void SetClickable(bool c);
    void SetHoldable(bool h);
    int GetState();
    int GetStateChan();
    void SetAlpha(int a);
    int GetAlpha();
    void SetScale(float s);
    float GetScale();
    void SetTrigger(GuiTrigger * t);
    void SetTrigger(u8 i, GuiTrigger * t);
    void SetPosition(int x, int y);
    void SetAlignment(int hor, int vert);
    void SetEffect(int e, int a, int t = 0);
    void SetEffectOnOver(int e, int a, int t = 0);
    void SetEffectGrow();
    bool IsInside(int x, int y);
    void SetState(int s, int c = -1);
    void ResetState();
    void SetVisible(bool v);
    void SetFocus(int f);
    int IsFocused();
    
    virtual void Update(GuiTrigger * t);
    virtual void Draw();

protected:
    virtual void UpdateEffects();
    
    int xoffset, yoffset;
    int xmin, xmax, ymin, ymax;
    int width, height;
    int xoffsetDyn, yoffsetDyn;
    int alpha, alphaDyn;
    float xscale, yscale, scaleDyn;
    int effects, effectAmount, effectTarget;
    int effectsOver, effectAmountOver, effectTargetOver;
    int alignmentHor, alignmentVert;
    int state, stateChan;
    bool selectable, clickable, holdable, visible;
    int focus;
    bool rumble;
    GuiTrigger * trigger[5];
    GuiElement * parentElement;
    UpdateCallback updateCB;
};

#endif // _GUICPP_H_
