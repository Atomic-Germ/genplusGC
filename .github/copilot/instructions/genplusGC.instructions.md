---
applyTo: **
description: Instructions for refactoring Genesis Plus GX into a GameCube/Wii-focused emulator with new GUI system based on snes9xGC and fceuGC architecture
---

# Genesis Plus GX GameCube/Wii Refactoring Project

## Project Overview

This project aims to transform Genesis Plus GX from a multi-platform emulator into a focused GameCube/Wii emulator with an optimized, modern GUI system. The primary goals are:

1. **Remove multi-platform support** - Strip out libretro, SDL, PSP2, GCW0, and all non-GC/Wii code
2. **Build new GUI system** - Replace current custom GUI with C++ object-oriented system based on snes9xGC/fceuGC
3. **GameCube optimization** - Focus on memory efficiency and performance for GameCube hardware
4. **Bug fixes** - Address known emulation and platform-specific issues
5. **Code modernization** - Clean up and refactor legacy code

## Current Architecture Analysis

### Directory Structure
- **core/** (6.7MB) - Emulation core supporting Genesis/MD, Sega/Mega CD, Master System, Game Gear, SG-1000
  - Platform-independent emulation code
  - M68K, Z80 CPU emulators
  - Sound (Tremor/Vorbis, YM2612, PSG)
  - CD hardware emulation
  - Cart hardware (SVP chip, various mappers)
  - NTSC filters
  
- **gx/** (37MB) - GameCube/Wii specific implementation
  - main.c - Entry point and main loop
  - config.c/h - Configuration system
  - gx_audio.c/h - Audio rendering
  - gx_input.c/h - Input handling
  - gx_video.c/h - Video rendering
  - **gui/** - Current GUI system (9,179 lines C code)
    - gui.c/h - Basic GUI rendering
    - menu.c/h - Menu system
    - filesel.c/h - File selector
    - cheats.c/h - Cheat manager
    - font.c/h - Font rendering
  - **fileio/** - File operations
  - **utils/** - Utilities (VI encoder, OGG player, Wii DRC support)
  - **images/** - 114 PNG resources
  - **sounds/** - Audio assets

- **Platforms to Remove:**
  - libretro/ (832KB) - RetroArch port
  - sdl/ (348KB) - SDL1/SDL2 ports
  - psp2/ (304KB) - PS Vita port
  - gcw0/ (484KB) - GCW Zero port

### Build System
- **Makefile.gc** - GameCube build (genplus_cube.dol)
  - Uses DEVKITPPC toolchain
  - Flags: `-O3 -fomit-frame-pointer -DUSE_LIBTREMOR -DUSE_16BPP_RENDERING -DALT_RENDERER`
  - Libraries: png, fat, iso9660, asnd, ogc, m, z
  
- **Makefile.wii** - Wii build (genplus_wii.dol)
  - Additional: `-DHW_RVL -DUSE_LIBCHDR -DMAXROMSIZE=15728640`
  - Extra libs: di, wiiuse, bte
  - Includes CHD/FLAC support for CD images

### Current GUI Architecture (TO BE REPLACED)
The existing GUI is a custom C-based system with:
- Direct GX rendering primitives
- PNG image loading and display
- Basic button/menu navigation
- ~9,700 lines of C code
- Platform-specific #ifdef blocks (HW_RVL/HW_DOL)

### Reference Implementations

**snes9xGC GUI Architecture** (to emulate):
- C++ object-oriented design using libwiigui framework
- ~9,300 lines of menu/GUI code
- Class hierarchy:
  - GuiElement (base class)
  - GuiWindow (window management)
  - GuiButton, GuiImage, GuiText, GuiImageData
  - GuiTrigger (input handling)
  - GuiSound (audio feedback)
  - GuiFileBrowser, GuiSaveBrowser, GuiOptionBrowser
  - GuiKeyboard (on-screen keyboard)
- Files structure:
  - menu.cpp - Main menu logic and flow
  - gui/gui_*.cpp - Individual GUI components
  - gui/gui.h - Central header with all class definitions

**fceuGC** follows identical pattern with same class structure

## Removal Targets

### Directories to Delete
```
/libretro/          # RetroArch/libretro port
/sdl/               # SDL ports
/psp2/              # PS Vita port  
/gcw0/              # GCW Zero handheld port
/builds/            # Pre-built binaries
```

### Files to Delete
```
/Makefile.libretro  # libretro build config
```

### Code Patterns to Remove
- All `#ifdef LIBRETRO` blocks
- Platform abstraction layers for non-GC/Wii
- Multi-platform OSD (on-screen display) code

### Keep for Reference Only
- Wii support should be maintained as secondary target
- HW_RVL defines for Wii-specific features (USB, network, DRC)
- GameCube is primary, Wii is "bonus"

## New GUI System Requirements

### Architecture Pattern (from snes9xGC/fceuGC)

**File Structure:**
```cpp
/gx/gui/
  gui.h                 // Master header with all class definitions
  gui_element.cpp       // Base GuiElement class
  gui_window.cpp        // GuiWindow class  
  gui_button.cpp        // GuiButton class
  gui_image.cpp         // GuiImage class
  gui_imagedata.cpp     // GuiImageData class
  gui_text.cpp          // GuiText class
  gui_trigger.cpp       // GuiTrigger (input) class
  gui_sound.cpp         // GuiSound class
  gui_filebrowser.cpp   // File browser widget
  gui_savebrowser.cpp   // Save state browser
  gui_optionbrowser.cpp // Options menu widget
  gui_keyboard.cpp      // On-screen keyboard

/gx/
  menu.cpp              // Main menu logic and state machine
  menu.h                // Menu enums and prototypes
```

**Core Classes to Implement:**

1. **GuiElement** (base class)
   - Position (x, y), scale, alignment
   - Visibility, focus, selectability states
   - Parent/child relationships
   - Update() and Draw() virtual methods
   - Event callbacks (click, hover, etc.)

2. **GuiWindow** : GuiElement
   - Container for multiple elements
   - Z-order management
   - Focus management

3. **GuiButton** : GuiElement
   - Image, text, and icon support
   - Hover/click/disabled states
   - Sound effects on interaction
   - Multiple trigger support

4. **GuiImage** / **GuiImageData**
   - PNG loading and caching
   - Tiling, scaling, rotation
   - Color modulation

5. **GuiText**
   - FreeTypeGX integration
   - Text wrapping, alignment
   - Multi-line support
   - Dynamic sizing

6. **GuiTrigger**
   - Input abstraction (GC pad, Wiimote, Classic, etc.)
   - Button mapping
   - Analog stick support

**Menu System Flow:**
```
MainMenu()
├── Load Game Menu
│   ├── Recent ROMs
│   ├── Browse SD/USB
│   └── Browse DVD
├── Options Menu
│   ├── Video Settings
│   ├── Audio Settings  
│   ├── Controller Settings
│   └── System Settings
├── Save/Load State
├── Cheats Menu
├── Reset Game
└── Exit to Loader
```

### GUI Component Specifications

**File Browser:**
- Directory navigation with SD/USB/DVD support
- File filtering (.md, .smd, .bin, .gen, .sms, .gg, .sg, .zip)
- Thumbnail/icon display
- Sort options (name, date, size)
- Pagination (25-30 items per page)

**Settings Menus:**
- Tabbed or hierarchical organization
- Option types: toggle, slider, dropdown, button
- Live preview where applicable
- "Reset to defaults" option
- Save confirmation

**Save State Browser:**
- Screenshot thumbnails
- Date/time stamps
- Quick save/load slots (0-9)
- Auto-save option
- Confirm before overwrite

### Integration Points

**Input System (gx_input.c):**
- Map to GuiTrigger system
- Support all controllers: GC pad, Wiimote, Classic, Nunchuk, Wii DRC
- Rumble feedback integration

**Video System (gx_video.c):**
- Provide GX context to GUI rendering
- Handle screen resolution (480i/480p/576i)
- Aspect ratio correction
- Screenshot capture for save states

**Audio System (gx_audio.c):**
- GUI sound effect playback
- Background music support (OGG)
- Volume controls integration

**Config System (config.c/h):**
- Extend t_config struct for new GUI settings
- XML or binary config save/load
- Version migration support

## GameCube Optimization Targets

### Memory Constraints
- GameCube: 24MB main RAM (16MB MEM1 + 8MB locked MEM2)
- Wii: 88MB total (24MB MEM1 + 64MB MEM2)
- Current issues:
  - Large ROM support (MAXROMSIZE=15728640 on Wii only)
  - CD image buffering
  - Multiple texture/image caching

**Optimization Strategies:**
1. Lazy loading for GUI resources
2. Texture compression for UI elements
3. Streaming for large ROMs
4. MEM2 utilization on Wii
5. Config option: GameCube "lite" mode (reduce features for memory)

### Performance Optimization
1. **Rendering:**
   - Pre-compile display lists
   - Batch GX calls
   - Texture atlas for GUI
   - 16-bit rendering mode (already enabled)

2. **Emulation:**
   - Profile CPU usage (M68K, Z80)
   - Optimize memory access patterns
   - Cache-friendly data structures
   - Look for BUSY_LOOP_HACKS opportunities (already in Z80)

3. **Frame Pacing:**
   - Reduce input lag (already improved in v1.7.5)
   - VSync options
   - Audio/video sync tuning

## Known Bugs & Issues to Address

### From HISTORY.txt Analysis:
1. **CD Emulation Issues:**
   - Audio stutters during FMV (Timer/CDD interrupts)
   - Random freezes (CDC emulation - Jeopardy, ESPN)
   - CDD seek command accuracy (Final Fight CD)
   - Word-RAM byte access
   - PRG-RAM access timing

2. **Interrupt Handling:**
   - Level 3 (GFX) interrupt masking
   - H-INT vector in Mode 1
   - Pending interrupts on state load
   - Communication register access (Streets of Rage freeze)

3. **Memory/Mapper Issues:**
   - SRAM bank-switching (regression in 1.7.4)
   - Games >8MB with Game Genie
   - SVP memory handlers (Virtua Racing)
   - I2C EEPROM accuracy

4. **Platform Specific:**
   - Boot ROM loading when switching systems
   - State load/save with CD hardware
   - 4MB ROM support
   - Game Gear LCD ghosting filter

### Code Quality Issues (from grep analysis):
- TODO: Motion sensing for Wiimote (gx_input.c)
- TODO: Wiimote DRC support (wiidrc.c)
- TODO: M68K address error handling (use goto instead of return)
- HACK: HVC latch forced when disabled (lightgun.c)

### Testing Priorities:
1. CD-based games (Sega CD/Mega CD)
2. Large ROM support (>4MB)
3. Save state reliability
4. Memory card compatibility
5. Controller input accuracy
6. Audio/video sync

## Development Workflow

### Phase 1: Platform Cleanup (Week 1)
1. Remove all non-GC/Wii platform directories
2. Strip libretro/SDL code from core
3. Clean up #ifdef blocks
4. Verify GC/Wii builds still work
5. Update Makefiles

### Phase 2: GUI Foundation (Week 2-3)
1. Create new C++ GUI class hierarchy
2. Port base classes from snes9xGC
3. Implement GuiElement, GuiWindow, GuiImage
4. Set up GuiTrigger input system
5. Basic rendering test

### Phase 3: GUI Components (Week 4-5)
1. File browser
2. Settings menus
3. Save state manager
4. On-screen keyboard
5. Cheats interface

### Phase 4: Menu Integration (Week 6)
1. Main menu flow
2. In-game menu (Home button)
3. Configuration persistence
4. Theme/graphics polish

### Phase 5: Optimization (Week 7)
1. Memory profiling
2. Rendering optimization
3. GameCube-specific tuning
4. Performance benchmarking

### Phase 6: Bug Fixes & Testing (Week 8+)
1. Address known emulation bugs
2. CD emulation fixes
3. Input/controller testing
4. Save state validation
5. Regression testing

## Testing Matrix

### Hardware:
- [ ] GameCube (DOL-001, DOL-101)
- [ ] Wii (RVL-001, RVL-101)
- [ ] Swiss/Nintendont loader compatibility

### Input Devices:
- [ ] GameCube Controller
- [ ] Wiimote + Nunchuk
- [ ] Classic Controller
- [ ] Wii U Pro Controller (via DRC)

### Storage:
- [ ] SD Card (Wii/Swiss)
- [ ] USB Drive (Wii)
- [ ] DVD-R (GameCube)
- [ ] Memory Card saves

### Game Compatibility:
- [ ] Genesis/Mega Drive (US/JP/EU)
- [ ] Sega CD / Mega CD (US/JP/EU)
- [ ] Master System
- [ ] Game Gear
- [ ] SG-1000
- [ ] Large ROMs (>4MB)
- [ ] Special mappers (SVP, X-in-1, etc.)

## Code Style Guidelines

### C++ GUI Code:
```cpp
// Follow snes9xGC/fceuGC patterns
class GuiElement {
public:
    GuiElement();
    virtual ~GuiElement();
    virtual void Draw() = 0;
    virtual void Update(GuiTrigger *trigger);
    
protected:
    int x, y;
    int width, height;
    bool visible;
};
```

### Platform Defines:
```c
#ifdef HW_RVL
    // Wii-specific code (USB, network, Wiimote, DRC)
#else  
    // GameCube code (default)
#endif
```

### Memory Management:
```c
// Prefer MEM2 for large buffers on Wii
#ifdef HW_RVL
    void *buffer = memalign(32, size); // MEM2
#else
    void *buffer = malloc(size);        // MEM1
#endif
```

## Performance Targets

### GameCube:
- 60 FPS for Genesis/Master System
- 50 FPS for PAL games
- <3 frames of input lag
- <100ms state save/load
- <1 second ROM load (from SD)

### Wii:
- Same as GameCube plus:
- CD image streaming from USB
- Network ROM loading
- 4x faster state operations (MEM2)

## File Naming Conventions

### GUI Files:
- `gui_*.cpp` - GUI component implementations
- `gui.h` - Master GUI header
- `menu.cpp` - Menu logic
- All lowercase with underscores

### Core Files:
- Keep existing naming (genesis.c, vdp_ctrl.c, etc.)
- `gx_*.c` - GameCube/Wii platform layer

### Assets:
- `*_png.h` - Compiled PNG images (bin2o)
- `*_ogg.h` - Compiled OGG audio
- `*_pcm.h` - Compiled PCM samples

## Documentation Requirements

1. Update README.md with new focus
2. Document GUI class hierarchy (Doxygen)
3. Build instructions for devkitPPC
4. User manual for new menu system
5. Changelog tracking all changes

## Success Criteria

### Functional:
- ✅ Clean build for GameCube and Wii
- ✅ All GUI features working
- ✅ File browser with SD/USB/DVD
- ✅ Save states with thumbnails
- ✅ Configuration persistence
- ✅ All controller inputs working

### Performance:
- ✅ 60 FPS on GameCube (Genesis/SMS)
- ✅ <50MB memory usage on GameCube
- ✅ <3 frames input lag
- ✅ No audio crackling/stuttering

### Quality:
- ✅ No platform-specific crashes
- ✅ Proper error handling
- ✅ Clean code (no warnings)
- ✅ No memory leaks
- ✅ Consistent UI/UX with snes9xGC/fceuGC

## Notes & Considerations

1. **Backward Compatibility:**
   - Config migration from old format
   - Save state compatibility (may break)
   - Memory card data format

2. **Future Enhancements:**
   - Online play (Wii only)
   - IPS/BPS patching
   - Screenshot gallery
   - Achievement tracking
   - Shader/filter support

3. **License Compliance:**
   - Maintain GPL compatibility
   - Credit original authors
   - Document code sources (libwiigui)

4. **Community:**
   - GBAtemp release thread
   - GitHub issue tracking
   - Beta testing program
