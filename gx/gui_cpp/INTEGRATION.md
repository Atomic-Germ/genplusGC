# GUI C++ Integration Guide

## Overview

This document describes how to integrate the new C++ GUI foundation with the existing Genesis Plus GX codebase. The C++ GUI (`gx/gui_cpp/`) is designed to coexist with the existing C GUI (`gx/gui/`) during the transition period.

## Architecture

### Dual GUI System

```
Genesis Plus GX
├── gx/gui/          # Existing C GUI (active in production)
│   ├── gui.h/c      # Menu system, legacy code
│   └── menu.c       # ROM browser, settings
│
└── gx/gui_cpp/      # New C++ GUI (foundation ready)
    ├── gui.h        # Class declarations
    └── *.cpp        # Implementation files
```

The two systems are independent and can be integrated gradually:
- **Phase 3.3**: Build system integration (compile alongside C code)
- **Phase 4**: Controller/rendering integration (connect to hardware)
- **Phase 5**: Menu implementation (replace C menus incrementally)

## Class Hierarchy

```
GuiElement (Base)
├── GuiImage        - Image/texture display
├── GuiText         - Text rendering
├── GuiButton       - Interactive button
└── GuiWindow       - Container/layout

GuiTrigger          - Controller input abstraction
GuiSound            - Audio playback wrapper
GuiImageData        - PNG image data storage
```

## Integration Points

### 1. Controller Input Integration

**GuiTrigger** abstracts controller input and needs connection to PAD/WPAD APIs.

```cpp
// In main loop or input handling code:
#include "gx/gui_cpp/gui.h"

GuiTrigger trigA;
trigA.SetSimpleTrigger(-1, WPAD_BUTTON_A, PAD_BUTTON_A);

// Update trigger state from actual controller:
void UpdateTriggers()
{
    // Read PAD/WPAD state
    PAD_ScanPads();
    WPAD_ScanPads();
    
    // Update triggers (TODO: implement Update() with real input)
    trigA.Update();
}
```

**Implementation Needed:**
- Modify `GuiTrigger::Update()` to read actual PAD/WPAD state
- Track button press/release/hold durations
- Handle cursor position for pointer devices

### 2. Graphics/Rendering Integration

**GuiImage** displays textures and needs connection to GX rendering pipeline.

```cpp
// In rendering code:
void RenderGUI()
{
    // Set up GX for 2D rendering
    GX_SetViewport(0, 0, screenWidth, screenHeight, 0, 1);
    
    // Draw GUI elements
    mainWindow->Draw();  // Recursively draws all children
}
```

**Implementation Needed:**
- Modify `GuiImage::Draw()` to call GX texture mapping functions
- Load PNG data via PNGU into GX textures
- Implement tile rendering for repeating backgrounds
- Handle rotation and scaling transformations

**GuiText** renders text and needs connection to font system.

```cpp
// Initialize font system (FreeTypeGX or custom):
void InitFonts()
{
    // Load TTF fonts
    // Initialize FreeTypeGX
}

// In GuiText::Draw():
void GuiText::Draw()
{
    if (!visible || !text) return;
    
    // Set text color
    GXColor color = {r, g, b, a};
    
    // Render using FreeTypeGX or custom font renderer
    ftgx->drawText(x, y, text, fontSize, color, alignment);
}
```

**Implementation Needed:**
- Integrate FreeTypeGX or implement custom font renderer
- Implement `charToWideChar()` for ASCII to wide char conversion
- Implement text wrapping and line height calculations
- Support multiple font sizes

### 3. Audio Integration

**GuiSound** plays sound effects and needs connection to ASND.

```cpp
// Initialize audio system:
void InitAudio()
{
    ASND_Init();
}

// Load sound files:
GuiSound* LoadSound(const char* path)
{
    GuiSound* sound = new GuiSound();
    
    // Load WAV/OGG file from SD/DVD
    u8* buffer = LoadAudioFile(path);
    u32 length = GetAudioFileLength(path);
    
    // Set buffer for GuiSound (TODO: add SetBuffer method)
    sound->SetBuffer(buffer, length);
    
    return sound;
}
```

**Implementation Needed:**
- Modify `GuiSound::Play()` to call ASND playback functions
- Implement audio file loading (WAV/OGG)
- Add SetBuffer() method to GuiSound
- Implement Stop() and Pause() methods

### 4. Resource Loading

**GuiImageData** loads PNG images from filesystem.

