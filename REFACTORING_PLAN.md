# Genesis Plus GX Comprehensive Refactoring Plan

## Executive Summary

This document outlines a systematic, methodical refactoring plan to transform Genesis Plus GX from a multi-platform emulator into a focused, optimized GameCube/Wii emulator with a modern C++ GUI system based on the proven architectures of snes9xGC and fceuGC.

**Key Objectives:**
1. Remove all non-GC/Wii platform code (~2MB of dead code)
2. Replace 9,179 lines of C-based GUI with C++ object-oriented system
3. Implement unit testing framework following snes9xGC/fceuGC patterns
4. Optimize for GameCube memory constraints (24MB) and performance
5. Address known emulation bugs and issues
6. Maintain CI/CD pipeline for automated building and testing

---

## Phase 1: Foundation & Analysis (Week 1)

### 1.1 Repository Setup and Baseline

**Tasks:**
- [ ] Create feature branch `refactor/phase1-cleanup`
- [ ] Document current build baseline (successful builds for GC/Wii)
- [ ] Run initial CI build to establish baseline
- [ ] Create backup tags for current state
- [ ] Document current file/directory structure

**Verification:**
- Baseline builds succeed for both GameCube and Wii targets
- CI pipeline runs successfully
- All artifacts documented

**Estimated Time:** 1 day

---

### 1.2 Platform Cleanup - Remove Dead Code

**Directories to Remove:**
```bash
rm -rf libretro/          # 832KB - RetroArch/libretro port
rm -rf sdl/               # 348KB - SDL1/SDL2 ports  
rm -rf psp2/              # 304KB - PS Vita port
rm -rf gcw0/              # 484KB - GCW Zero handheld port
rm -rf builds/            # 0B   - Pre-built binaries (empty)
```

**Files to Remove:**
```bash
rm Makefile.libretro
```

**Total Space Saved:** ~2MB of source code

**Code Changes Required:**
1. Search for and remove all `#ifdef LIBRETRO` blocks in core files
2. Remove SDL-specific `#ifdef` blocks
3. Clean up platform abstraction layers for removed platforms
4. Update build system to reference only GC/Wii

**Search Commands:**
```bash
# Find all libretro references
grep -r "LIBRETRO" core/ gx/

# Find all SDL references  
grep -r "SDL" core/ gx/

# Find platform defines
grep -r "#ifdef" core/ | grep -E "(PSP2|GCW0|SDL)"
```

**Verification:**
- [ ] GC/Wii builds still succeed after cleanup
- [ ] No broken includes or undefined symbols
- [ ] CI pipeline passes
- [ ] Binary size reduction confirmed

**Estimated Time:** 2 days

---

### 1.3 Update Documentation

**Files to Update:**
- [ ] `README.md` - Focus on GC/Wii, remove multi-platform references
- [ ] Update build instructions for GC/Wii only
- [ ] Document removed platforms with rationale
- [ ] Update feature list to reflect GC/Wii focus

**New Documentation:**
- [ ] Create `PLATFORMS.md` - Detailed GC/Wii platform information
- [ ] Create `BUILDING.md` - Comprehensive build guide
- [ ] Update `HISTORY.txt` with refactoring notes

**Estimated Time:** 1 day

---

## Phase 2: Unit Testing Infrastructure (Week 2)

### 2.1 Test Framework Setup

**Directory Structure to Create:**
```
tests/
├── framework/
│   └── simple_test.h           # Lightweight test framework (from snes9xGC)
├── mocks/
│   ├── mock_libogc.h           # Mock GameCube/Wii types and functions
│   └── mock_libogc.cpp         # Mock implementations
├── unit/
│   ├── test_main.cpp           # Test entry point
│   └── test_dummy.cpp          # Framework verification tests
├── Makefile                    # Test build system
└── README.md                   # Testing documentation
```

**Files to Create:**

1. **`tests/framework/simple_test.h`** (Copy from snes9xGC)
   - Test registration macros
   - Assertion macros (ASSERT_TRUE, ASSERT_EQ, etc.)
   - TestRunner class
   - TestRegistrar class

2. **`tests/mocks/mock_libogc.h`** (Adapted from snes9xGC/fceuGC)
   - Basic types (u8, u16, u32, s8, s16, s32)
   - GX types and constants
   - VI types (GXRModeObj, video modes)
   - PAD/WPAD button constants
   - Mock function stubs

3. **`tests/mocks/mock_libogc.cpp`**
   - Global object implementations
   - Complex mock functions

4. **`tests/unit/test_main.cpp`**
   ```cpp
   #include "../framework/simple_test.h"
   
   int main() {
       return TestRunner::instance().runTests();
   }
   ```

5. **`tests/unit/test_dummy.cpp`** (Framework verification)
   - Basic assertion tests
   - Framework functionality validation

6. **`tests/Makefile`** (Adapted from snes9xGC)
   - Standard g++ compiler (not DevkitPPC)
   - Auto-discovery of test files
   - Targets: tests, run, clean, rebuild, static-analysis

7. **`tests/README.md`**
   - Testing guide
   - How to write tests
   - How to run tests
   - CI integration notes

**Verification:**
- [ ] Test framework compiles with standard g++
- [ ] Dummy tests run successfully
- [ ] `make run` works in tests directory
- [ ] CI can build and run tests

**Estimated Time:** 2 days

---

