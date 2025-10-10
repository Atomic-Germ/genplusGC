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

// Text scrolling
enum { SCROLL_NONE, SCROLL_HORIZONTAL };

// FreeTypeGX text styles (placeholder for future integration)
#define FTGX_NULL               0x0000
#define FTGX_JUSTIFY_LEFT       0x0001
#define FTGX_JUSTIFY_CENTER     0x0002
#define FTGX_JUSTIFY_RIGHT      0x0004
#define FTGX_ALIGN_TOP          0x0010
#define FTGX_ALIGN_MIDDLE       0x0020
#define FTGX_ALIGN_BOTTOM       0x0040

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

//! Image data handler (PNG loading)
class GuiImageData
{
public:
    GuiImageData(const u8 * img);
    ~GuiImageData();
    u8 * GetImage();
    int GetWidth();
    int GetHeight();
    
protected:
    u8 * data;
    int width;
    int height;
};

//! Image display element
class GuiImage : public GuiElement
{
public:
    GuiImage(GuiImageData * img);
    GuiImage(int w, int h, GXColor c);
    ~GuiImage();
    void SetImage(GuiImageData * img);
    void SetAngle(float a);
    void SetTile(int t);
    GXColor GetPixel(int x, int y);
    void SetPixel(int x, int y, GXColor color);
    void SetStripe(int s);
    void ColorStripe(int s);
    
    void Draw();
    
protected:
    int imgType;
    u8 * image;
    GuiImageData * imageData;
    int tile;
    float imageangle;
    int stripe;
};

//! Text rendering element
class GuiText : public GuiElement
{
public:
    GuiText(const char * t, int s, GXColor c);
    GuiText(const char * t);
    ~GuiText();
    void SetText(const char * t);
    void SetWText(wchar_t * t);
    int GetLength();
    static void SetPresets(int sz, GXColor c, int w, u16 s, int h, int v);
    void SetFontSize(int s);
    void SetMaxWidth(int w);
    int GetTextWidth();
    void SetScroll(int s);
    void SetWrap(bool w, int width = 0);
    void SetColor(GXColor c);
    void SetStyle(u16 s);
    void SetAlignment(int hor, int vert);
    void ResetText();
    
    void Draw();
    
protected:
    GXColor color;
    wchar_t* text;
    wchar_t *textDyn[20];
    int textDynNum;
    char * origText;
    int size;
    int maxWidth;
    int textScroll;
    int textScrollPos;
    int textScrollInitialDelay;
    int textScrollDelay;
    u16 style;
    bool wrap;
};

//! Window container for GUI elements
class GuiWindow : public GuiElement
{
public:
    GuiWindow();
    GuiWindow(int w, int h);
    ~GuiWindow();
    void Append(GuiElement* e);
    void Insert(GuiElement* e, u32 i);
    void Remove(GuiElement* e);
    void RemoveAll();
    bool Find(GuiElement* e);
    GuiElement* GetGuiElementAt(u32 i) const;
    u32 GetSize();
    void SetVisible(bool v);
    void ResetState();
    void SetState(int s);
    int GetSelected();
    void SetFocus(int f);
    void ChangeFocus(GuiElement * e);
    void ToggleFocus(GuiTrigger * t);
    void MoveSelectionHor(int d);
    void MoveSelectionVert(int d);
    void ResetText();
    
    void Draw();
    void DrawTooltip();
    void Update(GuiTrigger * t);
    
protected:
    std::vector<GuiElement*> _elements;
};

//! Interactive button widget
class GuiButton : public GuiElement
{
public:
    GuiButton(int w, int h);
    ~GuiButton();
    
    void SetImage(GuiImage* i);
    void SetImageOver(GuiImage* i);
    void SetImageHold(GuiImage* i);
    void SetImageClick(GuiImage* i);
    void SetIcon(GuiImage* i);
    void SetIconOver(GuiImage* i);
    void SetLabel(GuiText* t);
    void SetLabelOver(GuiText* t);
    void SetSoundOver(GuiSound* s);
    void SetSoundHold(GuiSound* s);
    void SetSoundClick(GuiSound* s);
    
    void Draw();
    void Update(GuiTrigger * t);
    
protected:
    GuiImage * image;
    GuiImage * imageOver;
    GuiImage * imageHold;
    GuiImage * imageClick;
    GuiImage * icon;
    GuiImage * iconOver;
    GuiText * label;
    GuiText * labelOver;
    GuiSound * soundOver;
    GuiSound * soundHold;
    GuiSound * soundClick;
};

#endif // _GUICPP_H_