```cpp
// Load image from SD card:
GuiImageData* LoadImage(const char* path)
{
    // Read PNG file
    FILE* file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    u32 size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    u8* buffer = (u8*)malloc(size);
    fread(buffer, 1, size, file);
    fclose(file);
    
    // Create image data
    GuiImageData* imgData = new GuiImageData(buffer, size);
    free(buffer);  // GuiImageData makes internal copy
    
    return imgData;
}
```

**Implementation Needed:**
- Integrate PNGU for PNG decoding
- Modify `GuiImageData` constructor to decode PNG
- Extract width, height, and pixel data
- Convert to GX texture format

## Usage Examples

### Creating a Simple Menu

```cpp
// Create window container
GuiWindow* menuWindow = new GuiWindow(640, 480);

// Load resources
GuiImageData* btnImgData = LoadImage("sd:/apps/genplus/images/button.png");
GuiImage* btnImg = new GuiImage(btnImgData);

GuiSound* btnSoundOver = LoadSound("sd:/apps/genplus/sounds/button_over.wav");
GuiSound* btnSoundClick = LoadSound("sd:/apps/genplus/sounds/button_click.wav");

// Create trigger
GuiTrigger trigA;
trigA.SetSimpleTrigger(-1, WPAD_BUTTON_A, PAD_BUTTON_A);

// Create button
GuiText* btnText = new GuiText("Start Game");
GuiButton* btnStart = new GuiButton(200, 48);
btnStart->SetImage(btnImg);
btnStart->SetLabel(btnText);
btnStart->SetSoundOver(btnSoundOver);
btnStart->SetSoundClick(btnSoundClick);
btnStart->SetTrigger(&trigA);
btnStart->SetPosition(220, 200);
btnStart->SetAlignment(ALIGN_LEFT, ALIGN_TOP);

// Add to window
menuWindow->Append(btnStart);

// Main loop
while (running)
{
    // Update GUI
    menuWindow->Update();
    
    // Check button state
    if (btnStart->GetState() == STATE_CLICKED)
    {
        StartGame();
        btnStart->ResetState();
    }
    
    // Render
    GX_ClearScreen();
    menuWindow->Draw();
    GX_Flush();
}

// Cleanup
delete menuWindow;  // Does not delete children
delete btnStart;
delete btnText;
delete btnImg;
delete btnImgData;
delete btnSoundOver;
delete btnSoundClick;
```

### Creating a Settings Menu

```cpp
// Create option buttons
const char* options[] = {"Easy", "Normal", "Hard"};
GuiButton* difficultyBtns[3];

for (int i = 0; i < 3; i++)
{
    GuiText* txt = new GuiText(options[i]);
    GuiButton* btn = new GuiButton(150, 40);
    btn->SetLabel(txt);
    btn->SetPosition(50, 100 + i * 50);
    btn->SetTrigger(&trigA);
    
    difficultyBtns[i] = btn;
    menuWindow->Append(btn);
}

// Handle selection
for (int i = 0; i < 3; i++)
{
    if (difficultyBtns[i]->GetState() == STATE_CLICKED)
    {
        config.difficulty = i;
        difficultyBtns[i]->ResetState();
    }
}
```

## Memory Management

### Ownership Model

The GUI uses **non-owning composition**:

- **GuiWindow** does not delete child elements
- **GuiButton** does not delete image/label/icon elements
- **Caller owns all GUI elements** and must delete them

```cpp
// Create elements
GuiWindow* window = new GuiWindow(640, 480);
GuiButton* btn = new GuiButton(200, 48);
GuiText* txt = new GuiText("Click Me");

// Compose
btn->SetLabel(txt);      // btn does NOT own txt
window->Append(btn);      // window does NOT own btn

// Cleanup (must delete in reverse order)
window->Remove(btn);      // Remove from container
delete window;            // Safe - doesn't delete btn
delete btn;               // Safe - doesn't delete txt
delete txt;               // Delete owned text
```

### Resource Lifecycle

```cpp
// Images
GuiImageData* imgData = new GuiImageData(pngData, pngSize);
GuiImage* img = new GuiImage(imgData);
// img does NOT own imgData - caller must delete both

// Sounds  
GuiSound* sound = new GuiSound();
// sound manages internal buffer - just delete when done

// Text
GuiText* text = new GuiText("Hello");
// text owns internal char* buffer - deletes in destructor
```

## Build Integration

### Makefile Changes

