# GameCube / Wii Platform Information

## Overview

Genesis Plus GX is optimized for Nintendo GameCube and Wii consoles. This document provides technical information about these platforms and how the emulator leverages their capabilities.

## Platform Specifications

### Nintendo GameCube (DOL-001/101)

**Hardware:**
- CPU: IBM PowerPC 750CXe "Gekko" @ 486 MHz
- RAM: 24MB main (16MB MEM1 + 8MB locked ARAM)
- GPU: ATI "Flipper" @ 162 MHz (GX Graphics API)
- Audio: Custom DSP, 48 KHz stereo
- Storage: DVD-ROM, Memory Card (up to 16MB)

**Constraints:**
- Limited memory (24MB total)
- No USB support (Swiss loader required for SD)
- DVD read-only
- No network capabilities

**Optimization Focus:**
- Memory efficiency is critical
- Texture compression for GUI
- ROM streaming for large files
- Aggressive code size reduction

### Nintendo Wii (RVL-001/101)

**Hardware:**
- CPU: IBM PowerPC 750CL "Broadway" @ 729 MHz
- RAM: 88MB total (24MB MEM1 + 64MB MEM2)
- GPU: ATI "Hollywood" @ 243 MHz (GX Graphics API)
- Audio: Custom DSP, 48 KHz stereo
- Storage: SD Card, USB, DVD, Memory Card

**Advantages over GameCube:**
- 64MB additional MEM2 for buffers and assets
- Native SD/USB support
- Network capabilities (Wi-Fi)
- Progressive scan video (480p)
- Multiple controller types

**Wii-Specific Features:**
- Wiimote support (IR pointing, motion)
- Classic Controller support
- USB storage for ROMs and saves
- Network ROM loading (future)

## Memory Management

### GameCube Strategy
```c
// Primary RAM (MEM1) - 24MB total
// - System: ~8MB
// - Emulator code: ~2-3MB
// - Emulation state: ~4-6MB
// - GUI/textures: ~2-4MB
// - ROM buffer: remaining (~4-8MB)

// For ROMs >8MB: streaming from storage
```

### Wii Strategy
```c
// MEM1 (24MB) - Core emulation
// - Same layout as GameCube
// - Critical hot paths

// MEM2 (64MB) - Extended features
// - Large ROM buffering (up to 16MB)
// - Save state storage
// - Audio buffers
// - GUI textures and resources
// - CHD/compressed format working buffers

#ifdef HW_RVL
    void *buffer = memalign(32, size); // Allocate in MEM2
#else
    void *buffer = malloc(size);        // MEM1 only
#endif
```

## Graphics (GX API)

### Rendering Pipeline
1. **Display Lists**: Pre-compiled for static UI elements
2. **Texture Management**: Atlas for small sprites, streaming for large
3. **Frame Buffer**: 32-byte aligned, optimal for DMA
4. **Video Modes**: 
   - GameCube: 480i (NTSC), 576i (PAL)
   - Wii: 480i/480p (NTSC), 576i (PAL)

### Optimization Techniques
- Batch GX primitive calls
- Minimize texture binds
- Use display lists for menus
- Pre-swizzle textures at build time
- RGB565 format for UI (vs RGBA8888)

## Audio (ASND/DMA)

### Configuration
- Sample Rate: 48 KHz (native)
- Buffers: 2-4 adaptive (based on performance)
- Format: 16-bit stereo PCM
- Latency: ~40-60ms target

### Optimization
- MEM2 audio buffers on Wii
- Direct DMA buffer writing
- Cache-aligned buffers (32-byte)
- Dynamic buffer adjustment

## Controller Support

### GameCube Controller
- All 4 ports supported
- Analog stick for navigation
- Rumble support
- Full button mapping

### Wiimote (Wii only)
- Horizontal mode (D-pad + buttons)
- IR pointing for menus
- Rumble support
- Motion controls (future)

### Wiimote + Nunchuk (Wii only)
- Analog stick for navigation
- Combined button set
- Motion controls (future)

### Classic Controller (Wii only)
- Full button compatibility
- Analog sticks
- Shoulder buttons
- Preferred for gameplay

