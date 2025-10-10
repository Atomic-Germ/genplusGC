# Build System Integration Plan

## Overview

This document outlines the step-by-step plan for integrating the C++ GUI foundation (`gx/gui_cpp/`) into the Genesis Plus GX build system. This is the roadmap for **Phase 3.3**.

## Current Build System

Genesis Plus GX uses two makefiles:
- `Makefile.gc` - GameCube builds
- `Makefile.wii` - Wii builds

Both use DevkitPPC toolchain with libogc.

### Current Structure

```makefile
# Existing makefiles compile C sources only:
SOURCES := core/ gx/
CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(CFILES:.c=.o)

# Link
$(TARGET).elf: $(OFILES)
    $(CC) $(LDFLAGS) $(OFILES) $(LIBS) -o $@
```

## Required Changes

### Phase 3.3 Goal

Integrate C++ GUI compilation without breaking existing C code build:
1. Add C++ source files to build
2. Add C++ compiler support
3. Maintain backward compatibility
4. Verify clean compilation

### File Changes Needed

#### 1. Makefile.gc

**Current Status:** Compiles C sources only

**Changes Required:**

```makefile
#---------------------------------------------------------------------------------
# C++ GUI Sources
#---------------------------------------------------------------------------------
GUICPP_DIR := gx/gui_cpp
GUICPP_SRC := \
    $(GUICPP_DIR)/gui_element.cpp \
    $(GUICPP_DIR)/gui_trigger.cpp \
    $(GUICPP_DIR)/gui_sound.cpp \
    $(GUICPP_DIR)/gui_imagedata.cpp \
    $(GUICPP_DIR)/gui_image.cpp \
    $(GUICPP_DIR)/gui_text.cpp \
    $(GUICPP_DIR)/gui_window.cpp \
    $(GUICPP_DIR)/gui_button.cpp

GUICPP_OBJ := $(GUICPP_SRC:.cpp=.o)

#---------------------------------------------------------------------------------
# Compiler flags for C++
#---------------------------------------------------------------------------------
CXXFLAGS := $(CFLAGS) -fno-exceptions -fno-rtti -std=c++11

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------

# Pattern rule for C++ compilation
%.o: %.cpp
	@echo $(notdir $<)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@

# Update main target to include C++ objects
$(OUTPUT).elf: $(OFILES) $(GUICPP_OBJ)
	@echo linking ... $(notdir $@)
	$(LD) $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

# Add C++ objects to clean target
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(OFILES) $(GUICPP_OBJ)
```

**Key Points:**
- Uses `$(CXX)` for C++ compilation (powerpc-eabi-g++)
- Adds `-fno-exceptions -fno-rtti` for embedded systems
- Uses `-std=c++11` for modern C++ features
- Keeps existing C compilation unchanged
- Links both C and C++ objects together

#### 2. Makefile.wii

**Current Status:** Compiles C sources only

**Changes Required:**

Same changes as `Makefile.gc` (structure is nearly identical):

```makefile
#---------------------------------------------------------------------------------
# C++ GUI Sources
#---------------------------------------------------------------------------------
GUICPP_DIR := gx/gui_cpp
GUICPP_SRC := \
    $(GUICPP_DIR)/gui_element.cpp \
    $(GUICPP_DIR)/gui_trigger.cpp \
    $(GUICPP_DIR)/gui_sound.cpp \
    $(GUICPP_DIR)/gui_imagedata.cpp \
    $(GUICPP_DIR)/gui_image.cpp \
    $(GUICPP_DIR)/gui_text.cpp \
    $(GUICPP_DIR)/gui_window.cpp \
    $(GUICPP_DIR)/gui_button.cpp

GUICPP_OBJ := $(GUICPP_SRC:.cpp=.o)

#---------------------------------------------------------------------------------
# Compiler flags for C++
#---------------------------------------------------------------------------------
CXXFLAGS := $(CFLAGS) -fno-exceptions -fno-rtti -std=c++11

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------

# Pattern rule for C++ compilation
%.o: %.cpp
	@echo $(notdir $<)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@

# Update main target to include C++ objects
$(OUTPUT).elf: $(OFILES) $(GUICPP_OBJ)
	@echo linking ... $(notdir $@)
	$(LD) $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

# Add C++ objects to clean target
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol $(OFILES) $(GUICPP_OBJ)
```

## Implementation Steps

### Step 1: Verify Current Build

Before making changes, ensure current build works:

```bash
cd /Users/caseyjparker/Repos/genplusGC

# Clean existing build
make -f Makefile.gc clean

# Build for GameCube
make -f Makefile.gc

# Verify no errors
echo "GameCube build: $?"

# Clean
make -f Makefile.wii clean

# Build for Wii
make -f Makefile.wii

# Verify no errors
echo "Wii build: $?"
```

