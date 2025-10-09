# Phase 3: GUI Architecture Planning Document

## Overview

This document outlines the architecture and implementation plan for the Genesis Plus GX GUI system, based on the proven libwiigui framework used in snes9xGC and fceuGC.

**Date**: 2024  
**Phase**: 3.1 - Architecture Planning  
**Reference**: snes9xGC GUI system

---

## Reference Architecture (snes9xGC)

### Class Hierarchy

```
GuiSound         (independent)
GuiTrigger       (independent)
GuiImageData     (independent)
    │
GuiElement (base class)
    ├── GuiImage
    ├── GuiText
    ├── GuiTooltip
    └── GuiButton
    │
GuiWindow : GuiElement
    ├── GuiKeyboard
    │
    └── (custom windows)
```

### Additional Widget Classes
- GuiOptionBrowser : GuiElement
- GuiSaveBrowser : GuiElement
- GuiFileBrowser : GuiElement

### File Structure (snes9xGC)
```
source/gui/
├── gui.h                    (37,983 bytes - master header)
├── gui_element.cpp          (588 lines)
├── gui_window.cpp           (451 lines)
├── gui_button.cpp           (369 lines)
├── gui_image.cpp            (163 lines)
├── gui_imagedata.cpp        (28 lines)
├── gui_text.cpp             (316 lines)
├── gui_trigger.cpp          (270 lines)
├── gui_sound.cpp            (82 lines)
├── gui_filebrowser.cpp      (428 lines)
├── gui_savebrowser.cpp      (377 lines)
├── gui_optionbrowser.cpp    (314 lines)
└── gui_keyboard.cpp         (317 lines)

Total: ~3,700 lines of implementation
```

---

## Planned Architecture (Genesis Plus GX)

### File Structure
```
gx/gui/
├── gui.h                    (master header with all class definitions)
├── gui_element.cpp          (GuiElement base class)
├── gui_window.cpp           (GuiWindow container)
├── gui_button.cpp           (GuiButton)
├── gui_image.cpp            (GuiImage)
├── gui_imagedata.cpp        (GuiImageData)
├── gui_text.cpp             (GuiText)
├── gui_trigger.cpp          (GuiTrigger - input handling)
├── gui_sound.cpp            (GuiSound)
├── gui_filebrowser.cpp      (file selection widget)
├── gui_savebrowser.cpp      (save state browser)
├── gui_optionbrowser.cpp    (settings menu widget)
└── gui_keyboard.cpp         (on-screen keyboard)
```

### Class Responsibilities

#### GuiElement (Base Class)
**Purpose**: Foundation for all GUI elements

**Key Members**:
- Position (x, y) and dimensions (width, height)
- Scale, alpha, rotation
- Visibility, selectability, clickability states
- Parent/child relationships
- Effects (fade, slide, scale)
- Alignment (left, right, center, top, bottom, middle)
- Update/Draw callbacks

**Key Methods**:
- `Update(GuiTrigger* trigger)` - Process input
- `Draw()` - Render element
- `SetPosition()`, `SetAlignment()`, `SetScale()`
- `SetState()`, `SetVisible()`, `SetSelectable()`
- `SetEffect()` - Animation effects
- `GetState()`, `IsInside()`, `IsVisible()`

**Estimated**: ~300 lines

---

#### GuiWindow : GuiElement
**Purpose**: Container for multiple elements with focus management

**Key Members**:
- Vector of child GuiElements
- Focus tracking
- Window state

**Key Methods**:
- `Append(GuiElement*)` - Add child
- `Insert(GuiElement*, int index)` - Insert at position
- `Remove(GuiElement*)` - Remove child
- `RemoveAll()` - Clear children
- `GetSelected()` - Get focused element
- `MoveSelectionVert/Horiz()` - Navigate focus
- `Update()` - Update all children
- `Draw()` - Draw all children

**Estimated**: ~250 lines

---

#### GuiTrigger
**Purpose**: Input abstraction for different controllers

**Key Members**:
- Button mappings (GC pad, Wiimote, Classic, etc.)
- Trigger types (simple, held, button-only)
- Controller state tracking

**Key Methods**:
- `SetSimpleTrigger()` - Single button press
- `SetHeldTrigger()` - Button hold
- `SetButtonOnlyTrigger()` - Ignore analog
- `Clicked(GuiTrigger*)` - Check if triggered
- `Left()`, `Right()`, `Up()`, `Down()` - Directional

**Estimated**: ~200 lines

---

#### GuiImageData
**Purpose**: Load and store PNG image data

**Key Members**:
- Image data buffer
- Width, height
- GX texture object

**Key Methods**:
- Constructor(const u8* img) - From compiled PNG
- `GetWidth()`, `GetHeight()`
- `GetImage()` - Get raw data

