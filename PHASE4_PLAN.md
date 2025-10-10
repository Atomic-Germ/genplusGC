# Phase 4: Hardware Integration Plan

## Overview

Phase 4 integrates the C++ GUI foundation with Genesis Plus GX hardware APIs for rendering, input, and audio. The C++ GUI classes currently have stub implementations that need to be connected to the actual GX, PAD/WPAD, and ASND systems.

**Duration:** 3-4 days  
**Branch:** `phase4-hardware-integration`

---

## Current State

### C++ GUI Classes (Ready but Stubbed)
- ✅ GuiElement (base class) - Complete structure
- ✅ GuiImage - Needs Draw() implementation
- ✅ GuiText - Needs Draw() implementation  
- ✅ GuiTrigger - Needs Update() implementation
- ✅ GuiSound - Needs Play/Stop/Pause implementation
- ✅ GuiWindow - Complete (uses child Draw/Update)
- ✅ GuiButton - Complete (uses child Draw/Update)

### Existing Genesis Plus GX Infrastructure
- ✅ GX rendering functions in `gx/gx_video.c`:
  - `gxDrawTexture()` - Draw texture at position
  - `gxDrawRectangle()` - Draw filled rectangle
  - `gxDrawTextureRotate()` - Draw with rotation
  - `gxDrawTextureRepeat()` - Tiled drawing
- ✅ Input handling in `gx/gx_input.c`:
  - PAD_ScanPads(), WPAD_ScanPads()
  - Button state tracking
- ✅ Audio in `gx/gx_audio.c`:
  - ASND library integration

### Reference Implementations
- ✅ `../snes9xGC/source/gui/` - Complete working GUI
- ✅ `../snes9xGC/source/video.cpp` - Menu_DrawImg(), Menu_DrawRectangle()
- ✅ `../snes9xGC/source/gui/gui_trigger.cpp` - Input Update()

---

## Implementation Plan

### Phase 4.1: Rendering Integration (2 days)

#### Step 1: Create GUI Rendering Helper Functions

Create `gx/gui_drawing.c` and `gx/gui_drawing.h` with helper functions adapted from snes9xGC:

```c
// gx/gui_drawing.h
#ifndef _GUI_DRAWING_H
#define _GUI_DRAWING_H

#include <gccore.h>

void GUI_InitVideo();
void GUI_DrawInit();
void GUI_DrawImg(f32 xpos, f32 ypos, u16 width, u16 height, u8 data[],
                 f32 degrees, f32 scaleX, f32 scaleY, u8 alpha);
void GUI_DrawRectangle(f32 x, f32 y, f32 width, f32 height, GXColor color, u8 filled);
void GUI_DrawText(int x, int y, const char *text, int size, GXColor color);

#endif
```

**Implementation Notes:**
- Adapt `Menu_DrawImg()` from snes9xGC
- Adapt `Menu_DrawRectangle()` from snes9xGC
- Use existing `gxDrawTexture()` and `gxDrawRectangle()` as reference
- Handle GX texture format conversion (RGBA8)

#### Step 2: Implement GuiImage::Draw()

Update `gx/gui_cpp/gui_image.cpp`:

```cpp
void GuiImage::Draw()
{
    if (!image || !this->IsVisible() || tile == 0)
        return;

    float currScaleX = this->GetScaleX();
    float currScaleY = this->GetScaleY();
    int currLeft = this->GetLeft();
    int thisTop = this->GetTop();

    if (tile > 0)
    {
        // Tiled rendering
        int alpha = this->GetAlpha();
        for (int i = 0; i < tile; ++i)
        {
            GUI_DrawImg(currLeft + width * i, thisTop, width, height, 
                       image, imageangle, currScaleX, currScaleY, alpha);
        }
    }
    else
    {
        // Single image
        GUI_DrawImg(currLeft, thisTop, width, height, image, 
                   imageangle, currScaleX, currScaleY, this->GetAlpha());
    }

    // Stripe effect
    if (stripe > 0)
    {
        int thisHeight = this->GetHeight();
        int thisWidth = this->GetWidth();
        for (int y = 0; y < thisHeight; y += 6)
            GUI_DrawRectangle(currLeft, thisTop + y, thisWidth, 3, 
                            (GXColor){0, 0, 0, (u8)stripe}, 1);
    }

    this->UpdateEffects();
}
```