**Expected Result:** Both builds succeed with no errors (existing baseline)

### Step 2: Backup Makefiles

Create backups before modification:

```bash
cp Makefile.gc Makefile.gc.backup
cp Makefile.wii Makefile.wii.backup
```

### Step 3: Update Makefile.gc

Add C++ GUI sources and compilation rules:

1. Add GUICPP_DIR variable
2. Add GUICPP_SRC list
3. Add GUICPP_OBJ variable
4. Add CXXFLAGS
5. Add C++ pattern rule
6. Update link target to include GUICPP_OBJ
7. Update clean target

### Step 4: Update Makefile.wii

Apply same changes to Wii makefile.

### Step 5: Test GameCube Build

```bash
cd /Users/caseyjparker/Repos/genplusGC

# Clean build
make -f Makefile.gc clean

# Build with C++ GUI
make -f Makefile.gc 2>&1 | tee gc_build.log

# Check for errors
if [ $? -eq 0 ]; then
    echo "✅ GameCube build succeeded"
else
    echo "❌ GameCube build failed"
    cat gc_build.log
fi

# Check for warnings
grep -i warning gc_build.log || echo "✅ No warnings"

# Verify output
ls -lh genplus_cube.dol
```

### Step 6: Test Wii Build

```bash
cd /Users/caseyjparker/Repos/genplusGC

# Clean build
make -f Makefile.wii clean

# Build with C++ GUI
make -f Makefile.wii 2>&1 | tee wii_build.log

# Check for errors
if [ $? -eq 0 ]; then
    echo "✅ Wii build succeeded"
else
    echo "❌ Wii build failed"
    cat wii_build.log
fi

# Check for warnings
grep -i warning wii_build.log || echo "✅ No warnings"

# Verify output
ls -lh genplus_wii.dol
```

### Step 7: Verify Binary Size

Check that binary size increase is reasonable:

```bash
# Before integration (baseline)
ls -lh genplus_cube.dol.before
ls -lh genplus_wii.dol.before

# After integration
ls -lh genplus_cube.dol
ls -lh genplus_wii.dol

# Calculate increase
# Expected: ~30-50 KB increase (8 C++ classes, no runtime usage yet)
```

### Step 8: Verify No Symbol Conflicts

Ensure C++ GUI doesn't conflict with C GUI:

```bash
# Extract symbols from ELF
powerpc-eabi-objdump -t builds/genplus_cube.o | grep -i gui > symbols_gc.txt
powerpc-eabi-objdump -t builds/genplus_wii.o | grep -i gui > symbols_wii.txt

# Check for conflicts (should be none - different namespaces)
# C GUI: gui_* functions
# C++ GUI: GuiElement::*, GuiButton::*, etc.
```

### Step 9: Test Incremental Build

Verify incremental compilation works:

```bash
# Touch a C++ file
touch gx/gui_cpp/gui_button.cpp

# Rebuild
make -f Makefile.gc

# Should only recompile gui_button.cpp and relink
# Verify fast incremental build (~2-3 seconds)
```

### Step 10: Commit Changes

```bash
git add Makefile.gc Makefile.wii
git commit -m "Phase 3.3: Integrate C++ GUI into build system"
```

## Potential Issues & Solutions

### Issue 1: C++ Standard Library Not Found

**Symptom:** Linker errors about missing C++ runtime

**Cause:** DevkitPPC may have limited C++ standard library support

**Solution:**
```makefile
# Use -nostdlib and link manually
LDFLAGS += -nostdlib
LIBS += -lstdc++ -lgcc
```

Our GUI doesn't use standard library (no exceptions, no RTTI, no STL), so this should not occur.

### Issue 2: Name Mangling Conflicts

**Symptom:** Linker errors about duplicate symbols

**Cause:** C++ name mangling conflicts with C code

**Solution:**
```cpp
// In gui.h, wrap C linkage if needed:
#ifdef __cplusplus
extern "C" {
#endif

// C linkage declarations

#ifdef __cplusplus
}
#endif
```

Our GUI uses C++ exclusively, so this should not occur unless integrating with C code.

### Issue 3: Binary Size Too Large

**Symptom:** DOL file exceeds reasonable size (>10 MB)

**Cause:** Debug symbols or unused code included

**Solution:**
```makefile
# Strip symbols
LDFLAGS += -s

# Enable optimization
CXXFLAGS += -O2 -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections
```

Already enabled in test makefile, should not occur.

### Issue 4: Compilation Warnings

**Symptom:** Warnings during C++ compilation

**Cause:** Strict warning flags catching issues