**Estimated**: ~50 lines (very simple wrapper)

---

#### GuiImage : GuiElement
**Purpose**: Display images with transformations

**Key Members**:
- GuiImageData pointer
- Tile settings (horizontal/vertical)
- Color modulation
- Stripe effects

**Key Methods**:
- `SetImage(GuiImageData*)`
- `SetAngle()`, `SetTile()`, `SetStripe()`
- `ColorStripe()` - Stripe effect
- `Draw()` - Render with GX

**Estimated**: ~200 lines

---

#### GuiText : GuiElement
**Purpose**: Render text with FreeTypeGX

**Key Members**:
- Text string (wchar_t*)
- Font size, color
- Text wrapping/scrolling
- Alignment

**Key Methods**:
- `SetText(const char*)` - Set text
- `SetColor()`, `SetFont()`
- `SetWrap()`, `SetScroll()`
- `SetMaxWidth()` - Text wrapping
- `Draw()` - Render text

**Estimated**: ~350 lines

---

#### GuiButton : GuiElement
**Purpose**: Interactive button with states

**Key Members**:
- Images for each state (default, selected, held, disabled)
- GuiText label
- GuiImage icon
- GuiTrigger array
- GuiSound hover/click sounds
- Click callback

**Key Methods**:
- `SetImage()`, `SetImageOver()`, `SetImageHold()`
- `SetIcon()`, `SetLabel()`
- `SetSoundOver()`, `SetSoundClick()`
- `SetTrigger()` - Assign trigger
- `ResetState()`, `SetState()`
- `Update()` - Handle input
- `Draw()` - Render button

**Estimated**: ~400 lines

---

#### GuiSound
**Purpose**: Play sound effects and music

**Key Members**:
- Sound data pointer
- Sound type (PCM, OGG)
- Volume, loop settings

**Key Methods**:
- Constructor(const u8* sound, s32 length, int type)
- `Play()`, `Stop()`, `Pause()`
- `SetVolume()`, `SetLoop()`

**Estimated**: ~150 lines

---

#### GuiFileBrowser : GuiElement
**Purpose**: Browse files/folders on storage devices

**Key Members**:
- File list
- Current directory
- Trigger buttons
- File/folder icons
- Page tracking

**Key Methods**:
- `ResetState()`, `SetFocus()`
- `ParseDirectory()` - Load file list
- `Update()` - Handle navigation
- `Draw()` - Render file list

**Estimated**: ~500 lines

---

#### GuiSaveBrowser : GuiElement
**Purpose**: Browse save states with thumbnails

**Key Members**:
- Save state list
- Thumbnail images
- Timestamps

**Key Methods**:
- `ParseDirectory()` - Load saves
- `Update()` - Handle selection
- `Draw()` - Render saves with thumbnails

**Estimated**: ~450 lines

---

#### GuiOptionBrowser : GuiElement
**Purpose**: Display scrollable options menu

**Key Members**:
- Option list
- Values list
- Scroll position
- Arrow buttons

**Key Methods**:
- `SetOptions()` - Set menu items
- `Update()` - Handle scrolling
- `Draw()` - Render options

**Estimated**: ~400 lines

---

#### GuiKeyboard : GuiWindow
**Purpose**: On-screen keyboard

**Key Members**:
- Key buttons array
- Input text display
- Shift/caps state

**Key Methods**:
- `Update()` - Handle key presses
- `Draw()` - Render keyboard
- `GetText()` - Get typed text

**Estimated**: ~400 lines

---

## Integration Points

### With Existing Code

#### gx/gx_input.c/h
- Map controller input to GuiTrigger
- Provide PAD/WPAD state to GUI system
- Handle controller detection

#### gx/gx_video.c/h
- Provide GX rendering context
- Manage frame buffer
- Handle screen resolution
- Screenshot capture for save states

#### gx/gx_audio.c/h
- Play GUI sound effects via GuiSound
- Background music playback (OGG)
- Volume control integration

#### gx/config.c/h
- Extend t_config structure for GUI settings
- Save/load GUI preferences
- Theme settings

#### gx/fileio/
- File system access for GuiFileBrowser
- Device enumeration (SD, USB, DVD)
- Path handling utilities

---

## Dependencies

### Libraries Required
- **libogc** - GameCube/Wii base library
- **PNGU** - PNG decoding (already in gx/utils/)
- **FreeTypeGX** - Text rendering (already in gx/utils/)
- **libpng** - PNG library
- **libfreetype** - Font library

### Asset Requirements
- UI graphics (buttons, backgrounds, icons)
- Fonts (TrueType)
- Sound effects (PCM for clicks, hovers)
- Music (OGG for background music - optional)

---

## Memory Considerations