### 2.2 CI/CD Pipeline Updates

**Update `.github/workflows/build.yml`:**

Add unit testing job before build jobs:
```yaml
jobs:
  unit-tests:
    name: Unit Tests
    runs-on: ubuntu-latest
    steps:
      - name: Checkout code
        uses: actions/checkout@v4
        
      - name: Install build dependencies
        run: sudo apt-get install -y build-essential g++
        
      - name: Build unit tests
        run: cd tests && make tests
        
      - name: Run unit tests
        run: cd tests && make run
        
      - name: Upload test results
        uses: actions/upload-artifact@v4
        if: always()
        with:
          name: test-results
          path: tests/test-results.txt
          retention-days: 5

  build:
    name: Build
    needs: [unit-tests]  # Don't build if tests fail
    # ... existing build configuration
```

**Create `.github/workflows/test.yml`:**
- Dedicated workflow for testing
- Runs on all pushes and PRs
- Includes static analysis (cppcheck)
- Memory checking (valgrind on Linux)

**Verification:**
- [ ] Tests run in CI before builds
- [ ] Failed tests prevent builds
- [ ] Test results uploaded as artifacts
- [ ] Coverage reports generated (optional)

**Estimated Time:** 1 day

---

### 2.3 Initial Test Coverage

**Priority Test Files to Create:**

1. **`test_config.cpp`** - Configuration system
   - Default value validation
   - Setting bounds checking
   - Config save/load logic

2. **`test_fileop.cpp`** - File operations
   - Device prefix stripping (sd:/, usb:/, dvd:/)
   - Path manipulation
   - File extension validation (.md, .smd, .bin, .gen, etc.)
   - Safe string operations

3. **`test_button_mapping.cpp`** - Controller mapping
   - Controller type validation
   - Button constant validation
   - Mapping logic

4. **`test_video_mode.cpp`** - Video mode selection
   - NTSC/PAL detection
   - Progressive/interlaced selection
   - Resolution validation

**Test Coverage Goals:**
- 50+ initial tests across 4 modules
- Focus on pure logic functions
- No hardware dependencies (fully mocked)
- Fast execution (<1 second total)

**Estimated Time:** 2 days

---

## Phase 3: GUI Foundation - C++ Class Hierarchy (Week 3-4)

### 3.1 GUI Architecture Planning

**Reference Implementation Analysis:**
- snes9xGC: 9,307 lines C++ GUI code across 13 files
- fceuGC: Similar structure, proven pattern
- Genesis Plus GX current: 9,179 lines C code (to be replaced)

**Core Classes to Implement:**

```
GuiElement (base)
├── GuiWindow
├── GuiButton  
├── GuiImage
├── GuiText
└── GuiTooltip (optional)

GuiImageData (image loading)
GuiTrigger (input handling)
GuiSound (audio feedback)

Specialized Widgets:
├── GuiFileBrowser
├── GuiSaveBrowser
├── GuiOptionBrowser
└── GuiKeyboard
```

**File Structure:**
```
gx/gui/
├── gui.h                    # Master header with all class definitions
├── gui_element.cpp          # Base GuiElement class
├── gui_window.cpp           # GuiWindow container class
├── gui_button.cpp           # GuiButton interactive element
├── gui_image.cpp            # GuiImage rendering
├── gui_imagedata.cpp        # GuiImageData loading/caching
├── gui_text.cpp             # GuiText rendering (FreeTypeGX)
├── gui_trigger.cpp          # GuiTrigger input abstraction
├── gui_sound.cpp            # GuiSound audio feedback
├── gui_filebrowser.cpp      # File browser widget
├── gui_savebrowser.cpp      # Save state browser
├── gui_optionbrowser.cpp    # Options menu widget
└── gui_keyboard.cpp         # On-screen keyboard

gx/
├── menu.cpp                 # Main menu logic and state machine
└── menu.h                   # Menu enums and prototypes
```

**Estimated Time:** 1 day planning

---

### 3.2 Base GUI Classes Implementation

**Step 1: Create GUI Master Header**

**File:** `gx/gui/gui.h`

Key contents:
```cpp
#ifndef LIBWIIGUI_H
#define LIBWIIGUI_H

#include <gccore.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <wchar.h>
#include <math.h>
#include <asndlib.h>

#ifdef HW_RVL
#include <wiiuse/wpad.h>
#endif

#include "osd.h"  // Genesis Plus headers
#include "../gx_video.h"
#include "../gx_input.h"
#include "../gx_audio.h"

// Alignment constants
enum {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTRE,
    ALIGN_TOP,
    ALIGN_BOTTOM,
    ALIGN_MIDDLE
};

// State constants
enum {
    STATE_DEFAULT,
    STATE_SELECTED,
    STATE_CLICKED,
    STATE_DISABLED,
    STATE_HELD
};

// Forward declarations
class GuiElement;
class GuiWindow;
class GuiButton;
class GuiImage;
class GuiImageData;
class GuiText;
class GuiTrigger;
class GuiSound;

// Class definitions follow...
```

**Step 2: Implement GuiElement Base Class**

**File:** `gx/gui/gui_element.cpp`

Core functionality:
- Position management (x, y, offset)
- Alignment (horizontal, vertical)
- Visibility, focus, selection states
- Parent/child relationships
- Effect system (slide, fade, scale)
- Virtual Update() and Draw() methods
- Event callbacks