**Solution:**
- Fix warnings (preferred)
- OR disable specific warnings (not recommended):
```makefile
CXXFLAGS += -Wno-unused-parameter
```

We've already verified zero warnings with `-Wall -Wextra`.

### Issue 5: Missing Headers

**Symptom:** Cannot find gui.h

**Cause:** Include paths not set correctly

**Solution:**
```makefile
# Add include path
CXXFLAGS += -I$(CURDIR)
```

Should not occur - gui.h uses relative includes.

## Verification Checklist

After integration, verify:

- [ ] GameCube build compiles without errors
- [ ] GameCube build compiles without warnings
- [ ] Wii build compiles without errors
- [ ] Wii build compiles without warnings
- [ ] Binary size increase is reasonable (<100 KB)
- [ ] No symbol conflicts between C and C++ GUI
- [ ] Incremental builds work correctly
- [ ] Clean builds work correctly
- [ ] DOL files are generated correctly
- [ ] Existing functionality still works (C GUI untouched)

## Testing on Hardware

### GameCube Testing

```bash
# Build DOL
make -f Makefile.gc

# Copy to SD card (if using SD loader)
cp genplus_cube.dol /Volumes/SDCARD/apps/genplus/boot.dol

# Test on GameCube
# - Boot should work
# - Existing menus should function
# - No crashes or hangs
```

### Wii Testing

```bash
# Build DOL
make -f Makefile.wii

# Copy to SD card
cp genplus_wii.dol /Volumes/SDCARD/apps/genplus/boot.dol

# Test on Wii
# - Boot should work  
# - Existing menus should function
# - No crashes or hangs
```

**Note:** C++ GUI is compiled but not used yet. Phase 4 will integrate rendering/input.

## Performance Considerations

### Binary Size

Expected binary size increase:
- **Before:** ~1.5 MB (C code only)
- **After:** ~1.55 MB (C + C++ GUI foundation)
- **Increase:** ~50 KB (8 classes, minimal overhead)

### Runtime Performance

C++ GUI should have minimal performance impact:
- Virtual functions used sparingly
- No RTTI overhead (`-fno-rtti`)
- No exception overhead (`-fno-exceptions`)
- Similar performance to C GUI

### Memory Usage

Static memory increase:
- **Global variables:** ~100 bytes (constants)
- **Code size:** ~50 KB (functions)
- **Data size:** ~1 KB (vtables)

Dynamic memory (runtime):
- Created on demand (menus)
- Typical usage: ~50-100 KB (menu elements)
- Comparable to existing C GUI

## Rollback Plan

If integration fails, rollback is simple:

```bash
# Restore original makefiles
cp Makefile.gc.backup Makefile.gc
cp Makefile.wii.backup Makefile.wii

# Clean and rebuild
make -f Makefile.gc clean
make -f Makefile.gc

make -f Makefile.wii clean  
make -f Makefile.wii

# Verify builds work
```

C++ GUI code remains in `gx/gui_cpp/` but is not compiled.

## Next Steps (Phase 4)

After successful build integration:

1. **Rendering Integration**
   - Implement `GuiImage::Draw()` with GX calls
   - Implement `GuiText::Draw()` with FreeTypeGX
   - Test visual output

2. **Controller Integration**
   - Implement `GuiTrigger::Update()` with PAD/WPAD
   - Test button interaction
   - Verify input response

3. **Audio Integration**
   - Implement `GuiSound::Play()` with ASND
   - Test sound effects
   - Verify audio playback

4. **Resource Integration**
   - Implement `GuiImageData` PNG loading with PNGU
   - Test image loading from SD card
   - Verify resource management

## Timeline

**Phase 3.3 Estimated Duration:** 1-2 days

- **Day 1 Morning:** Makefile updates and initial build testing
- **Day 1 Afternoon:** Issue resolution and verification
- **Day 2 Morning:** Hardware testing (optional)
- **Day 2 Afternoon:** Documentation and commit

## Success Criteria

Phase 3.3 is complete when:

1. ✅ Both makefiles updated with C++ support
2. ✅ GameCube build succeeds with zero warnings
3. ✅ Wii build succeeds with zero warnings
4. ✅ Binary size increase is reasonable
5. ✅ No symbol conflicts
6. ✅ Incremental builds work
7. ✅ Existing functionality preserved
8. ✅ Changes committed to repository

## References

- DevkitPPC documentation: https://devkitpro.org/wiki/Getting_Started
- libogc documentation: https://libogc.devkitpro.org/
- Makefile.test: `gx/gui_cpp/Makefile.test` (reference implementation)
- Phase 3.2 Summary: `PHASE3.2_SUMMARY.md`
