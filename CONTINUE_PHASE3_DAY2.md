# Phase 3.2 Day 2: GUI Image & Data Classes - Continuation Prompt

## Context & Current Status

You are continuing the Genesis Plus GX refactoring project, specifically **Phase 3: GUI Foundation**, currently on **Phase 3.2 Day 2**.

### What Has Been Completed

**Phase 3.1: Architecture Planning** ✅
- Created comprehensive `GUI_ARCHITECTURE.md` document
- Analyzed snes9xGC GUI system (13 classes, ~3,700 lines)
- Documented complete class hierarchy and implementation plan
- Defined integration points with existing code

**Phase 3.2 Day 1: Foundation Classes** ✅
- Created `gx/gui_cpp/` directory for new C++ GUI (separate from old C GUI in `gx/gui/`)
- Implemented **gui.h** (140 lines) - Master header with class declarations
- Implemented **gui_element.cpp** (400 lines) - Complete base class with:
  - Position & alignment system
  - Effects (slide, fade, scale)
  - State management
  - Parent/child hierarchy
- Implemented **gui_trigger.cpp** (70 lines) - Controller input abstraction
- Implemented **gui_sound.cpp** (50 lines) - Sound playback wrapper
- Created **Makefile.test** for compilation verification
- **All classes compile cleanly** with powerpc-eabi-g++ (zero warnings)
- Fixed build issue: Restored `gx/gui/gui.h` (old C GUI) which is still needed by existing code

### Repository Structure

```
gx/
├── gui/              # OLD C GUI (keep intact, used by existing code)
│   ├── gui.h         # Original C header (10KB)
│   ├── gui.c
│   ├── menu.c
│   └── ...
│
└── gui_cpp/          # NEW C++ GUI (our work-in-progress)
    ├── gui.h         # New C++ header (4KB)
    ├── gui_element.cpp
    ├── gui_trigger.cpp
    ├── gui_sound.cpp
    └── Makefile.test
```

### Current Branch
- **Branch**: `phase3-gui-foundation`
- **Last Commit**: Fixed build by restoring gx/gui/gui.h
- **CI Status**: All builds passing ✅

---

## Phase 3.2 Day 2: Objectives

Implement image handling classes based on snes9xGC pattern.

### Files to Create Today

1. **gui_imagedata.cpp** (~50 lines)
   - Load PNG image data
   - Store texture information
   - Provide width/height accessors
   - Reference: `~/Repos/snes9xGC/source/gui/gui_imagedata.cpp`

2. **gui_image.cpp** (~200 lines)
   - Display images with GuiElement
   - Handle textures and effects
   - Tile support (horizontal/vertical)
   - Color modulation
   - Reference: `~/Repos/snes9xGC/source/gui/gui_image.cpp`

3. Update **gui.h** with class declarations for:
   - GuiImageData
   - GuiImage

---

## Implementation Guidelines

### Step 1: Examine snes9xGC Reference

```bash
# View GuiImageData implementation
head -100 ~/Repos/snes9xGC/source/gui/gui_imagedata.cpp

# View GuiImage implementation
head -150 ~/Repos/snes9xGC/source/gui/gui_image.cpp

# Extract class definitions from header
sed -n '/class GuiImageData/,/^};/p' ~/Repos/snes9xGC/source/gui/gui.h
sed -n '/class GuiImage/,/^};/p' ~/Repos/snes9xGC/source/gui/gui.h
```

### Step 2: Update gui.h Header

Add to `gx/gui_cpp/gui.h` before the closing `#endif`:

```cpp
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
    void Stripe(int s);
    void ColorStripe(int s);
    
    void Draw();
    
protected:
    int imgType;
    u8 * image;
    GuiImageData * imageData;
    int tile;
    float imageangle;
    int stripe;
    GXColor stripecolor;
};
```

### Step 3: Create gui_imagedata.cpp

Key points:
- Constructor loads PNG data (placeholder for now - actual PNG loading comes later)
- Store width, height, raw data pointer
- Simple getters
- Keep it minimal (~50 lines)

### Step 4: Create gui_image.cpp