**Reference:** Port directly from `snes9xGC/source/gui/gui_element.cpp`

Key members:
```cpp
class GuiElement {
protected:
    int xoffset, yoffset;
    int xmin, xmax, ymin, ymax;
    int width, height;
    int alpha;
    float xscale, yscale;
    int state;
    int stateChan;
    GuiTrigger* trigger[5];
    GuiElement* parentElement;
    bool rumble;
    bool selectable;
    bool clickable;
    bool holdable;
    bool visible;
    int focus;
    int alignmentVert;
    int alignmentHor;
    
public:
    GuiElement();
    virtual ~GuiElement();
    virtual void Draw() = 0;
    virtual void Update(GuiTrigger* trigger);
    
    // Getters/setters for all properties
    void SetPosition(int x, int y);
    void SetAlignment(int h, int v);
    void SetSize(int w, int h);
    // ... many more
};
```

**Step 3: Implement GuiWindow**

**File:** `gx/gui/gui_window.cpp`

Container class for GUI elements:
- Manages multiple child elements
- Z-order/draw order management  
- Focus management
- Element iteration
- Update/Draw all children

**Step 4: Implement GuiImageData and GuiImage**

**Files:** 
- `gx/gui/gui_imagedata.cpp` - Image loading, PNG decode, caching
- `gx/gui/gui_image.cpp` - Image rendering, tiling, scaling

Key features:
- PNG loading via PNGU
- Texture caching
- GX texture format conversion
- Tiling support
- Color modulation
- Rotation (optional)

**Step 5: Implement GuiTrigger**

**File:** `gx/gui/gui_trigger.cpp`

Input abstraction layer:
- Button mapping (GC pad, Wiimote, Classic, Nunchuk)
- Trigger types (simple, held, button only)
- Integration with existing gx_input.c
- Support for all controller types

**Step 6: Implement GuiText**

**File:** `gx/gui/gui_text.cpp`

Text rendering:
- FreeTypeGX integration (if available) or bitmap fonts
- Text alignment
- Multi-line support
- Text wrapping
- Dynamic sizing
- Color, alpha, scaling

**Step 7: Implement GuiButton**

**File:** `gx/gui/gui_button.cpp`

Interactive button element:
- Multiple states (normal, hover, clicked, disabled)
- Image + text + icon support
- Sound effects on interaction
- Multiple trigger support
- Tooltip support (optional)

**Step 8: Implement GuiSound**

**File:** `gx/gui/gui_sound.cpp`

Audio feedback:
- PCM/OGG playback
- Volume control
- Integration with gx_audio.c
- Sound effect management

**Verification After Each Step:**
- [ ] File compiles without errors
- [ ] Integrates with existing code
- [ ] No memory leaks (check with valgrind in tests)
- [ ] Follows snes9xGC patterns

**Estimated Time:** 5-6 days

---

### 3.3 Build System Updates

**Update `Makefile.gc` and `Makefile.wii`:**

1. Add C++ compiler flags:
```makefile
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
```

2. Add GUI source files:
```makefile
SOURCES := core core/m68k core/z80 ... \
           gx gx/utils gx/gui gx/fileio gx/images gx/sounds
```

3. Enable C++ linking:
```makefile
ifeq ($(strip $(CPPFILES)),)
    export LD := $(CC)
else
    export LD := $(CXX)  # Use C++ linker
endif
```

4. Optimization flags (from appendix):
```makefile
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -s
```

**Verification:**
- [ ] GameCube build succeeds
- [ ] Wii build succeeds
- [ ] Binary size is reasonable (~2-3MB)
- [ ] No linker errors

**Estimated Time:** 1 day

---

## Phase 4: GUI Widgets & Components (Week 5)

### 4.1 File Browser Widget

**File:** `gx/gui/gui_filebrowser.cpp`

**Features:**
- Directory navigation (SD/USB/DVD)
- File filtering by extension:
  - Genesis/MD: .md, .smd, .bin, .gen
  - Sega CD: .cue, .chd, .iso, .cdi
  - Master System: .sms
  - Game Gear: .gg
  - SG-1000: .sg
  - Archives: .zip, .7z
- Pagination (25-30 items per page)
- Scroll with analog stick/D-pad
- Icon/thumbnail display (optional)
- Sort options (name, date, size)
- Device switching (SD ↔ USB ↔ DVD)

**Integration Points:**
- Current `gx/fileio/` system
- File listing functions
- Device detection

**Estimated Time:** 2 days

---

### 4.2 Save State Browser

**File:** `gx/gui/gui_savebrowser.cpp`

**Features:**
- Display save state slots (0-9 + auto-save)
- Screenshot thumbnails
- Date/time stamps
- Quick save/load
- Confirm before overwrite dialog
- Empty slot indication
- Sort by date or slot

**Integration:**
- Save state screenshot capture
- State save/load functions
- Thumbnail generation

**Estimated Time:** 2 days

---

### 4.3 Options Browser

**File:** `gx/gui/gui_optionbrowser.cpp`

**Features:**
- Scrollable option list
- Option types:
  - Toggle (on/off)
  - Slider (range values)
  - Dropdown (multiple choices)
  - Button (action)
- Live preview where applicable
- "Reset to defaults" option
- Category grouping

