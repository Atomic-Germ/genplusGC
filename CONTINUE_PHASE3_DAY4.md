# Phase 3.2 Day 4: GUI Button Class - Continuation Prompt

## Context & Current Status

You are continuing the Genesis Plus GX refactoring project, specifically **Phase 3: GUI Foundation**, currently on **Phase 3.2 Day 4**.

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

**Phase 3.2 Day 3: Text & Window Classes** ✅
- Implemented **gui_text.cpp** (305 lines) - Text rendering element with:
  - Font size and color management
  - Text wrapping and scrolling support
  - Style support (FTGX alignment flags)
  - Alignment options (horizontal/vertical)
  - Preset system for consistent styling
  - ASCII to wchar_t conversion (placeholder for full i18n)
- Implemented **gui_window.cpp** (253 lines) - Window container with:
  - Child element management (append/insert/remove)
  - Focus handling and selection tracking
  - Draw/Update propagation to children
  - State management across elements
  - std::vector-based element storage
- Updated **gui.h** (+80 lines) with GuiText, GuiWindow, and text constants
- Updated **Makefile.test** with new source files
- **All classes compile cleanly** with zero warnings
- **Cumulative: 1,681 lines** in gui_cpp module

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
    ├── gui.h         # New C++ header (289 lines)
    ├── gui_element.cpp    (400 lines)
    ├── gui_trigger.cpp    (70 lines)
    ├── gui_sound.cpp      (50 lines)
    ├── gui_imagedata.cpp  (56 lines)
    ├── gui_image.cpp      (240 lines)
    ├── gui_text.cpp       (305 lines) ✨
    ├── gui_window.cpp     (253 lines) ✨
    └── Makefile.test
```

### Current Branch
- **Branch**: `phase3-gui-foundation`
- **Last Commit**: Phase 3.2 Day 3: Implement GUI text and window classes
- **Commit Hash**: 8d122de
- **CI Status**: All builds passing ✅

---

## Phase 3.2 Day 4: Objectives

Implement the interactive button widget - the most commonly used GUI element that combines images, text, sounds, and triggers into a clickable component.

### Files to Create Today

1. **gui_button.cpp** (~300 lines)
   - Interactive button widget
   - Combines GuiImage and GuiText
   - Multiple images (default, over, hold, click)
   - Sound effects for hover/click
   - Trigger-based interaction
   - State-based appearance
   - Label positioning and styling
   - Reference: `~/Repos/snes9xGC/source/gui/gui_button.cpp`

2. Update **gui.h** with class declaration for:
   - GuiButton

---

## Implementation Guidelines

### Step 1: Examine snes9xGC Reference

```bash
# View GuiButton implementation
head -250 ~/Repos/snes9xGC/source/gui/gui_button.cpp

# Extract class definition from header
sed -n '/class GuiButton /,/^};/p' ~/Repos/snes9xGC/source/gui/gui.h

# Study the constructor patterns
grep -A 10 "GuiButton::" ~/Repos/snes9xGC/source/gui/gui_button.cpp | head -40
```

### Step 2: Update gui.h Header

Add to `gx/gui_cpp/gui.h` before the closing `#endif`:

```cpp
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
```

### Step 3: Create gui_button.cpp

Key points from snes9xGC:
- Constructor initializes all pointers to NULL
- Simple width/height constructor (most common)
- SetImage* methods for different states (default, over, hold, click)
- SetLabel* methods for button text in different states
- SetSound* methods for audio feedback
- SetIcon* methods for small icons on buttons
- Update() handles trigger detection and state changes
- Draw() renders appropriate image/text based on state
- Proper state transitions (default → over → held → clicked)

Important implementation notes:
- Initialize all image/text/sound pointers to NULL
- Button does NOT own the child elements (no delete in destructor)
- Check state to determine which image/text to show
- Play sounds on state transitions
- Check triggers for hover/click detection
- Handle button alignment for icon and label positioning
- Support multiple trigger types (simple, held, etc.)

### Step 4: Implementation Structure

```cpp
GuiButton::GuiButton(int w, int h)
{
    width = w;
    height = h;
    
    // Initialize all pointers to NULL
    image = NULL;
    imageOver = NULL;
    imageHold = NULL;
    imageClick = NULL;
    icon = NULL;
    iconOver = NULL;
    label = NULL;
    labelOver = NULL;
    soundOver = NULL;
    soundHold = NULL;
    soundClick = NULL;
    
    // Make buttons selectable and clickable by default
    selectable = true;
    clickable = true;
}
```