### GameCube (24MB RAM)
- Lazy loading of GUI resources
- Texture compression (RGB565)
- Limited simultaneous windows
- Simplified effects

### Wii (88MB RAM)
- MEM2 for GUI textures and resources
- Full effects enabled
- Multiple cached screens
- Larger texture support

---

## Build System Requirements

### Makefile Changes

#### Makefile.gc
```makefile
# Add C++ compiler
CXX = $(PREFIX)g++
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++11

# Add GUI sources
GUI_SOURCES = $(wildcard gx/gui/*.cpp)
GUI_OBJS = $(GUI_SOURCES:.cpp=.o)

# Link with C++ linker
$(TARGET).elf: $(OBJS) $(GUI_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

# Compile GUI files
gx/gui/%.o: gx/gui/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
```

#### Compiler Flags
- `-fno-exceptions` - Disable C++ exceptions (save code size)
- `-fno-rtti` - Disable runtime type info (save code size)
- `-std=c++11` - C++11 standard for modern features
- `-fvisibility=hidden` - Hide symbols by default

---

## Implementation Order

### Phase 3.2: Base Classes (5 days)

**Day 1: Foundation**
1. Create `gx/gui/gui.h` skeleton
2. Implement GuiElement (base class) - ~300 lines
3. Implement GuiTrigger (input) - ~200 lines
4. Test compilation

**Day 2: Data & Display**
1. Implement GuiImageData - ~50 lines
2. Implement GuiImage - ~200 lines
3. Implement GuiSound - ~150 lines
4. Test image display

**Day 3: Text & Windows**
1. Implement GuiText - ~350 lines
2. Implement GuiWindow - ~250 lines
3. Test text rendering and containers

**Day 4: Interaction**
1. Implement GuiButton - ~400 lines
2. Test button states and callbacks
3. Integration testing

**Day 5: Verification**
1. Memory leak testing
2. Code review against snes9xGC
3. Documentation updates
4. CI build verification

---

## Testing Strategy

### Unit Tests (Where Possible)
- GuiTrigger button mapping
- GuiElement state management
- Position/alignment calculations

### Integration Tests (Manual)
- Simple window with button
- Image display
- Text rendering
- Sound playback
- Controller input

### Hardware Testing
- GameCube: Memory usage, performance
- Wii: Full feature set, all controllers

---

## Code Style Guidelines

### Naming Conventions
```cpp
// Classes: PascalCase
class GuiElement { };

// Methods: PascalCase
void SetPosition(int x, int y);

// Members: camelCase
int xOffset;
bool isVisible;

// Constants: UPPER_SNAKE_CASE
#define ALIGN_LEFT 0
```

### Memory Management
```cpp
// Use smart patterns, but manual management for performance
GuiElement* elem = new GuiElement();
// ... use ...
delete elem;

// In containers, track ownership
std::vector<GuiElement*> elements; // GuiWindow owns these
```

### Header Organization
```cpp
// gui.h structure:
// 1. Includes
// 2. Enums and defines
// 3. Forward declarations
// 4. Independent classes (GuiSound, GuiTrigger, GuiImageData)
// 5. GuiElement and derivatives
// 6. Widget classes
```

---

## Success Criteria

### Phase 3.2 Complete When:
- [x] All 9 base classes implemented
- [x] Clean compilation (no warnings)
- [x] Follows snes9xGC patterns
- [x] Memory leak free (valgrind tested)
- [x] Basic functionality verified
- [x] Documentation complete

### Phase 3.3 Complete When:
- [x] Makefiles updated for C++
- [x] GameCube builds successfully
- [x] Wii builds successfully
- [x] Binary size acceptable (<3MB)
- [x] CI builds passing

---

## Risks and Mitigation

### Risk: Binary Size Bloat
**Mitigation**: 
- Use `-fno-exceptions -fno-rtti`
- Enable `-fvisibility=hidden`
- Link-time optimization
- Strip symbols in release

### Risk: Memory Leaks
**Mitigation**:
- Clear ownership patterns
- Destructor hierarchy
- Testing with mock environment

### Risk: Performance Issues
**Mitigation**:
- Display lists for static UI
- Texture atlas for small images
- Lazy loading
- GC-specific optimizations

---

## Next Steps (Phase 3.2)

1. Create `gx/gui/gui.h` with all class declarations
2. Implement GuiElement (foundation)
3. Implement GuiTrigger (input)
4. Implement image/sound classes
5. Implement GuiText, GuiWindow, GuiButton
6. Update Makefiles
7. Test and verify

**Estimated Time**: 5-7 days  
**Current Phase**: 3.1 (Planning) ✅  
**Next Phase**: 3.2 (Implementation)

---

**Document Version**: 1.0  
**Last Updated**: 2024  
**Status**: Planning Complete