**Settings Categories:**
- Video (rendering, filters, aspect ratio)
- Audio (volume, sample rate)
- Controllers (mapping, rumble)
- System (region, timing, BIOS)
- Saves (auto-save, memory card)

**Estimated Time:** 2 days

---

### 4.4 On-Screen Keyboard

**File:** `gx/gui/gui_keyboard.cpp`

**Features:**
- QWERTY layout
- Upper/lower case
- Numbers and symbols
- Backspace, space, enter
- Navigate with controller/Wiimote
- Input display area
- Use cases: cheat codes, save names, network settings

**Estimated Time:** 1 day

---

## Phase 5: Menu System Implementation (Week 6)

### 5.1 Menu Architecture Design

**File:** `gx/menu.cpp` + `gx/menu.h`

**Menu State Machine:**

```
Main Menu
├── Load Game
│   ├── Recent ROMs
│   ├── Browse Device (SD/USB/DVD)
│   └── Back
├── Game Settings (when game loaded)
│   ├── Video Settings
│   ├── Audio Settings
│   ├── Controller Settings
│   ├── System Settings
│   └── Back
├── Save/Load State (when game loaded)
│   ├── Save State
│   ├── Load State
│   ├── Screenshot Mode
│   └── Back
├── Cheats (when game loaded)
│   ├── Browse Cheats
│   ├── Add Cheat
│   ├── Edit Cheat
│   └── Back
├── Reset Game (when game loaded)
├── Options
│   ├── Paths
│   ├── Video
│   ├── Audio
│   ├── Controllers
│   ├── Network (Wii only)
│   └── Back
├── Credits
└── Exit to Loader
```

**Menu Flow Functions:**
```cpp
int MenuMain();
int MenuLoadGame();
int MenuGameSettings();
int MenuSaveLoadState();
int MenuCheats();
int MenuOptions();
int MenuControllers();
void MenuReset();
```

**Global State:**
```cpp
static GuiWindow* mainWindow = NULL;
static GuiSound* bgMusic = NULL;
static GuiSound* enterSound = NULL;
static GuiSound* exitSound = NULL;
static GuiImageData* gameScreen = NULL;

static int currentMenu = MENU_NONE;
static bool guiHalt = false;
```

**Estimated Time:** 3 days

---

### 5.2 Menu Integration with Emulation

**Files to Update:**
- `gx/main.c` - Main loop integration
- `gx/gx_input.c` - Menu trigger (Home button)
- `gx/gx_video.c` - Screen capture for save states
- `gx/config.c` - Menu settings persistence

**Home Button Menu (In-Game):**
- Pause emulation
- Display menu overlay
- Resume game
- Save state quick save
- Return to main menu
- Reset game
- Exit

**Integration Tasks:**
1. Add menu trigger in input handler
2. Pause emulation when menu active
3. Resume emulation when menu closed
4. Screenshot capture for save states
5. Settings hot-reload

**Estimated Time:** 2 days

---

### 5.3 Asset Integration

**Current Assets:**
- `gx/images/*.png` - 114 PNG files
- `gx/sounds/*.pcm` - Sound effects
- `gx/sounds/*.ogg` - Background music

**Tasks:**
1. Review and consolidate images
2. Convert to optimized formats (from appendix):
   - Consider pre-converted TPL textures
   - RGB565 swizzled for GX
   - Reduce PNG decode overhead
3. Build-time asset conversion via bin2o
4. Memory-efficient loading (lazy load, MEM2 on Wii)

**Asset Optimization (from appendix):**
- Preconvert to GPU-ready formats at build time
- Remove runtime PNG decode where possible
- Use texture atlas for small UI elements
- Compress with optimal settings

**Estimated Time:** 1 day

---

## Phase 6: Testing & Quality Assurance (Week 7)

### 6.1 Expand Unit Test Coverage

**New Test Files:**

1. **`test_menu.cpp`** - Menu logic tests
   - Menu state transitions
   - Setting validation
   - Menu item visibility rules

2. **`test_save_state.cpp`** - Save state tests
   - Slot management
   - Filename generation
   - Thumbnail handling

3. **`test_cheat.cpp`** - Cheat system tests
   - Code parsing
   - Code validation
   - Enable/disable logic

4. **`test_gui_element.cpp`** - GUI base class tests
   - Position calculations
   - Alignment logic
   - Effect system
   - State management

5. **`test_gui_button.cpp`** - Button tests
   - State transitions
   - Event handling
   - Trigger responses

**Target Coverage:**
- 150+ total tests
- All new GUI classes covered
- Edge cases tested
- Memory safety validated

**Estimated Time:** 3 days

---

### 6.2 Integration Testing

**Manual Test Plan:**

**Hardware Testing:**
- [ ] GameCube (DOL-001, DOL-101)
- [ ] Wii (RVL-001)
- [ ] Swiss loader
- [ ] Homebrew Channel

**Controller Testing:**
- [ ] GameCube Controller
- [ ] Wiimote
- [ ] Wiimote + Nunchuk
- [ ] Classic Controller
- [ ] Wii U Pro Controller

**Storage Testing:**
- [ ] SD Card (FAT32, exFAT)
- [ ] USB Drive (FAT32)
- [ ] DVD-R (GameCube)
- [ ] Memory Card saves

**Game Compatibility Testing:**
- [ ] Genesis/Mega Drive (US/JP/EU)
- [ ] Sega CD (US/JP/EU)
- [ ] Master System
- [ ] Game Gear
- [ ] SG-1000
- [ ] Large ROMs (>4MB)
- [ ] SVP chip (Virtua Racing)
- [ ] CD games with FMV