#### Step 3: Implement GuiText::Draw()

Update `gx/gui_cpp/gui_text.cpp`:

Options:
1. Use existing FreeTypeGX (if available in gx/utils)
2. Use simple bitmap font rendering
3. Adapt from snes9xGC text rendering

```cpp
void GuiText::Draw()
{
    if (!text || !this->IsVisible())
        return;

    int currLeft = this->GetLeft();
    int thisTop = this->GetTop();
    
    // TODO: Implement with FreeTypeGX or bitmap fonts
    // GUI_DrawText(currLeft, thisTop, text, size, color);
    
    this->UpdateEffects();
}
```

#### Step 4: PNG Image Loading

Update `gx/gui_cpp/gui_imagedata.cpp` to use PNGU:

```cpp
GuiImageData::GuiImageData(const u8 * img)
{
    PNGUPROP imgProp;
    IMGCTX ctx = PNGU_SelectImageFromBuffer(img);
    
    if (ctx)
    {
        if (PNGU_GetImageProperties(ctx, &imgProp) == PNGU_OK)
        {
            width = imgProp.imgWidth;
            height = imgProp.imgHeight;
            
            int len = width * height * 4;
            if (len % 32) len += (32 - len % 32);
            
            data = (u8 *)memalign(32, len);
            
            if (data)
            {
                PNGU_DecodeTo4x4RGBA8(ctx, imgProp.imgWidth, imgProp.imgHeight, 
                                     data, 255);
                DCFlushRange(data, len);
            }
        }
        PNGU_ReleaseImageContext(ctx);
    }
}
```

---

### Phase 4.2: Controller Integration (1 day)

#### Step 1: Implement GuiTrigger::Update()

Update `gx/gui_cpp/gui_trigger.cpp`:

```cpp
void GuiTrigger::Update()
{
    // Scan controllers
    PAD_ScanPads();
    #ifdef HW_RVL
    WPAD_ScanPads();
    #endif
    
    // Update button states
    u32 padButtons = 0;
    u32 wpadButtons = 0;
    
    if (chan >= 0)
    {
        // Specific channel
        padButtons = PAD_ButtonsDown(chan);
        #ifdef HW_RVL
        wpadButtons = WPAD_ButtonsDown(chan);
        #endif
    }
    else
    {
        // Any channel
        for (int i = 0; i < 4; i++)
        {
            padButtons |= PAD_ButtonsDown(i);
            #ifdef HW_RVL
            wpadButtons |= WPAD_ButtonsDown(i);
            #endif
        }
    }
    
    // Check trigger conditions
    if (type == TRIGGER_SIMPLE)
    {
        if ((padButtons & pad) || (wpadButtons & wpad))
        {
            // Trigger activated
        }
    }
    // ... handle TRIGGER_HELD, TRIGGER_BUTTON_ONLY
}
```

#### Step 2: Test Input

Create test program to verify:
- Button presses detected
- Wiimote pointer position tracked
- Trigger callbacks work

---

### Phase 4.3: Audio Integration (1 day)

#### Step 1: Implement GuiSound::Play/Stop/Pause()

Update `gx/gui_cpp/gui_sound.cpp`:

```cpp
void GuiSound::Play()
{
    if (!sound || !length)
        return;
    
    // Use ASND to play PCM sound
    // Check existing gx/gx_audio.c for reference
    
    // For now, stub:
    // ASND_SetVoice(voice, format, sound, length, freq, volume, volume, NULL);
}

void GuiSound::Stop()
{
    // ASND_StopVoice(voice);
}

void GuiSound::Pause()
{
    // ASND_PauseVoice(voice, 1);
}
```

#### Step 2: Test Audio

- Load PCM sound effect
- Play on button click
- Verify volume control

---

### Phase 4.4: Integration Testing (1 day)

#### Test 1: Simple Window with Button

```cpp
// Test in main.c or test file
GuiWindow testWindow(640, 480);

GuiImageData btnImgData(button_png);
GuiImage btnImg(&btnImgData);

GuiText btnText("Click Me");
GuiButton testBtn(100, 50);
testBtn.SetImage(&btnImg);
testBtn.SetLabel(&btnText);

GuiTrigger trigA;
trigA.SetSimpleTrigger(-1, WPAD_BUTTON_A, PAD_BUTTON_A);
testBtn.SetTrigger(&trigA);

testWindow.Append(&testBtn);

// In render loop:
testWindow.Draw();
```

