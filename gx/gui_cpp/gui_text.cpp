/****************************************************************************
 * Genesis Plus GX - GUI System
 *
 * Based on libwiigui from snes9xGC (Tantric 2009)
 * Adapted for Genesis Plus GX (2024)
 *
 * gui_text.cpp - Text rendering element
 ***************************************************************************/

#include "gui.h"

// Static preset variables
static GXColor presetColor = (GXColor){255, 255, 255, 255};
static int presetSize = 0;
static int presetMaxWidth = 0;
static int presetAlignmentHor = 0;
static int presetAlignmentVert = 0;
static u16 presetStyle = 0;

#define TEXT_SCROLL_DELAY          8
#define TEXT_SCROLL_INITIAL_DELAY  6

/**
 * Constructor for the GuiText class.
 */
GuiText::GuiText(const char * t, int s, GXColor c)
{
    origText = NULL;
    text = NULL;
    size = s;
    color = c;
    alpha = c.a;
    style = FTGX_JUSTIFY_CENTER | FTGX_ALIGN_MIDDLE;
    maxWidth = 0;
    wrap = false;
    textDynNum = 0;
    textScroll = SCROLL_NONE;
    textScrollPos = 0;
    textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
    textScrollDelay = TEXT_SCROLL_DELAY;
    
    alignmentHor = ALIGN_CENTRE;
    alignmentVert = ALIGN_MIDDLE;
    
    if(t)
    {
        origText = strdup(t);
        // TODO: Implement charToWideChar conversion when text system is integrated
        // For now, simple ASCII to wchar_t conversion
        int len = strlen(t);
        text = new wchar_t[len + 1];
        for(int i = 0; i < len; i++)
            text[i] = (wchar_t)t[i];
        text[len] = 0;
    }
    
    for(int i = 0; i < 20; i++)
        textDyn[i] = NULL;
}

/**
 * Constructor for the GuiText class, uses presets
 */
GuiText::GuiText(const char * t)
{
    origText = NULL;
    text = NULL;
    size = presetSize;
    color = presetColor;
    alpha = presetColor.a;
    style = presetStyle;
    maxWidth = presetMaxWidth;
    wrap = false;
    textDynNum = 0;
    textScroll = SCROLL_NONE;
    textScrollPos = 0;
    textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
    textScrollDelay = TEXT_SCROLL_DELAY;
    
    alignmentHor = presetAlignmentHor;
    alignmentVert = presetAlignmentVert;
    
    if(t)
    {
        origText = strdup(t);
        // TODO: Implement charToWideChar conversion when text system is integrated
        // For now, simple ASCII to wchar_t conversion
        int len = strlen(t);
        text = new wchar_t[len + 1];
        for(int i = 0; i < len; i++)
            text[i] = (wchar_t)t[i];
        text[len] = 0;
    }
    
    for(int i = 0; i < 20; i++)
        textDyn[i] = NULL;
}

/**
 * Destructor for the GuiText class.
 */
GuiText::~GuiText()
{
    if(origText)
        free(origText);
    if(text)
        delete[] text;
    
    if(textDynNum > 0)
    {
        for(int i = 0; i < textDynNum; i++)
            if(textDyn[i])
                delete[] textDyn[i];
    }
}

void GuiText::SetText(const char * t)
{
    if(origText)
        free(origText);
    if(text)
        delete[] text;
    
    if(textDynNum > 0)
    {
        for(int i = 0; i < textDynNum; i++)
            if(textDyn[i])
                delete[] textDyn[i];
    }
    
    origText = NULL;
    text = NULL;
    textDynNum = 0;
    textScrollPos = 0;
    textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
    
    if(t)
    {
        origText = strdup(t);
        // TODO: Implement charToWideChar conversion when text system is integrated
        // For now, simple ASCII to wchar_t conversion
        int len = strlen(t);
        text = new wchar_t[len + 1];
        for(int i = 0; i < len; i++)
            text[i] = (wchar_t)t[i];
        text[len] = 0;
    }
}

void GuiText::SetWText(wchar_t * t)
{
    if(origText)
        free(origText);
    if(text)
        delete[] text;
    
    if(textDynNum > 0)
    {
        for(int i = 0; i < textDynNum; i++)
            if(textDyn[i])
                delete[] textDyn[i];
    }
    
    origText = NULL;
    text = NULL;
    textDynNum = 0;
    textScrollPos = 0;
    textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
    
    if(t)
        text = wcsdup(t);
}

int GuiText::GetLength()
{
    if(!text)
        return 0;
    
    return wcslen(text);
}

void GuiText::SetPresets(int sz, GXColor c, int w, u16 s, int h, int v)
{
    presetSize = sz;
    presetColor = c;
    presetStyle = s;
    presetMaxWidth = w;
    presetAlignmentHor = h;
    presetAlignmentVert = v;
}

void GuiText::SetFontSize(int s)
{
    size = s;
}

void GuiText::SetMaxWidth(int width)
{
    maxWidth = width;
    
    for(int i = 0; i < textDynNum; i++)
    {
        if(textDyn[i])
        {
            delete[] textDyn[i];
            textDyn[i] = NULL;
        }
    }
    
    textDynNum = 0;
}

int GuiText::GetTextWidth()
{
    if(!text)
        return 0;
    
    // TODO: Calculate actual width using FreeTypeGX when integrated
    // For now, return approximate width based on character count
    return wcslen(text) * size / 2;
}

void GuiText::SetScroll(int s)
{
    textScroll = s;
    textScrollPos = 0;
    textScrollInitialDelay = TEXT_SCROLL_INITIAL_DELAY;
    textScrollDelay = TEXT_SCROLL_DELAY;
}

void GuiText::SetWrap(bool w, int width)
{
    wrap = w;
    if(width > 0)
        maxWidth = width;
    
    // Clear existing wrapped text
    for(int i = 0; i < textDynNum; i++)
    {
        if(textDyn[i])
        {
            delete[] textDyn[i];
            textDyn[i] = NULL;
        }
    }
    
    textDynNum = 0;
    
    // TODO: Implement text wrapping when FreeTypeGX is integrated
    // For now, wrapping is prepared but not calculated
}

void GuiText::SetColor(GXColor c)
{
    color = c;
    alpha = c.a;
}

void GuiText::SetStyle(u16 s)
{
    style = s;
}

void GuiText::SetAlignment(int hor, int vert)
{
    alignmentHor = hor;
    alignmentVert = vert;
}

void GuiText::ResetText()
{
    // Re-translate text if needed
    // TODO: Implement when translation system is integrated
    if(origText)
    {
        SetText(origText);
    }
}

void GuiText::Draw()
{
    if(!text || !this->IsVisible())
        return;
    
    GXColor tempColor = color;
    tempColor.a = this->GetAlpha();
    
    int newSize = size * this->GetScale();
    
    // TODO: Implement actual text rendering with FreeTypeGX
    // This will call FreeTypeGX rendering functions to draw the text
    // at the calculated position with proper alignment and effects
    
    // Placeholder structure for future rendering:
    // 1. Calculate final position based on alignment
    // 2. Apply effects (fade, scale, etc.)
    // 3. Handle scrolling if enabled
    // 4. Render wrapped lines if wrap is enabled
    // 5. Call FreeTypeGX DrawText with calculated parameters
    
    (void)newSize;    // Suppress unused variable warning
    (void)tempColor;  // Suppress unused variable warning
    
    this->UpdateEffects();
}