**Feature Testing:**
- [ ] ROM loading from all devices
- [ ] Save states (save/load/screenshots)
- [ ] Memory card saves (SRAM, EEPROM)
- [ ] Cheats (Game Genie, Action Replay)
- [ ] Video settings (filters, aspect ratio)
- [ ] Audio settings (volume, sample rate)
- [ ] Controller mapping (all types)
- [ ] Reset game
- [ ] Exit to loader

**Performance Testing:**
- [ ] 60 FPS Genesis/MD
- [ ] 50 FPS PAL games
- [ ] Input lag measurement (<3 frames)
- [ ] Audio sync (no crackling)
- [ ] Fast save state (<100ms)
- [ ] Fast ROM load (<1s from SD)

**Estimated Time:** 3 days

---

### 6.3 Bug Fixing

**Known Issues to Address** (from instructions):

**CD Emulation:**
- [ ] Audio stutters during FMV
- [ ] Random freezes (CDC emulation)
- [ ] CDD seek command accuracy
- [ ] Word-RAM byte access
- [ ] PRG-RAM timing

**Interrupt Handling:**
- [ ] Level 3 interrupt masking
- [ ] H-INT vector in Mode 1
- [ ] Pending interrupts on state load
- [ ] Communication register freeze

**Memory/Mapper:**
- [ ] SRAM bank-switching
- [ ] Games >8MB with Game Genie
- [ ] SVP memory handlers
- [ ] I2C EEPROM accuracy

**Platform Specific:**
- [ ] Boot ROM loading
- [ ] State load/save with CD
- [ ] 4MB ROM support
- [ ] Game Gear LCD filter

**TODOs from Code:**
- [ ] Motion sensing for Wiimote
- [ ] Wiimote DRC support
- [ ] M68K address error (use goto)
- [ ] HVC latch hack

**Estimated Time:** Ongoing (2+ days)

---

## Phase 7: Optimization (Week 8)

### 7.1 Memory Optimization

**GameCube Constraints:**
- 24MB main RAM (16MB MEM1 + 8MB locked)
- Need to fit: emulator + GUI + ROM + save states

**Optimization Strategies:**

1. **Lazy Loading for GUI Resources**
   - Load images on-demand
   - Unload when not visible
   - Cache frequently used only

2. **Texture Compression**
   - Use RGB565 instead of RGBA8888 where possible
   - Compress UI elements
   - Texture atlas for small sprites

3. **ROM Streaming** (for large ROMs)
   - Stream from storage for >4MB ROMs
   - Cache hot sections only
   - Option for full load vs streaming

4. **MEM2 Utilization** (Wii only)
   ```c
   #ifdef HW_RVL
       void *buffer = memalign(32, size); // MEM2
   #else
       void *buffer = malloc(size);        // MEM1
   #endif
   ```

5. **Config Option: "Lite Mode"**
   - Reduce GUI effects
   - Disable thumbnails
   - Smaller texture cache
   - Optimize for GC

**Memory Profiling:**
- Track allocation sizes
- Identify large buffers
- Monitor peak usage
- Test on GameCube hardware

**Estimated Time:** 2 days

---

### 7.2 Rendering Optimization

**From Appendix - Video Path Improvements:**

1. **Display Lists for Static UI**
   - Pre-compile menu quads
   - Update matrices only
   - Reduce per-frame setup

2. **Texture Atlas**
   - Combine small UI elements
   - Reduce texture binds
   - Better cache utilization

3. **Batch GX Calls**
   - Group similar primitives
   - Minimize state changes
   - Use GX efficiently

4. **NTSC Filter Optimization**
   - Default to disabled on GC
   - Fixed-point conversion
   - LUT-based approach
   - Make it optional toggle

5. **Frame Buffer Management**
   - 32-byte alignment
   - MEM2 for large buffers (Wii)
   - Flush/invalidate once per frame
   - EFB-scaled copy optimization

**Code Changes:**
```c
// In gx_video.c
#ifndef HW_RVL
    config.ntsc = 0;  // Default off on GameCube
#endif

// Pre-compiled display lists
static GX_DisplayList menuQuadDL = NULL;

void InitMenuDisplayLists() {
    // Create static display lists for menu backgrounds
    GX_BeginDisplayList(dlBuffer, DL_SIZE);
    // ... GX calls
    menuQuadDL = GX_EndDisplayList();
}
```

**Estimated Time:** 2 days

---

### 7.3 Build Optimization

**From Appendix - Build and Link Optimizations:**

**Makefile Changes:**

```makefile
# Link-time optimization
CFLAGS += -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables
LDFLAGS += -Wl,--gc-sections -s

# C++ specific (GUI files)
CXXFLAGS += -fno-exceptions -fno-rtti -fvisibility=hidden

# Optional LTO (if supported)
# CFLAGS += -flto
# LDFLAGS += -flto

# PPC tuning
CFLAGS += -mcpu=750 -mhard-float -frename-registers

# Release vs Debug profiles
ifdef DEBUG
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O3 -fomit-frame-pointer
    LDFLAGS += -s
endif
```

**Expected Results:**
- 10-20% binary size reduction
- Faster link times
- Better dead code elimination
- Smaller .dol file

