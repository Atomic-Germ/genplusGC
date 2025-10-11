# Phase 5: Menu Implementation Plan

## Overview

Phase 5 implements the actual user interface using the C++ GUI foundation built in Phases 3 and 4. This phase will create functional menus, integrate text rendering and audio, and provide a complete user experience.

**Duration:** 3-4 days  
**Branch:** `phase5-menu-implementation`  
**Status:** Planning

---

## Current State

### What's Ready from Phase 4 ✅
- ✅ GuiElement base class with effects
- ✅ GuiImage with full GX rendering
- ✅ GuiText structure (needs font integration)
- ✅ GuiTrigger with PAD/WPAD input
- ✅ GuiSound structure (needs audio integration)
- ✅ GuiWindow container management
- ✅ GuiButton interactive elements
- ✅ GUI_DrawImg() and GUI_DrawRectangle()

### Existing Genesis Plus GX Assets ✅
- ✅ Font system in `gx/font.c/h` (bitmap fonts)
- ✅ Audio system in `gx/gx_audio.c` (ASND, oggplayer)
- ✅ Sound files: button_over.pcm, button_select.pcm, intro.pcm
- ✅ 114 PNG images for UI (buttons, backgrounds, icons)
- ✅ Existing C menu system in `gx/gui/menu.c` (reference)

### Reference Implementations ✅
- ✅ snes9xGC complete menu system
- ✅ snes9xGC FreeTypeGX integration
- ✅ snes9xGC file browser, option browser, save browser
- ✅ Genesis Plus GX existing C menus

---

## Phase 5 Objectives

### 5.1: Font Integration (1 day)
**Goal:** Enable text rendering in C++ GUI

**Options:**
1. **Use Existing Bitmap Fonts** (Recommended - Simple)
   - Integrate `gx/font.c` functions with GuiText
   - Call existing `FONT_write()` from `GuiText::Draw()`
   - Fastest to implement, already working

2. **Port FreeTypeGX** (Future enhancement)
   - Would require significant work
   - Better quality but not essential for Phase 5
   - Defer to Phase 7 (Optimization)

**Implementation:**
```cpp
// In gui_text.cpp
void GuiText::Draw()
{
    if (!text || !this->IsVisible())
        return;
    
    int left = this->GetLeft();
    int top = this->GetTop();
    GXColor c = color;
    c.a = this->GetAlpha();
    
    // Convert wchar_t* to char* if needed
    // Call existing bitmap font system
    FONT_writeCenter(text, size, left, top, width, c);
    
    this->UpdateEffects();
}
```

---

### 5.2: Audio Integration (1 day)
**Goal:** Enable sound effects in C++ GUI

**Implementation:**
```cpp
// In gui_sound.cpp
void GuiSound::Play()
{
    if (!sound || !length)
        return;
    
    if (type == SOUND_PCM)
    {
        // Use existing ASND system from gx_audio.c
        ASND_SetVoice(ASND_GetFirstUnusedVoice(), 
                     VOICE_MONO_16BIT, 
                     48000, 0, 
                     (u8*)sound, length, 
                     volume, volume, NULL);
    }
    else if (type == SOUND_OGG)
    {
        // Use oggplayer if needed
    }
}

void GuiSound::Stop()
{
    ASND_StopVoice(voice);
}
```

**Sound Loading:**
```cpp
// Load PCM sounds from gx/sounds/
extern const u8 button_over_pcm[];
extern const u32 button_over_pcm_size;
extern const u8 button_select_pcm[];
extern const u32 button_select_pcm_size;

GuiSound btnSoundOver(button_over_pcm, button_over_pcm_size, SOUND_PCM);
GuiSound btnSoundClick(button_select_pcm, button_select_pcm_size, SOUND_PCM);
```

---

### 5.3: Widget Implementation (1 day)
**Goal:** Create high-level GUI widgets

#### 5.3.1: GuiFileBrowser
Based on snes9xGC `gui_filebrowser.cpp`

**Features:**
- Directory navigation
- File filtering by extension (.md, .smd, .bin, .gen, .sms, .gg, etc.)
- Scroll with D-pad/analog
- Device switching (SD/USB/DVD)
- Integration with existing `gx/fileio/` system

