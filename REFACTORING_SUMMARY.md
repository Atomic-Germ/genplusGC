# Genesis Plus GX Refactoring - Quick Reference

## Overview

This is a systematic refactoring of Genesis Plus GX to create a focused, optimized GameCube/Wii emulator with modern C++ GUI architecture based on snes9xGC and fceuGC.

**Full Plan:** See `REFACTORING_PLAN.md` for complete details (1,628 lines)

---

## Key Changes

### What We're Removing
- **Platform Code (~2MB):** libretro/, sdl/, psp2/, gcw0/, builds/
- **Old GUI (~9,179 lines):** Current C-based GUI system
- **Dead Code:** Platform abstraction for removed platforms

### What We're Adding
- **Modern GUI (~9,000 lines C++):** Object-oriented system based on libwiigui
- **Unit Tests (~2,000 lines):** Comprehensive test suite with CI integration
- **Optimizations:** Memory, rendering, and build system improvements
- **Documentation:** User manual, API docs, build guides

---

## 9-Phase Timeline

| Phase | Focus | Duration | Key Deliverables |
|-------|-------|----------|------------------|
| 1 | Foundation & Cleanup | Week 1 | Remove platforms, baseline tests |
| 2 | Unit Testing | Week 2 | Test framework, CI integration, initial tests |
| 3 | GUI Foundation | Week 3-4 | Core C++ classes (Element, Window, Button, etc.) |
| 4 | GUI Widgets | Week 5 | FileBrowser, SaveBrowser, OptionBrowser, Keyboard |
| 5 | Menu System | Week 6 | Menu logic, state machine, integration |
| 6 | Testing & QA | Week 7 | Expand tests, integration testing, bug fixing |
| 7 | Optimization | Week 8 | Memory, rendering, audio, build optimizations |
| 8 | Documentation | Week 9 | Doxygen, user manual, guides |
| 9 | Release Prep | Ongoing | Final testing, packaging, release |

**Total Estimate:** ~9.5 weeks (47 working days)

---

## Architecture Changes

### Current GUI (C-based)
```
gx/gui/
├── gui.c (2,850 lines)
├── menu.c (7,056 lines)  
├── filesel.c (1,167 lines)
├── cheats.c (2,341 lines)
└── font.c (564 lines)
Total: 9,179 lines C
```

### New GUI (C++ object-oriented)
```
gx/gui/
├── gui.h (master header)
├── gui_element.cpp (base class)
├── gui_window.cpp
├── gui_button.cpp
├── gui_image.cpp
├── gui_imagedata.cpp
├── gui_text.cpp
├── gui_trigger.cpp
├── gui_sound.cpp
├── gui_filebrowser.cpp
├── gui_savebrowser.cpp
├── gui_optionbrowser.cpp
└── gui_keyboard.cpp

gx/
├── menu.cpp (menu logic)
└── menu.h
Total: ~9,000 lines C++
```

---

## Testing Strategy

### Unit Tests (Automated via CI)
- **Framework:** Simple custom framework (no external deps)
- **Mocking:** Mock libogc/GameCube libraries  
- **Target:** 150+ tests across multiple modules
- **Run Time:** <1 second total
- **CI Integration:** GitHub Actions, runs before builds

### Test Categories
1. **Config System** - Settings validation, defaults
2. **File Operations** - Path handling, extensions
3. **Button Mapping** - Controller configuration
4. **Video Modes** - Mode selection logic
5. **Menu Logic** - State transitions
6. **Save States** - Slot management
7. **Cheats** - Code parsing
8. **GUI Components** - Element, Button, etc.

### Integration Tests (Manual)
- Hardware: GameCube, Wii, Swiss, HBC
- Controllers: GC pad, Wiimote, Classic, Nunchuk
- Storage: SD, USB, DVD, Memory Card
- Games: Genesis, Sega CD, SMS, GG, SG-1000
- Features: ROMs, saves, states, cheats, settings

---

## Key Optimizations

### Memory (from appendix instructions)
- Lazy loading for GUI resources
- Texture compression (RGB565)
- ROM streaming for large files
- MEM2 utilization on Wii
- GameCube "lite mode" option

### Rendering
- Pre-compiled display lists for static UI
- Texture atlas for small elements
- Batch GX calls
- NTSC filter default off on GC
- EFB-scaled copy optimization

### Build System
```makefile
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -s
CXXFLAGS += -fno-exceptions -fno-rtti -fvisibility=hidden
```

### Audio
- Adaptive buffer sizing (2-4 buffers)
- Tighter rate control
- MEM2 buffers on Wii
- Direct buffer writing

---

## Success Criteria