**Verification:**
- [ ] Build succeeds with new flags
- [ ] Binary size reduced
- [ ] Performance maintained or improved
- [ ] No new bugs introduced

**Estimated Time:** 1 day

---

### 7.4 Audio Optimization

**From Appendix - Audio Path Improvements:**

1. **Adaptive Buffer Size**
   ```c
   // Dynamic SOUND_BUFFER_NUM
   #define SOUND_BUFFER_NUM_MIN 2
   #define SOUND_BUFFER_NUM_MAX 4
   
   static int soundBufferNum = 3; // Default
   
   void AdaptiveAudioBuffer() {
       if (underruns > threshold) {
           soundBufferNum = min(soundBufferNum + 1, SOUND_BUFFER_NUM_MAX);
       }
   }
   ```

2. **Tighter Rate Control**
   - Smaller drift steps
   - VSync locking
   - Reduce pitch modulation

3. **MEM2 Audio Buffers** (Wii)
   - Large work buffers in MEM2
   - DMA-aligned
   - Batch cache operations

4. **Direct Buffer Writing**
   - Avoid memcpy where possible
   - Write directly to DMA buffers
   - Reduce cache flushes

**Estimated Time:** 1 day

---

## Phase 8: Documentation & Polish (Week 9)

### 8.1 Code Documentation

**Doxygen Setup:**

1. Create `Doxyfile`:
```
PROJECT_NAME = "Genesis Plus GX"
INPUT = gx/gui gx core
EXTRACT_ALL = YES
GENERATE_HTML = YES
GENERATE_LATEX = NO
```

2. Document all GUI classes:
```cpp
/**
 * @class GuiElement
 * @brief Base class for all GUI elements
 * 
 * GuiElement provides common functionality for all GUI components including
 * position, size, alignment, visibility, and event handling.
 */
class GuiElement {
    /**
     * @brief Set element position
     * @param x X coordinate
     * @param y Y coordinate
     */
    void SetPosition(int x, int y);
};
```

3. Generate documentation:
```bash
doxygen Doxyfile
```

**Estimated Time:** 2 days

---

### 8.2 User Documentation

**Create User Manual:**

**File:** `docs/USER_MANUAL.md`

Contents:
1. **Getting Started**
   - Installation
   - First launch
   - Loading ROMs

2. **Menu System**
   - Navigation
   - All menu options explained
   - Screenshots

3. **Controller Setup**
   - Supported controllers
   - Button mapping
   - Rumble configuration

4. **Advanced Features**
   - Save states
   - Cheats
   - CD games
   - Network (Wii)

5. **Troubleshooting**
   - Common issues
   - Error messages
   - Performance tips

6. **FAQ**

**Update README.md:**
- Remove multi-platform info
- Focus on GC/Wii features
- Add screenshots
- Link to user manual
- Build instructions
- Contributing guidelines

**Estimated Time:** 2 days

---

### 8.3 Build & Release Documentation

**Create `BUILDING.md`:**

```markdown
# Building Genesis Plus GX

## Prerequisites

- devkitPPC r41 or later
- libogc 2.3.1 or later
- GameCube tools

## Building for Wii

make -f Makefile.wii

## Building for GameCube

make -f Makefile.gc

## Build Options

DEBUG=1       - Debug build with symbols
PROFILE=1     - Profiling build
STRIP=0       - Don't strip symbols

## Output

- genplus_wii.dol
- genplus_cube.dol
```

**Create `CHANGELOG.md`:**
- Document all changes
- Breaking changes
- New features
- Bug fixes
- Known issues

**Estimated Time:** 1 day

---

### 8.4 Final Testing & Release Prep

**Pre-Release Checklist:**

**Code Quality:**
- [ ] All compiler warnings resolved
- [ ] No memory leaks (valgrind clean)
- [ ] Static analysis clean (cppcheck)
- [ ] Code formatted consistently
- [ ] All TODOs addressed or documented

**Testing:**
- [ ] All unit tests pass
- [ ] CI builds pass
- [ ] Manual testing complete
- [ ] Regression testing done
- [ ] Performance benchmarks met

**Documentation:**
- [ ] README updated
- [ ] User manual complete
- [ ] Build instructions tested
- [ ] Changelog updated
- [ ] Code documented (Doxygen)

**Release Artifacts:**
- [ ] Wii build (.dol + HBC files)
- [ ] GameCube build (.dol)
- [ ] Source code archive
- [ ] Documentation bundle
- [ ] Sample ROMs/saves (if legal)

**GitHub Release:**
- [ ] Tag version (v2.0.0)
- [ ] Release notes
- [ ] Upload artifacts
- [ ] Update Pre-release

**Estimated Time:** 2 days

---

## Phase 9: Post-Release (Ongoing)

### 9.1 Community Feedback

**Channels:**
- GitHub Issues
- GBAtemp thread
- Reddit (r/EmulationOnSwitch, r/WiiHacks)
- Discord servers

**Response Plan:**
- Triage issues (bug/feature/question)
- Label and prioritize
- Assign milestones
- Regular updates

---

### 9.2 Future Enhancements

**Potential Features:**

1. **Online Play** (Wii)
   - Netplay for 2-player games
   - Lobby system
   - NAT traversal

2. **IPS/BPS Patching**
   - ROM translation support
   - Hack support
   - Auto-patching