### Wii U Pro Controller (via DRC, Wii only)
- Same as Classic Controller
- Wireless connectivity

## Storage Devices

### SD Card (Wii, Swiss on GC)
- Primary storage for ROMs
- FAT32/exFAT support
- Fast access (<100ms load)
- Save state storage
- Configuration files

### USB Drive (Wii only)
- Secondary ROM storage
- FAT32 recommended
- Larger capacity
- Slower than SD

### DVD (GameCube/Wii)
- Read-only
- ISO9660 filesystem
- Good for ROM collections
- No save capability

### Memory Card (GameCube/Wii)
- SRAM/EEPROM saves
- Configuration backup
- Limited capacity (59-251 blocks)

## Video Modes

### NTSC (60 Hz)
- 640x480 (GameCube/Wii)
- Progressive 480p (Wii only, component cable)
- Used for US/JP regions

### PAL (50 Hz)
- 640x576 interlaced
- Used for EU region
- Proper timing for PAL games

### Aspect Ratio
- 4:3 native
- 16:9 widescreen (pillarbox)
- Per-game override option

## Build Targets

### GameCube (Makefile.gc)
```makefile
TARGET = genplus_cube.dol
CFLAGS = -O3 -fomit-frame-pointer
CFLAGS += -DUSE_LIBTREMOR -DUSE_16BPP_RENDERING
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -s
```

### Wii (Makefile.wii)
```makefile
TARGET = genplus_wii.dol
CFLAGS = -O3 -fomit-frame-pointer -DHW_RVL
CFLAGS += -DUSE_LIBTREMOR -DUSE_16BPP_RENDERING
CFLAGS += -DUSE_LIBCHDR -DMAXROMSIZE=15728640
LDFLAGS = -Wl,--gc-sections -s
LIBS = -ldi -lwiiuse -lbte  # Wii-specific
```

## Performance Targets

### GameCube
- **Genesis/Mega Drive**: 60 FPS (100% speed)
- **Sega CD**: 60 FPS (most games)
- **Master System**: 60 FPS (100% speed)
- **Game Gear**: 60 FPS (100% speed)
- **Input Lag**: <3 frames
- **State Save/Load**: <100ms
- **ROM Load**: <1 second (SD via Swiss)
- **Memory Usage**: <50MB

### Wii
- **All systems**: 60/50 FPS (region-dependent)
- **CD Games**: Full speed with CHD
- **Large ROMs**: Full buffering up to 16MB
- **Input Lag**: <3 frames
- **State Save/Load**: <50ms (MEM2 cached)
- **ROM Load**: <500ms (SD/USB)
- **Memory Usage**: <80MB

## Known Limitations

### GameCube
- ROM size limited to ~8MB (streaming for larger)
- No native SD/USB (requires Swiss)
- Limited GUI features (lite mode)
- No network capabilities
- No progressive scan

### Wii
- DVD drive slower than SD/USB
- Wi-Fi limited range
- Classic Controller required for best experience
- Motion controls not yet implemented

## Loader Compatibility

### Swiss (GameCube)
- SD Card support
- USB support (via adapter)
- Direct .dol execution
- Memory card emulation

### Homebrew Channel (Wii)
- Standard installation
- Direct boot
- Return to HBC on exit
- Meta.xml support

### Nintendont (Wii)
- GameCube mode emulation
- USB/SD support
- Not primary target

## Future Platform Features

### Planned
- [ ] Wii motion controls (IR aiming for light gun)
- [ ] Network ROM loading (Wii)
- [ ] Wiimote DRC support
- [ ] Save state cloud sync (Wii)
- [ ] Screenshot sharing (Wii)

### Considered
- [ ] GameCube LAN adapter support
- [ ] GBA link cable emulation
- [ ] Online multiplayer (Wii)
- [ ] IPS/BPS patching

## Technical References

- [libogc Documentation](https://libogc.devkitpro.org/)
- [devkitPPC](https://devkitpro.org/)
- [GX Programming Guide](https://www.gc-forever.com/)
- [Wii Homebrew](https://wiibrew.org/)
- [GameCube Development Wiki](https://gc-forever.com/wiki/)

---

**Last Updated**: 2024  
**Platform Version**: GameCube/Wii Focused Build