**Structure:**
```cpp
class GuiFileBrowser : public GuiWindow
{
private:
    GuiButton* fileList[30];  // Page of files
    GuiText* fileNames[30];
    GuiImage* fileIcons[30];
    int numEntries;
    int selectedItem;
    int pageIndex;
    
public:
    GuiFileBrowser(int w, int h);
    void SetPath(const char* path);
    void ResetState();
    void Update(GuiTrigger* trigger);
    char* GetSelectedFile();
};
```

#### 5.3.2: GuiOptionBrowser
Based on snes9xGC `gui_optionbrowser.cpp`

**Features:**
- Settings display
- Value adjustment with left/right
- Option categories
- Save/cancel buttons

**Structure:**
```cpp
class GuiOptionBrowser : public GuiWindow
{
private:
    GuiButton* optionBtn[15];
    GuiText* optionTxt[15];
    GuiText* optionVal[15];
    int numOptions;
    int selectedOption;
    
public:
    GuiOptionBrowser(int w, int h);
    void SetOptions(OptionList* options, int cnt);
    void Update(GuiTrigger* trigger);
    int GetClickedOption();
};
```

#### 5.3.3: GuiKeyboard (Optional - Low Priority)
On-screen keyboard for cheat entry

---

### 5.4: Menu System Implementation (1 day)
**Goal:** Create actual menu screens

#### Main Menu
```cpp
int MenuMain()
{
    // Initialize
    GuiWindow mainWindow(640, 480);
    GuiImageData bgData(Bg_main_png);
    GuiImage bg(&bgData);
    
    // Title
    GuiText title("Genesis Plus GX");
    title.SetPosition(0, 50);
    
    // Buttons
    GuiButton btnLoad(200, 48);
    GuiImageData btnImgData(Button_png);
    GuiImage btnImg(&btnImgData);
    btnLoad.SetImage(&btnImg);
    GuiText btnLoadTxt("Load Game");
    btnLoad.SetLabel(&btnLoadTxt);
    
    // Add to window
    mainWindow.Append(&bg);
    mainWindow.Append(&title);
    mainWindow.Append(&btnLoad);
    
    // Main loop
    while (menu == MENU_NONE)
    {
        VIDEO_WaitVSync();
        
        // Update
        mainWindow.Update(&trigA);
        
        if (btnLoad.GetState() == STATE_CLICKED)
        {
            menu = MENU_LOAD_GAME;
        }
        
        // Draw
        mainWindow.Draw();
        
        // Menu_Render() from gx_video.c
    }
    
    return menu;
}
```

#### File Browser Menu
```cpp
int MenuLoadGame()
{
    GuiWindow window(640, 480);
    GuiFileBrowser browser(520, 360);
    browser.SetPath(config.last_dir);
    
    window.Append(&browser);
    
    while (menu == MENU_NONE)
    {
        browser.Update(&trigA);
        
        char* file = browser.GetSelectedFile();
        if (file)
        {
            // Load ROM
            LoadGame(file);
            return MENU_EXIT;
        }
        
        window.Draw();
    }
    
    return menu;
}
```

#### Settings Menu
```cpp
int MenuSettings()
{
    GuiWindow window(640, 480);
    GuiOptionBrowser options(520, 360);
    
    // Build options list
    OptionList opts[] = {
        {"Video Filter", config.filter, {"None", "HQ2X", "Scanlines"}},
        {"Aspect Ratio", config.aspect, {"4:3", "16:9", "Original"}},
        // ... more options
    };
    
    options.SetOptions(opts, sizeof(opts)/sizeof(opts[0]));
    window.Append(&options);
    
    while (menu == MENU_NONE)
    {
        options.Update(&trigA);
        
        int clicked = options.GetClickedOption();
        if (clicked >= 0)
        {
            // Handle option change
        }
        
        window.Draw();
    }
    
    return menu;
}
```

---

## Implementation Strategy