Add to `Makefile.gc` and `Makefile.wii`:

```makefile
# C++ GUI sources
GUICPP_SRC = gx/gui_cpp/gui_element.cpp \
             gx/gui_cpp/gui_trigger.cpp \
             gx/gui_cpp/gui_sound.cpp \
             gx/gui_cpp/gui_imagedata.cpp \
             gx/gui_cpp/gui_image.cpp \
             gx/gui_cpp/gui_text.cpp \
             gx/gui_cpp/gui_window.cpp \
             gx/gui_cpp/gui_button.cpp

# Add to object list
GUICPP_OBJ = $(GUICPP_SRC:.cpp=.o)

# Compile C++ files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link with both C and C++ objects
$(TARGET).elf: $(C_OBJ) $(GUICPP_OBJ)
	$(LD) $(LDFLAGS) $(C_OBJ) $(GUICPP_OBJ) $(LIBS) -o $@
```

See `BUILD_INTEGRATION.md` for detailed build system changes.

## Migration Strategy

### Incremental Replacement

1. **Phase 3.3**: Compile C++ GUI alongside C GUI (no conflicts)
2. **Phase 4**: Integrate controller/rendering (test in isolation)
3. **Phase 5.1**: Implement splash screen in C++ (replace first C menu)
4. **Phase 5.2**: Implement main menu in C++ (replace second C menu)
5. **Phase 5.3**: Implement settings menu in C++ (replace third C menu)
6. **Phase 5.4**: Remove old C GUI code

### Coexistence Pattern

During transition, both GUIs can coexist:

```cpp
// Old C GUI
#include "gx/gui/gui.h"
void ShowOldMenu() { /* C code */ }

// New C++ GUI
#include "gx/gui_cpp/gui.h"
void ShowNewMenu() { /* C++ code */ }

// Main code
if (useNewGUI)
    ShowNewMenu();
else
    ShowOldMenu();
```

## Known Limitations

Current implementation has placeholders that need integration:

1. **GuiTrigger::Update()** - Needs actual PAD/WPAD reading
2. **GuiImage::Draw()** - Needs GX texture rendering
3. **GuiText::Draw()** - Needs FreeTypeGX integration
4. **GuiSound::Play()** - Needs ASND integration
5. **GuiImageData constructor** - Needs PNGU PNG decoding
6. **GuiElement::UpdateEffects()** - Needs animation system

These are intentionally left as stubs and will be implemented during Phase 4.

## Testing Strategy

### Unit Testing (Phase 6)

After integration, add unit tests:

```cpp
// Test button state management
void TestButtonStates()
{
    GuiButton btn(100, 50);
    assert(btn.GetState() == STATE_DEFAULT);
    
    btn.SetState(STATE_SELECTED, -1);
    assert(btn.GetState() == STATE_SELECTED);
}

// Test window element management
void TestWindowElements()
{
    GuiWindow win(640, 480);
    GuiButton btn(100, 50);
    
    win.Append(&btn);
    assert(win.GetSize() == 1);
    
    win.Remove(&btn);
    assert(win.GetSize() == 0);
}
```

### Integration Testing

Test with real hardware:
1. Controller input response
2. Image rendering quality
3. Text rendering clarity
4. Sound playback
5. Performance (60 FPS target)

## Troubleshooting

### Common Issues

**Compile Errors:**
- Ensure DevkitPPC is installed and in PATH
- Check that libogc includes are accessible
- Verify C++11 support (`-std=c++11`)

**Runtime Crashes:**
- Check NULL pointer dereferences
- Verify element ownership (don't double-delete)
- Ensure parent/child relationships are correct

**Rendering Issues:**
- Initialize GX properly before GUI drawing
- Set correct viewport and projection
- Check texture format compatibility

**Input Not Working:**
- Call PAD_ScanPads() / WPAD_ScanPads() each frame
- Verify trigger button masks are correct
- Check controller is connected

## Next Steps

After integration:
1. Implement placeholder methods (Draw, Update, Play)
2. Test on real GameCube/Wii hardware
3. Create menu implementations using GUI classes
4. Performance optimization (if needed)
5. Add advanced features (animations, effects)

## References

- `GUI_ARCHITECTURE.md` - Full architecture documentation
- `BUILD_INTEGRATION.md` - Detailed build system changes
- `PHASE3.2_SUMMARY.md` - Implementation summary
- snes9xGC source code - Reference implementation