Key points:
- Extends GuiElement
- Two constructors: one for image data, one for solid color
- Handle texture/color/data image types
- Tile support for repeating images
- Draw() method renders with GX (placeholder for now)
- Stripe/color stripe effects

### Step 5: Update Makefile.test

Add new source files:
```makefile
SOURCES := gui_element.cpp gui_trigger.cpp gui_sound.cpp \
           gui_imagedata.cpp gui_image.cpp
```

### Step 6: Compile and Test

```bash
cd gx/gui_cpp
make -f Makefile.test clean
make -f Makefile.test test
```

Must compile with **zero warnings**.

---

## Important Notes

### Code Style
- Follow existing pattern from Day 1 classes
- Use `(void)param;` to suppress unused parameter warnings
- Keep headers compact, implementation detailed
- Match snes9xGC structure closely

### Dependencies
- **PNGU**: PNG decoding (will integrate later)
- **GX**: Rendering (placeholder functions for now)
- For now, focus on class structure, not actual rendering

### Compilation Flags
- `-fno-exceptions -fno-rtti` (no C++ overhead)
- `-std=c++11` (modern C++)
- `-O2 -Wall -Wextra -Werror` (strict warnings)

### Testing Strategy
- Verify compilation with devkitPPC (powerpc-eabi-g++)
- Check for warnings (must be zero)
- Verify class structure matches snes9xGC
- Actual rendering tests come later

---

## Success Criteria for Day 2

- [x] GuiImageData class created and compiles
- [x] GuiImage class created and compiles
- [x] gui.h updated with class declarations
- [x] Makefile.test updated and working
- [x] **Zero compilation warnings**
- [x] Code follows snes9xGC patterns
- [x] Progress tracked in REFACTORING_PROGRESS.md

---

## Expected Output

At the end of Day 2, you should have:

1. **gui_imagedata.cpp** (~50 lines)
2. **gui_image.cpp** (~200 lines)
3. **gui.h** updated (add ~30 lines for new classes)
4. **Makefile.test** updated
5. **All compiling cleanly**

Total new code: ~250 lines
Cumulative: ~770 lines (Day 1: 520 + Day 2: 250)

---

## Git Commit Message Template

```
Phase 3.2 Day 2: Implement GUI image classes

Image Handling Complete:

Files Created:
- gui_imagedata.cpp (~50 lines)
  * PNG data loading structure
  * Width/height storage
  * Image data accessor methods
  
- gui_image.cpp (~200 lines)
  * GuiElement-based image display
  * Texture and color modes
  * Tile support (repeat images)
  * Angle/stripe effects
  * Draw() method structure

Updated Files:
- gui.h - Added GuiImageData and GuiImage class declarations
- Makefile.test - Added new source files

Compilation Test:
✅ All classes compile with devkitPPC
✅ Zero warnings
✅ Follows snes9xGC architecture

Next: Day 3 - GuiText, GuiWindow (text rendering & containers)

Progress: Phase 3.2 Day 2/5 Complete
```

---

## Reference Commands

```bash
# Check snes9xGC reference
ls -la ~/Repos/snes9xGC/source/gui/

# View class in header
grep -A 30 "class GuiImageData" ~/Repos/snes9xGC/source/gui/gui.h

# Compile test
cd /Users/caseyjparker/Repos/genplusGC/gx/gui_cpp
make -f Makefile.test clean && make -f Makefile.test test

# Check warnings
cd /Users/caseyjparker/Repos/genplusGC/gx/gui_cpp
make -f Makefile.test test 2>&1 | grep -i warning

# Update progress
vim /Users/caseyjparker/Repos/genplusGC/REFACTORING_PROGRESS.md

# Commit and push
git add gx/gui_cpp/
git commit -m "Phase 3.2 Day 2: Implement GUI image classes"
git push
```

---

## Next Session Preview (Day 3)

After completing Day 2, Day 3 will implement:
- **GuiText** (~350 lines) - Text rendering with FreeTypeGX
- **GuiWindow** (~250 lines) - Container for multiple elements

---

**Ready to begin Phase 3.2 Day 2!**

The foundation (Day 1) is solid. Now we add image handling capabilities to the GUI system. Follow the snes9xGC pattern closely, keep code clean, and ensure everything compiles without warnings.