### Phase 5.1: Font Integration (Day 1)
**Morning:**
1. Study existing `gx/font.c` implementation
2. Create C++ wrapper for font functions
3. Update `GUI_DrawText()` to use bitmap fonts

**Afternoon:**
1. Implement `GuiText::Draw()` with real text rendering
2. Test with various font sizes and colors
3. Verify text positioning and alignment

**Deliverable:** Text renders on screen

---

### Phase 5.2: Audio Integration (Day 1-2)
**Tasks:**
1. Study existing ASND implementation in `gx_audio.c`
2. Implement `GuiSound::Play/Stop/Pause()` with ASND
3. Load PCM sound files from `gx/sounds/`
4. Add sound to button hover/click

**Deliverable:** Button sounds working

---

### Phase 5.3: Widget Creation (Day 2)
**Priority Order:**
1. **GuiFileBrowser** (Essential)
   - Port from snes9xGC
   - Integrate with `gx/fileio/` system
   - Test SD card browsing

2. **GuiOptionBrowser** (Essential)
   - Port from snes9xGC
   - Create settings structure
   - Test option navigation

3. **GuiKeyboard** (Optional - Skip if time limited)

**Deliverable:** File browser and option browser working

---

### Phase 5.4: Menu Implementation (Day 3-4)
**Menu Priority:**
1. **Main Menu** (Required)
   - Title screen
   - Load Game button
   - Options button
   - Exit button

2. **Load Game Menu** (Required)
   - File browser
   - Device selection
   - ROM loading

3. **Settings Menu** (Required)
   - Video options
   - Audio options
   - Controller options

4. **Advanced Menus** (If time permits)
   - Save state browser
   - Cheat manager
   - Credits screen

**Deliverable:** Functional menu system

---

## Integration Points

### Existing C Menu System
**Strategy:** Coexist, then replace

1. **Phase 5:** C++ menus available alongside C menus
2. **Testing:** Toggle between old/new menu systems
3. **Phase 6:** Verify C++ menus work correctly
4. **Phase 7:** Remove old C menu code

**Configuration Flag:**
```c
// In config
#define USE_CPP_GUI 1  // Toggle between old/new GUI

#if USE_CPP_GUI
    MenuMain_CPP();
#else
    MainMenu();  // Old C menu
#endif
```

### File I/O Integration
Use existing `gx/fileio/` functions:
```c
// Already available
int OpenDirectory(char *device);
int UpdateDirectory(char *device);
FILEENTRIES *GetFileEntries();
```

### ROM Loading
Use existing functions:
```c
// Already available
int LoadGame(char *filename);
```

---

## File Structure

### New Files to Create
```
gx/gui_cpp/
├── gui_filebrowser.cpp       # File browser widget
├── gui_filebrowser.h
├── gui_optionbrowser.cpp     # Settings browser widget  
├── gui_optionbrowser.h
├── gui_keyboard.cpp          # (Optional) On-screen keyboard
├── gui_keyboard.h
└── menu_cpp.cpp              # C++ menu implementation
    menu_cpp.h
```

### Files to Modify
```
gx/gui_cpp/
├── gui_text.cpp              # Add real text rendering
├── gui_sound.cpp             # Add ASND integration
└── gui_drawing.c             # Add font rendering to GUI_DrawText()

gx/
└── main.c                    # Toggle to C++ menu system
```

---

## Success Criteria

### Phase 5.1: Font Integration ✅
- [ ] Text renders correctly at various sizes
- [ ] Colors and alpha blending work
- [ ] Text alignment functions (left/center/right)
- [ ] Text wrapping for long strings
- [ ] No memory leaks

### Phase 5.2: Audio Integration ✅
- [ ] PCM sounds play correctly
- [ ] Button hover sound works
- [ ] Button click sound works
- [ ] Volume control functional
- [ ] No audio glitches

### Phase 5.3: Widgets ✅
- [ ] GuiFileBrowser lists files
- [ ] Can navigate directories
- [ ] File filtering works (.md, .smd, etc.)
- [ ] GuiOptionBrowser displays settings
- [ ] Can change option values
- [ ] Scroll works correctly