### Step 5: Update() Method Pattern

The Update() method should:
1. Call base class Update(t)
2. Check if button is visible and trigger is valid
3. Detect cursor/pointer position (IsInside check)
4. Update state based on position and button presses
5. Play sounds on state transitions
6. Handle state: DEFAULT → SELECTED (hover) → HELD → CLICKED

```cpp
void GuiButton::Update(GuiTrigger * t)
{
    if(!t || state == STATE_DISABLED || !this->IsVisible())
        return;
    
    // Base class update
    GuiElement::Update(t);
    
    // Check if inside button bounds
    // Update state based on trigger
    // Play sounds on state transitions
    // Handle click detection
}
```

### Step 6: Draw() Method Pattern

The Draw() method should:
1. Check visibility
2. Select appropriate image based on state
3. Select appropriate label based on state
4. Draw image if present
5. Draw icon if present
6. Draw label if present
7. Call UpdateEffects()

```cpp
void GuiButton::Draw()
{
    if(!this->IsVisible())
        return;
    
    // Determine which image to draw based on state
    GuiImage * imageToDraw = image;
    GuiText * labelToDraw = label;
    GuiImage * iconToDraw = icon;
    
    if(state == STATE_SELECTED && imageOver)
        imageToDraw = imageOver;
    // ... handle other states
    
    // Draw selected components
    if(imageToDraw)
        imageToDraw->Draw();
    if(iconToDraw)
        iconToDraw->Draw();
    if(labelToDraw)
        labelToDraw->Draw();
    
    this->UpdateEffects();
}
```

### Step 7: Update Makefile.test

Add new source file:
```makefile
SOURCES := gui_element.cpp gui_trigger.cpp gui_sound.cpp \
           gui_imagedata.cpp gui_image.cpp \
           gui_text.cpp gui_window.cpp \
           gui_button.cpp
```

### Step 8: Compile and Test

```bash
cd gx/gui_cpp
make -f Makefile.test clean
make -f Makefile.test test
```

Must compile with **zero warnings**.

---

## Important Notes

### Code Style
- Follow existing pattern from Days 1-3 classes
- Use `(void)param;` to suppress unused parameter warnings
- Keep headers compact, implementation detailed
- Match snes9xGC structure closely
- NULL checks before using pointers

### Button Ownership
- Button does NOT own child elements
- Images, text, sounds are set via pointers
- Caller is responsible for memory management
- Destructor should NOT delete child elements
- This matches snes9xGC pattern

### State Management
- DEFAULT: Normal appearance
- SELECTED: Mouse/cursor over button (hover)
- HELD: Button being pressed
- CLICKED: Button was clicked (trigger released)
- DISABLED: Button inactive

### Sound Playback
- soundOver: Play when entering hover state
- soundHold: Play when button pressed
- soundClick: Play when button clicked/released
- Check if sound exists before playing
- Don't replay if already playing

### Trigger Handling
- Check multiple trigger slots (trigger[0], etc.)
- Handle different trigger types (simple, held, button-only)
- Use IsInside() to check cursor position
- Consider state channel (stateChan) for multiplayer

### Image/Label Positioning
- Images/labels inherit button's position
- Icon typically positioned on left or top
- Label typically centered or aligned
- UpdateEffects() handles animation
- Parent relationship set via SetParent()

### Compilation Flags
- `-fno-exceptions -fno-rtti` (no C++ overhead)
- `-std=c++11` (modern C++)
- `-O2 -Wall -Wextra -Werror` (strict warnings)

---

## Success Criteria for Day 4

- [ ] GuiButton class created and compiles
- [ ] gui.h updated with class declaration
- [ ] Makefile.test updated and working
- [ ] **Zero compilation warnings**
- [ ] Code follows snes9xGC patterns
- [ ] All setter methods implemented
- [ ] Update() handles state transitions
- [ ] Draw() renders correct elements
- [ ] Progress tracked in REFACTORING_PROGRESS.md

---

## Expected Output

At the end of Day 4, you should have:

1. **gui_button.cpp** (~300 lines)
2. **gui.h** updated (add ~40 lines for GuiButton)
3. **Makefile.test** updated
4. **All compiling cleanly**

Total new code: ~300 lines
Cumulative: ~1,981 lines (Day 1: 520 + Day 2: 520 + Day 3: 600 + Day 4: 300)