3. **Screenshot Gallery**
   - In-game screenshots
   - Gallery browser
   - Export to SD

4. **Achievement Tracking**
   - Retroachievements integration
   - Custom achievements
   - Progress tracking

5. **Shader Support**
   - CRT filters
   - Scanlines
   - Color correction

6. **Save State Rewinding**
   - Rewind gameplay
   - Frame-by-frame

---

## Appendix A: Risk Mitigation

### Identified Risks

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| GUI rewrite introduces bugs | High | Medium | Extensive testing, gradual rollout |
| Memory constraints on GC | High | High | Profiling, optimization, lite mode |
| CI pipeline breaks | Medium | Low | Test locally first, backup configs |
| Build system issues | Medium | Medium | Keep old Makefiles as reference |
| Performance regression | High | Medium | Benchmarking, profiling tools |
| Incompatible with loaders | Medium | Low | Test with Swiss, HBC early |

---

## Appendix B: Resource Estimates

### Total Time Estimate: 9 weeks

**Breakdown:**
- Phase 1 (Cleanup): 4 days
- Phase 2 (Testing): 5 days  
- Phase 3 (GUI Base): 7 days
- Phase 4 (Widgets): 7 days
- Phase 5 (Menu): 5 days
- Phase 6 (Testing): 6 days
- Phase 7 (Optimization): 6 days
- Phase 8 (Docs): 5 days
- Phase 9 (Release): 2 days

**Total:** ~47 working days (~9.5 weeks at 5 days/week)

### Lines of Code Estimate

**New Code:**
- GUI system: ~9,000 lines C++
- Menu system: ~2,000 lines C++
- Unit tests: ~2,000 lines C++
- **Total new:** ~13,000 lines

**Removed Code:**
- Old GUI: ~9,179 lines C
- Platform code: ~2,000 lines C
- **Total removed:** ~11,179 lines

**Net Change:** +1,821 lines (but better architecture)

---

## Appendix C: Success Metrics

### Quantitative Metrics

**Performance:**
- [ ] 60 FPS Genesis/MD (100% games)
- [ ] <3 frames input lag
- [ ] <100ms save state operations
- [ ] <1 second ROM load from SD
- [ ] <50MB RAM usage on GC

**Quality:**
- [ ] 0 compiler warnings
- [ ] 0 memory leaks (valgrind)
- [ ] 150+ unit tests passing
- [ ] 95%+ test pass rate in CI

**Size:**
- [ ] <2MB .dol file (GC)
- [ ] <3MB .dol file (Wii)
- [ ] <100KB config file

### Qualitative Metrics

**User Experience:**
- Intuitive menu navigation
- Fast, responsive UI
- Consistent with snes9xGC/fceuGC
- No crashes or hangs
- Clear error messages

**Code Quality:**
- Well-documented
- Consistent style
- Modular design
- Easy to extend
- No technical debt

---

## Appendix D: Testing Checklist

### Unit Tests (Automated)

- [ ] Config system (defaults, bounds, save/load)
- [ ] File operations (paths, extensions, devices)
- [ ] Button mapping (controllers, buttons, combinations)
- [ ] Video modes (NTSC/PAL, progressive, resolution)
- [ ] Menu logic (state transitions, validation)
- [ ] Save states (slots, thumbnails, filenames)
- [ ] Cheats (parsing, validation, enable/disable)
- [ ] GUI elements (position, alignment, effects)
- [ ] GUI buttons (states, events, triggers)

### Integration Tests (Manual)

**ROM Loading:**
- [ ] Genesis/MD (.md, .smd, .bin, .gen)
- [ ] Sega CD (.cue, .chd, .iso)
- [ ] Master System (.sms)
- [ ] Game Gear (.gg)
- [ ] SG-1000 (.sg)
- [ ] ZIP archives
- [ ] From SD card
- [ ] From USB drive (Wii)
- [ ] From DVD (GC/Wii)

**Save States:**
- [ ] Save to slot 0-9
- [ ] Load from slot 0-9
- [ ] Auto-save on exit
- [ ] Screenshot capture
- [ ] Thumbnail display
- [ ] Overwrite confirmation
- [ ] Date/time stamps

**Cheats:**
- [ ] Game Genie codes
- [ ] Action Replay codes
- [ ] Enable/disable
- [ ] Multiple cheats
- [ ] Save cheat list
- [ ] Load cheat list

**Controllers:**
- [ ] GC pad (all buttons)
- [ ] Wiimote (horizontal)
- [ ] Wiimote + Nunchuk
- [ ] Classic Controller
- [ ] Wii U Pro Controller
- [ ] Rumble
- [ ] Custom mapping
- [ ] Save mapping

**Video:**
- [ ] NTSC 480i
- [ ] NTSC 480p (Wii)
- [ ] PAL 576i
- [ ] PAL 480p (Wii)
- [ ] Aspect ratio (4:3, 16:9)
- [ ] Filters (none, soft, sharp)
- [ ] NTSC filter (optional)
- [ ] Scanlines (optional)

**Audio:**
- [ ] Volume control
- [ ] Sample rate (48kHz)
- [ ] No crackling
- [ ] Sync with video
- [ ] Menu sounds
- [ ] Background music

**Memory Cards:**
- [ ] Save SRAM
- [ ] Load SRAM
- [ ] Save EEPROM
- [ ] Format card (if needed)
- [ ] Multiple saves