### Phase 5.4: Menus ✅
- [ ] Main menu displays
- [ ] Can navigate to load game
- [ ] File browser works
- [ ] Can load ROM from SD card
- [ ] Settings menu functional
- [ ] Back button returns correctly
- [ ] Menu transitions smooth

---

## Testing Plan

### Unit Testing
```cpp
// Test text rendering
void TestTextRendering()
{
    GuiText text("Test");
    text.SetPosition(100, 100);
    text.SetColor((GXColor){255, 255, 255, 255});
    
    // Should render without crash
    text.Draw();
}

// Test sound
void TestSound()
{
    GuiSound snd(button_over_pcm, button_over_pcm_size, SOUND_PCM);
    snd.Play();
    // Should hear sound
}

// Test file browser
void TestFileBrowser()
{
    GuiFileBrowser fb(400, 300);
    fb.SetPath("sd:/roms");
    fb.Update(&trigA);
    // Should list files
}
```

### Integration Testing
1. Load main menu → Should display
2. Navigate to load game → Should show file browser
3. Select ROM → Should load game
4. Access settings → Should show options
5. Change setting → Should save
6. Press B → Should return to previous menu

### Performance Testing
- Menu rendering at 60 FPS
- No lag during navigation
- Smooth transitions
- Fast file loading

---

## Risk Mitigation

### Risk 1: Font Rendering Complexity
**Mitigation:**
- Use existing bitmap font system (simple)
- Defer FreeTypeGX to Phase 7
- Test early and often

### Risk 2: Audio Conflicts
**Mitigation:**
- Use separate ASND voice channels for GUI
- Don't interfere with game audio
- Test volume mixing

### Risk 3: File Browser Performance
**Mitigation:**
- Paginate file lists (30 items per page)
- Cache directory contents
- Use existing optimized file I/O

### Risk 4: Menu State Management
**Mitigation:**
- Simple state machine (enum-based)
- Clear menu transitions
- Proper cleanup on exit

---

## Dependencies

### Required Libraries (Already Available)
- ✅ libogc (GX, PAD, WPAD)
- ✅ ASND (audio)
- ✅ FatFS (file I/O)
- ✅ libpng (image loading)

### Required Assets (Already Available)
- ✅ UI images (114 PNG files)
- ✅ Sound effects (3 PCM files)
- ✅ Fonts (bitmap font data)

### Required Code (Already Available)
- ✅ File I/O functions
- ✅ Config system
- ✅ Audio system
- ✅ Font system

---

## Timeline Estimate

| Task | Duration | Dependencies |
|------|----------|--------------|
| Font integration | 0.5 days | Phase 4 complete |
| Audio integration | 0.5 days | Font done |
| GuiFileBrowser | 1 day | Audio done |
| GuiOptionBrowser | 0.5 days | FileBrowser done |
| Main Menu | 0.5 days | Widgets done |
| Load Game Menu | 0.5 days | Main menu done |
| Settings Menu | 0.5 days | OptionBrowser done |
| Testing & Polish | 0.5 days | All menus done |

**Total: 4 days**

---

## Phase 5 Deliverables

### Code
- [ ] gui_text.cpp with real rendering
- [ ] gui_sound.cpp with ASND
- [ ] gui_filebrowser.cpp widget
- [ ] gui_optionbrowser.cpp widget
- [ ] menu_cpp.cpp menu system
- [ ] All code builds on GC + Wii

### Documentation
- [ ] PHASE5_SUMMARY.md
- [ ] Update REFACTORING_PROGRESS.md
- [ ] Code comments
- [ ] Integration guide

### Testing
- [ ] All menus functional
- [ ] ROM loading works
- [ ] Settings save correctly
- [ ] No crashes
- [ ] 60 FPS rendering

---

## Next Phase: Phase 6

After Phase 5 completes, Phase 6 will focus on:
- Comprehensive testing
- Bug fixes
- Performance optimization
- Additional widget polish
- Save state browser
- Cheat manager

---

**Document Status:** ✅ Complete  
**Phase 5 Branch:** phase5-menu-implementation  
**Ready to Begin:** ✅ Yes  
**Estimated Duration:** 3-4 days
