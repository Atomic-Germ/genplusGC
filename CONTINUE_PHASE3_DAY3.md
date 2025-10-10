# Phase 3.2 Day 3: GUI Text & Container Classes - Continuation Prompt

## Context & Current Status

You are continuing the Genesis Plus GX refactoring project, specifically **Phase 3: GUI Foundation**, currently on **Phase 3.2 Day 3**.

### What Has Been Completed

**Phase 3.1: Architecture Planning** ✅
- Created comprehensive `GUI_ARCHITECTURE.md` document
- Analyzed snes9xGC GUI system (13 classes, ~3,700 lines)
- Documented complete class hierarchy and implementation plan
- Defined integration points with existing code

**Phase 3.2 Day 1: Foundation Classes** ✅
- Created `gx/gui_cpp/` directory for new C++ GUI (separate from old C GUI in `gx/gui/`)
- Implemented **gui.h** (164 lines) - Master header with class declarations
- Implemented **gui_element.cpp** (400 lines) - Complete base class with:
  - Position & alignment system
  - Effects (slide, fade, scale)
  - State management
  - Parent/child hierarchy
- Implemented **gui_trigger.cpp** (70 lines) - Controller input abstraction
- Implemented **gui_sound.cpp** (50 lines) - Sound playback wrapper
- Created **Makefile.test** for compilation verification
- **All classes compile cleanly** with powerpc-eabi-g++ (zero warnings)

**Phase 3.2 Day 2: Image Classes** ✅
- Implemented **gui_imagedata.cpp** (56 lines) - PNG image data handler
- Implemented **gui_image.cpp** (240 lines) - Image display element with:
  - Texture and color modes
  - Tile support for repeating images
  - Angle rotation and stripe effects
  - Pixel-level manipulation
- Updated **gui.h** (+41 lines) with GuiImageData and GuiImage declarations
- Updated **Makefile.test** with new source files
- **All classes compile cleanly** with zero warnings
- **Cumulative: 1,039 lines** in gui_cpp module

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
    ├── gui.h         # New C++ header (205 lines)
    ├── gui_element.cpp    (400 lines)
    ├── gui_trigger.cpp    (70 lines)
    ├── gui_sound.cpp      (50 lines)
    ├── gui_imagedata.cpp  (56 lines)
    ├── gui_image.cpp      (240 lines)
    └── Makefile.test
```

### Current Branch
- **Branch**: `phase3-gui-foundation`
- **Last Commit**: Phase 3.2 Day 2: Implement GUI image classes
- **CI Status**: All builds passing ✅

---

## Phase 3.2 Day 3: Objectives

Implement text rendering and window container classes based on snes9xGC pattern.

### Files to Create Today

1. **gui_text.cpp** (~350 lines)
   - Text rendering with FreeTypeGX integration
   - Font management and text styling
   - Alignment, wrapping, scrolling
   - Color and effects support
   - Reference: `~/Repos/snes9xGC/source/gui/gui_text.cpp`

2. **gui_window.cpp** (~250 lines)
   - Container for multiple GUI elements
   - Child element management
   - Coordinate transformation
   - Focus handling
   - Draw/update propagation
   - Reference: `~/Repos/snes9xGC/source/gui/gui_window.cpp`

3. Update **gui.h** with class declarations for:
   - GuiText
   - GuiWindow

---

## Implementation Guidelines

### Step 1: Examine snes9xGC Reference

```bash
# View GuiText implementation
head -200 ~/Repos/snes9xGC/source/gui/gui_text.cpp

# View GuiWindow implementation
head -150 ~/Repos/snes9xGC/source/gui/gui_window.cpp

# Extract class definitions from header
sed -n '/class GuiText /,/^};/p' ~/Repos/snes9xGC/source/gui/gui.h
sed -n '/class GuiWindow /,/^};/p' ~/Repos/snes9xGC/source/gui/gui.h
```

### Step 2: Update gui.h Header

Add to `gx/gui_cpp/gui.h` before the closing `#endif`:

```cpp
//! Text rendering element
class GuiText : public GuiElement
{
public:
    GuiText(const char * t, int s, GXColor c);
    GuiText(const char * t, int s, GXColor c, int maxwidth, int wrapmeth);
    ~GuiText();
    void SetText(const char * t);
    void SetPresets(int size, GXColor c, int maxwidth, int wrapmeth, int textalign);
    void SetMaxWidth(int w);
    void SetColor(GXColor c);
    void SetStyle(int s);
    void SetAlignment(int h, int v);
    void SetWrap(bool w, int width);
    void SetScroll(int s);
    char * GetText();
    int GetTextWidth();
    
    void Draw();
    
protected:
    char * text;
    wchar_t * textDyn;
    int size;
    int maxWidth;
    int wrap;
    int textScroll;
    int textScrollPos;
    int textScrollInitialDelay;
    int textScrollDelay;
    GXColor color;
    int style;
    int alignmentHor;
    int alignmentVert;
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
    GuiElement* GetGuiElementAt(u32 i) const;
    u32 GetSize();
    void SetFocus(int f);
    void ChangeFocus(GuiElement* e);
    void ToggleFocus(int t);
    int GetSelected();
    void ResetState();
    void SetState(int s);
    void SetVisible(bool v);
    
    void Draw();
    void Update(GuiTrigger * t);
    
protected:
    std::vector<GuiElement*> _elements;
    GuiElement* _firstElement;
};
```

### Step 3: Create gui_text.cpp

Key points from snes9xGC:
- Constructor initializes text string, size, color
- SetText() updates displayed text (converts to wchar_t internally)
- Text wrapping support with multiple wrap methods
- Text scrolling for long strings
- Style flags (drop shadow, outline, etc.)
- FreeTypeGX integration for rendering (placeholder for now)
- Draw() method renders text with effects

Important implementation notes:
- Store both `char*` and `wchar_t*` versions of text
- Handle text width calculation for wrapping
- Implement scroll animation timing
- Support multiple alignment modes
- Keep FreeTypeGX calls as TODO comments for now

### Step 4: Create gui_window.cpp

Key points from snes9xGC:
- Maintains `std::vector<GuiElement*>` for children
- Append/Insert/Remove methods for child management
- Draw() iterates children and calls their Draw()
- Update() propagates input to visible/active children
- Focus management with first/selected element tracking
- Coordinate transformation (children positioned relative to window)
- State propagation (visible, state changes affect children)

Important implementation notes:
- Use `std::vector<GuiElement*>` for element storage
- Properly handle parent/child relationships
- Transform coordinates when checking input
- Focus cycling through selectable elements
- Don't delete elements in destructor (ownership stays with caller)

### Step 5: Update Makefile.test

Add new source files:
```makefile
SOURCES := gui_element.cpp gui_trigger.cpp gui_sound.cpp \
           gui_imagedata.cpp gui_image.cpp \
           gui_text.cpp gui_window.cpp
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
- Follow existing pattern from Day 1 & 2 classes
- Use `(void)param;` to suppress unused parameter warnings
- Keep headers compact, implementation detailed
- Match snes9xGC structure closely

### Dependencies
- **FreeTypeGX**: Text rendering (will integrate later)
- **std::vector**: For window child element storage (already included)
- **wchar.h**: Wide character support (already included in gui.h)
- For now, focus on class structure, not actual rendering

### Text Rendering Considerations
- Text width calculation will need FreeTypeGX later
- For now, can use placeholder width calculations
- Wrap methods: WRAP_CHAR, WRAP_WORD, WRAP_NONE
- Scroll types: SCROLL_HORIZONTAL, SCROLL_NONE

### Window Management
- Elements are NOT owned by window (no delete in destructor)
- Parent element relationship set via SetParent()
- Focus handling respects selectable flag
- Update order matters (process input before drawing)

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

## Success Criteria for Day 3

- [ ] GuiText class created and compiles
- [ ] GuiWindow class created and compiles
- [ ] gui.h updated with class declarations
- [ ] Makefile.test updated and working
- [ ] **Zero compilation warnings**
- [ ] Code follows snes9xGC patterns
- [ ] Progress tracked in REFACTORING_PROGRESS.md

---

## Expected Output

At the end of Day 3, you should have:

1. **gui_text.cpp** (~350 lines)
2. **gui_window.cpp** (~250 lines)
3. **gui.h** updated (add ~70 lines for new classes)
4. **Makefile.test** updated
5. **All compiling cleanly**

Total new code: ~600 lines
Cumulative: ~1,639 lines (Day 1: 520 + Day 2: 520 + Day 3: 600)

---

## Git Commit Message Template

```
Phase 3.2 Day 3: Implement GUI text and window classes