**UI/UX:**
- [ ] Menu navigation (all screens)
- [ ] File browser (all devices)
- [ ] Settings (all options)
- [ ] Help/tooltips
- [ ] Error messages
- [ ] Confirmation dialogs
- [ ] Progress bars
- [ ] Screenshots

### Compatibility Tests

**Test Games (suggested):**
- Sonic the Hedgehog (Genesis)
- Sonic CD (Sega CD)
- Virtua Racing (Genesis + SVP)
- Phantasy Star (Master System)
- Streets of Rage 2 (Genesis)
- Final Fantasy (PAL)
- Large ROM (>4MB)
- CD game with FMV

---

## Appendix E: Code Migration Checklist

### Old GUI → New GUI Migration

**Files to Remove:**
- [ ] `gx/gui/gui.c`
- [ ] `gx/gui/gui.h`
- [ ] `gx/gui/menu.c`
- [ ] `gx/gui/menu.h`
- [ ] `gx/gui/filesel.c`
- [ ] `gx/gui/filesel.h`
- [ ] `gx/gui/cheats.c`
- [ ] `gx/gui/cheats.h`
- [ ] `gx/gui/font.c`
- [ ] `gx/gui/font.h`
- [ ] `gx/gui/legal.c`

**Files to Create:**
- [ ] `gx/gui/gui.h` (new C++)
- [ ] `gx/gui/gui_element.cpp`
- [ ] `gx/gui/gui_window.cpp`
- [ ] `gx/gui/gui_button.cpp`
- [ ] `gx/gui/gui_image.cpp`
- [ ] `gx/gui/gui_imagedata.cpp`
- [ ] `gx/gui/gui_text.cpp`
- [ ] `gx/gui/gui_trigger.cpp`
- [ ] `gx/gui/gui_sound.cpp`
- [ ] `gx/gui/gui_filebrowser.cpp`
- [ ] `gx/gui/gui_savebrowser.cpp`
- [ ] `gx/gui/gui_optionbrowser.cpp`
- [ ] `gx/gui/gui_keyboard.cpp`
- [ ] `gx/menu.cpp` (new C++)
- [ ] `gx/menu.h` (new C++)

**Migration Strategy:**
1. Keep old files during development
2. Build new GUI alongside (no conflicts)
3. Switch main.c to use new GUI when ready
4. Test extensively
5. Remove old GUI files
6. Clean up any remaining references

---

## Appendix F: Reference Implementation Comparison

### Code Comparison: snes9xGC vs genplusGC

| Aspect | snes9xGC | genplusGC (Current) | genplusGC (Target) |
|--------|----------|---------------------|-------------------|
| GUI Language | C++ | C | C++ |
| GUI Framework | libwiigui | Custom | libwiigui (adapted) |
| GUI Lines | 9,307 | 9,179 | ~9,000 |
| Menu System | Object-oriented | Procedural | Object-oriented |
| File Browser | GuiFileBrowser | Custom | GuiFileBrowser |
| Save Browser | GuiSaveBrowser | Custom | GuiSaveBrowser |
| Options | GuiOptionBrowser | Custom | GuiOptionBrowser |
| Input | GuiTrigger | Direct PAD/WPAD | GuiTrigger |
| Unit Tests | Yes (77 tests) | No | Yes (150+ tests) |
| CI/CD | GitHub Actions | GitHub Actions | GitHub Actions |
| Build Time | ~2 min | ~2 min | ~2 min |
| Binary Size (Wii) | ~2.5MB | ~2.8MB | ~2.5MB |

---

## Appendix G: Tools & Resources

### Required Tools

**Development:**
- devkitPPC r41+
- devkitPro libraries
- libogc 2.3.1+
- gamecube-tools

**Testing:**
- g++ 7.0+ (for unit tests)
- make
- git
- valgrind (optional, Linux)
- cppcheck (optional)

**Documentation:**
- Doxygen (optional)
- Markdown viewer
- Text editor with C++ support

### Useful Resources

**Documentation:**
- devkitPro: https://devkitpro.org/
- libogc docs: https://libogc.devkitpro.org/
- GX Programming: https://www.gc-forever.com/

**Reference Code:**
- snes9xGC: https://github.com/Atomic-Germ/snes9xGC
- fceuGC: https://github.com/Atomic-Germ/fceuGC
- libwiigui: http://code.google.com/p/libwiigui/

**Testing:**
- Swiss: https://github.com/emukidid/swiss-gc
- Homebrew Channel: https://wiibrew.org/wiki/Homebrew_Channel
- Dolphin Emulator: https://dolphin-emu.org/ (for testing)

---

## Conclusion

This comprehensive refactoring plan provides a systematic, methodical approach to transforming Genesis Plus GX into a focused, optimized GameCube/Wii emulator with modern C++ architecture. By following the proven patterns of snes9xGC and fceuGC, implementing robust unit testing, and optimizing for the target hardware, we will create a high-quality emulator that is maintainable, performant, and user-friendly.

The 9-week timeline is realistic with dedicated effort, and the phased approach allows for incremental progress with validation at each step. The extensive testing and documentation ensure long-term sustainability and community contribution.

All work will be validated through the GitHub CI pipeline, with no requirement for local building during development, as specified in the requirements.

---

**Last Updated:** 2024
**Version:** 1.0
**Status:** Ready for Implementation