### Performance Targets
- ✅ 60 FPS Genesis/Master System
- ✅ 50 FPS PAL games
- ✅ <3 frames input lag
- ✅ <100ms save state operations
- ✅ <1 second ROM load from SD
- ✅ <50MB RAM on GameCube

### Quality Targets
- ✅ 0 compiler warnings
- ✅ 0 memory leaks (valgrind)
- ✅ 150+ unit tests passing
- ✅ CI builds passing
- ✅ All controllers working
- ✅ All storage devices working

### Code Quality
- ✅ Well-documented (Doxygen)
- ✅ Consistent style
- ✅ Modular design
- ✅ Easy to extend
- ✅ No technical debt

---

## Build & Test Workflow

### Local Development
```bash
# Unit tests (standard g++)
cd tests
make clean && make run

# Full build will use CI
# No local GC/Wii building required
```

### CI Pipeline (GitHub Actions)
```yaml
1. Unit Tests
   - Build tests with g++
   - Run all tests
   - Upload results
   
2. Build GameCube (if tests pass)
   - devkitPPC container
   - make -f Makefile.gc
   - Upload .dol
   
3. Build Wii (if tests pass)
   - devkitPPC container
   - make -f Makefile.wii
   - Upload .dol + HBC files
   
4. Release (on master)
   - Create artifacts
   - Tag Pre-release
   - Upload to GitHub Releases
```

---

## Reference Materials

### Instructions
- **Primary:** `.github/copilot/instructions/genplusGC.instructions.md`
- **Appendix:** `.github/copilot/instructions/genplusGC.appendix.instructions.md`
- **Full Plan:** `REFACTORING_PLAN.md` (this repository)

### Reference Repositories
- **snes9xGC:** https://github.com/Atomic-Germ/snes9xGC
  - GUI architecture model
  - Unit testing framework
  - Build system
  - CI/CD pipeline

- **fceuGC:** https://github.com/Atomic-Germ/fceuGC  
  - Similar architecture
  - Additional test examples
  - Documentation patterns

### Key Files to Study
```
snes9xGC/
├── source/gui/gui.h                    # Class hierarchy
├── source/gui/gui_element.cpp          # Base class
├── source/menu.cpp                     # Menu logic
├── tests/framework/simple_test.h       # Test framework
├── tests/Makefile                      # Test build
└── .github/workflows/test.yml          # CI config

fceuGC/
├── tests/README.md                     # Testing guide
└── tests/unit/test_*.cpp               # Test examples
```

---

## Quick Start Guide

### Step 1: Familiarize
- [x] Read `REFACTORING_PLAN.md` (full details)
- [x] Review `.github/copilot/instructions/*.md`
- [x] Study snes9xGC GUI architecture
- [x] Examine test framework in snes9xGC/fceuGC

### Step 2: Phase 1 (Week 1)
- [ ] Create branch `refactor/phase1-cleanup`
- [ ] Remove platform directories (libretro, sdl, psp2, gcw0)
- [ ] Clean up platform #ifdef blocks
- [ ] Update documentation
- [ ] Verify CI builds

### Step 3: Phase 2 (Week 2)
- [ ] Create tests/ directory structure
- [ ] Copy test framework from snes9xGC
- [ ] Create mock_libogc.h/cpp
- [ ] Write initial tests
- [ ] Add CI test job

### Step 4: Continue with Full Plan
- Follow `REFACTORING_PLAN.md` Phase 3-9
- Test at each step
- Validate with CI
- Document changes

---

## Important Notes

### Always Build in CI
- **Do not** build locally for GC/Wii
- Use GitHub Actions for all builds
- Unit tests can run locally (standard g++)
- Verify builds via CI artifacts

### Follow Proven Patterns
- **Copy from snes9xGC/fceuGC** - don't reinvent
- Maintain same class structure
- Use same testing approach
- Follow same conventions

### Minimize Changes
- **Surgical edits only** - don't refactor unnecessarily
- Focus on objectives in instructions
- Keep emulation core untouched unless fixing bugs
- Preserve working functionality

### Test Everything
- Write unit tests as you go
- Test each phase before moving on
- Use CI to validate
- Manual testing on hardware when possible

---

## Questions & Support

### Documentation
- Full plan: `REFACTORING_PLAN.md`
- Instructions: `.github/copilot/instructions/`
- Test guide: `tests/README.md` (once created)

### Reference Code
- snes9xGC source
- fceuGC source
- Original instructions

### Common Issues
- See `REFACTORING_PLAN.md` Appendix D for testing checklist
- See `REFACTORING_PLAN.md` Appendix A for risk mitigation
- Check CI logs for build errors

---

**Version:** 1.0  
**Status:** Ready for Phase 1  
**Last Updated:** 2024