---

## Git Commit Message Template

```
Phase 3.2 Day 4: Implement GUI button widget

Interactive Button Complete:

Files Created:
- gui_button.cpp (~300 lines)
  * Interactive button widget
  * Multiple image states (default/over/hold/click)
  * Icon support for button decorations
  * Label support with state-based text
  * Sound effects (hover/hold/click)
  * Trigger-based interaction
  * State management (DEFAULT/SELECTED/HELD/CLICKED)
  * Proper element composition (doesn't own children)
  
Updated Files:
- gui.h - Added GuiButton class declaration (~40 lines)
- Makefile.test - Added gui_button.cpp
- REFACTORING_PROGRESS.md - Updated Phase 3.2 Day 4 completion

Compilation Test:
✅ All classes compile with devkitPPC
✅ Zero warnings
✅ Follows snes9xGC architecture

Implementation Notes:
- Button composed of GuiImage, GuiText, GuiSound
- State-based rendering (shows different images/text per state)
- Sound feedback on state transitions
- Trigger system for input detection
- Element lifetime managed by caller (button doesn't own)
- Ready for menu integration

Next: Day 5 - Final verification and testing

Progress: Phase 3.2 Day 4/5 Complete (80% of Phase 3)
```

---

## Reference Commands

```bash
# Check snes9xGC reference
ls -la ~/Repos/snes9xGC/source/gui/

# View class in header
grep -A 60 "class GuiButton " ~/Repos/snes9xGC/source/gui/gui.h

# View implementation details
head -300 ~/Repos/snes9xGC/source/gui/gui_button.cpp

# Compile test
cd /Users/caseyjparker/Repos/genplusGC/gx/gui_cpp
make -f Makefile.test clean && make -f Makefile.test test

# Check warnings
cd /Users/caseyjparker/Repos/genplusGC/gx/gui_cpp
make -f Makefile.test test 2>&1 | grep -i warning

# Line counts
wc -l gui_button.cpp gui.h

# Total cumulative
wc -l *.cpp *.h

# Update progress
vim /Users/caseyjparker/Repos/genplusGC/REFACTORING_PROGRESS.md

# Commit and push
git add gx/gui_cpp/
git commit -m "Phase 3.2 Day 4: Implement GUI button widget"
git push
```

---

## Next Session Preview (Day 5)

After completing Day 4, Day 5 will focus on:
- **Final Verification** - Comprehensive compilation checks
- **Memory Testing** - Verify no memory leaks in constructors/destructors
- **Integration Planning** - Document how to integrate with existing menu system
- **Code Review** - Final cleanup and documentation
- **Phase 3.2 Summary** - Create completion document

---

## Key Differences from snes9xGC to Note

1. **Element Ownership**: Button does NOT delete child elements. This is critical - the caller manages lifetime of images, text, and sounds.

2. **Trigger System**: We use our GuiTrigger class (already implemented). Button checks trigger state and position.

3. **State Transitions**: 
   - Moving cursor over button: DEFAULT → SELECTED
   - Pressing button: SELECTED → HELD
   - Releasing button: HELD → CLICKED (then back to SELECTED or DEFAULT)
   - Moving cursor away: Any state → DEFAULT

4. **Sound Timing**:
   - soundOver: Play ONCE when entering SELECTED state
   - soundHold: Play ONCE when entering HELD state
   - soundClick: Play ONCE when entering CLICKED state

5. **Rendering Priority**:
   - Draw image first (background)
   - Draw icon second (overlay on image)
   - Draw label last (text on top)

6. **NULL Checks**: Always check pointers before using:
   ```cpp
   if(image)
       image->Draw();
   ```

---

## Testing Strategy

After implementation:
1. Verify compilation with devkitPPC
2. Check for warnings (must be zero)
3. Verify class structure matches snes9xGC
4. Code review for NULL pointer safety
5. Verify destructor doesn't delete child elements
6. Check that all setter methods are implemented
7. Verify Update() logic for state transitions
8. Verify Draw() logic for element selection

---

**Ready to begin Phase 3.2 Day 4!**

Days 1-3 are complete with solid foundation, image handling, text rendering, and window management. Now we add the button widget - the primary interactive element that will be used throughout the menu system. This is the last major class before verification and testing. Follow the snes9xGC pattern closely, ensure proper element composition (not ownership), and keep everything compiling without warnings.