#### Test 2: Image Display

- Load PNG image
- Display on screen
- Verify correct position and size

#### Test 3: Text Rendering

- Render text at different positions
- Test different font sizes
- Verify colors

#### Test 4: Input Response

- Button highlights on hover
- Click triggers callback
- Sound plays on interaction

---

## File Changes Summary

### New Files
- `gx/gui_drawing.c` - GUI rendering helpers
- `gx/gui_drawing.h` - GUI rendering header
- `gx/gui_test.c` - Integration test program (optional)

### Modified Files
- `gx/gui_cpp/gui_image.cpp` - Implement Draw()
- `gx/gui_cpp/gui_text.cpp` - Implement Draw()
- `gx/gui_cpp/gui_imagedata.cpp` - Implement PNG loading
- `gx/gui_cpp/gui_trigger.cpp` - Implement Update()
- `gx/gui_cpp/gui_sound.cpp` - Implement Play/Stop/Pause()

### Build System
- Update Makefile.gc to include gui_drawing.c
- Update Makefile.wii to include gui_drawing.c

---

## Success Criteria

### Phase 4.1: Rendering ✅
- [ ] GuiImage::Draw() renders textures correctly
- [ ] GuiText::Draw() renders text correctly
- [ ] Images display at correct position and size
- [ ] Rotation and scaling work
- [ ] Alpha blending works
- [ ] Tiled rendering works

### Phase 4.2: Input ✅
- [ ] GuiTrigger::Update() reads controller state
- [ ] Button presses detected
- [ ] Wiimote pointer tracked (Wii only)
- [ ] Multiple trigger types work (simple/held/button-only)
- [ ] Callbacks fire on trigger

### Phase 4.3: Audio ✅
- [ ] GuiSound::Play() plays PCM audio
- [ ] GuiSound::Stop() stops playback
- [ ] Volume control works
- [ ] Multiple sounds can play

### Phase 4.4: Integration ✅
- [ ] Complete test window renders
- [ ] Button responds to input
- [ ] Sound plays on click
- [ ] No crashes or memory leaks
- [ ] Performance acceptable (60 FPS)

---

## Risk Mitigation

### Risk 1: GX Texture Format Issues
**Mitigation:** 
- Study existing gxDrawTexture() implementation
- Use same RGBA8 format as Genesis Plus GX
- Test with simple images first

### Risk 2: Font Rendering Complexity
**Mitigation:**
- Start with bitmap fonts if FreeTypeGX unavailable
- Can enhance later in Phase 5
- Focus on basic rendering first

### Risk 3: Audio Conflicts
**Mitigation:**
- Study existing ASND usage in gx_audio.c
- Use separate voice channels for GUI sounds
- Test with simple PCM sounds first

### Risk 4: Performance Issues
**Mitigation:**
- Profile with simple test cases
- Optimize texture loading (cache)
- Use display lists if needed

---

## Next Steps After Phase 4

**Phase 5: Menu Implementation**
- Implement splash screen with new GUI
- Create main menu using GuiWindow/GuiButton
- Replace file browser with GuiFileBrowser
- Implement settings menu with GuiOptionBrowser

---

## Reference Materials

### snes9xGC Reference Files
- `../snes9xGC/source/video.cpp` - Rendering helpers
- `../snes9xGC/source/gui/gui_image.cpp` - Image drawing
- `../snes9xGC/source/gui/gui_text.cpp` - Text rendering
- `../snes9xGC/source/gui/gui_trigger.cpp` - Input handling
- `../snes9xGC/source/gui/gui_sound.cpp` - Audio playback

### Genesis Plus GX Files
- `gx/gx_video.c` - Existing GX rendering
- `gx/gx_input.c` - Existing input handling
- `gx/gx_audio.c` - Existing audio system
- `gx/utils/pngu.c` - PNG utilities

---

**Document Status:** Ready for Implementation  
**Phase 4 Start Date:** TBD  
**Estimated Completion:** 3-4 days