Text & Container Complete:

Files Created:
- gui_text.cpp (~350 lines)
  * Text rendering with FreeTypeGX structure
  * Font size and color management
  * Text wrapping and scrolling
  * Style support (shadow, outline)
  * Alignment options
  
- gui_window.cpp (~250 lines)
  * Container for multiple elements
  * Child element management (append/remove)
  * Focus handling and cycling
  * Coordinate transformation
  * Draw/Update propagation

Updated Files:
- gui.h - Added GuiText and GuiWindow class declarations
- Makefile.test - Added new source files
- REFACTORING_PROGRESS.md - Updated Phase 3.2 Day 3 completion

Compilation Test:
✅ All classes compile with devkitPPC
✅ Zero warnings
✅ Follows snes9xGC architecture

Next: Day 4 - GuiButton (interactive button widget)

Progress: Phase 3.2 Day 3/5 Complete
```

---

## Reference Commands

```bash
# Check snes9xGC reference
ls -la ~/Repos/snes9xGC/source/gui/

# View class in header
grep -A 50 "class GuiText " ~/Repos/snes9xGC/source/gui/gui.h
grep -A 40 "class GuiWindow " ~/Repos/snes9xGC/source/gui/gui.h

# Compile test
cd /Users/caseyjparker/Repos/genplusGC/gx/gui_cpp
make -f Makefile.test clean && make -f Makefile.test test

# Check warnings
cd /Users/caseyjparker/Repos/genplusGC/gx/gui_cpp
make -f Makefile.test test 2>&1 | grep -i warning

# Line counts
wc -l gui_text.cpp gui_window.cpp gui.h

# Update progress
vim /Users/caseyjparker/Repos/genplusGC/REFACTORING_PROGRESS.md

# Commit and push
git add gx/gui_cpp/
git commit -m "Phase 3.2 Day 3: Implement GUI text and window classes"
git push
```

---

## Next Session Preview (Day 4)

After completing Day 3, Day 4 will implement:
- **GuiButton** (~300 lines) - Interactive button widget with images and text
- Final compilation verification
- Memory leak checks

---

## Key Differences from snes9xGC to Note

1. **Text Rendering**: snes9xGC uses FreeTypeGX directly. For now, we create the structure but leave actual rendering as TODOs.

2. **Wrap Methods**: snes9xGC defines wrap constants. We should add these to gui.h:
   ```cpp
   enum { WRAP_NONE, WRAP_CHAR, WRAP_WORD };
   enum { SCROLL_NONE, SCROLL_HORIZONTAL };
   ```

3. **Text Styles**: Add style flags to gui.h:
   ```cpp
   enum { FTGX_NULL, FTGX_JUSTIFY_LEFT, FTGX_JUSTIFY_CENTER, FTGX_JUSTIFY_RIGHT };
   ```

4. **Window Elements**: Use std::vector (already included) for clean C++11 style.

---

**Ready to begin Phase 3.2 Day 3!**

Days 1 & 2 are complete with solid foundation and image handling. Now we add text rendering and container management to enable complex GUI layouts. Follow the snes9xGC pattern closely, keep code clean, and ensure everything compiles without warnings.
