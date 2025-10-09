# Building Genesis Plus GX for GameCube / Wii

## Prerequisites

### Required Tools

1. **devkitPro** (devkitPPC toolchain)
   - Download from: https://devkitpro.org/wiki/Getting_Started
   - Install devkitPPC package
   - Version: r41 or later recommended

2. **libogc** (GameCube/Wii libraries)
   - Version: 2.3.1 or later
   - Included with devkitPro

3. **Additional Libraries**
   - gamecube-tools-git
   - libogc2
   - libogc2-libdvm

### Environment Setup

```bash
# Set environment variables (usually done by devkitPro installer)
export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=$DEVKITPRO/devkitPPC
export PATH=$DEVKITPPC/bin:$PATH
```

## Building

### Wii Build

```bash
# Clone repository
git clone https://github.com/Atomic-Germ/genplusGC
cd genplusGC

# Build for Wii
make -f Makefile.wii clean
make -f Makefile.wii -j$(nproc)

# Output: genplus_wii.dol
```

### GameCube Build

```bash
# Build for GameCube
make -f Makefile.gc clean
make -f Makefile.gc -j$(nproc)

# Output: genplus_cube.dol
```

## Build Options

### Debug Build

```bash
# Enable debugging symbols and disable optimization
make -f Makefile.wii DEBUG=1

# Output will be larger but easier to debug
```

### Profile Build

```bash
# Enable profiling
make -f Makefile.wii PROFILE=1
```

### Clean Build

```bash
# Clean all build artifacts
make -f Makefile.wii clean

# Clean and rebuild
make -f Makefile.wii clean all
```

## Build Targets

### Makefile.wii Targets

- `all` (default) - Build complete Wii .dol file
- `clean` - Remove all build artifacts
- `run` - Build and run in Dolphin (if configured)

### Makefile.gc Targets

- `all` (default) - Build complete GameCube .dol file
- `clean` - Remove all build artifacts
- `run` - Build and run in Dolphin (if configured)

## Build Configuration

### Compiler Flags (Wii)

```makefile
# Optimization
CFLAGS = -O3 -fomit-frame-pointer

# Platform defines
CFLAGS += -DHW_RVL              # Wii-specific code
CFLAGS += -DUSE_LIBTREMOR       # Tremor (Vorbis)
CFLAGS += -DUSE_16BPP_RENDERING # 16-bit rendering
CFLAGS += -DUSE_LIBCHDR         # CHD support
CFLAGS += -DMAXROMSIZE=15728640 # 15MB ROM limit

# Code size reduction
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -s
```

### Compiler Flags (GameCube)

```makefile
# Optimization
CFLAGS = -O3 -fomit-frame-pointer

# Platform defines
CFLAGS += -DUSE_LIBTREMOR       # Tremor (Vorbis)
CFLAGS += -DUSE_16BPP_RENDERING # 16-bit rendering
CFLAGS += -DALT_RENDERER        # Alternative renderer

# Code size reduction
CFLAGS += -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-sections -s
```

## CI/CD Building (GitHub Actions)

The project uses GitHub Actions for automated building. You don't need to build locally for testing.

### Viewing CI Builds

1. Push to any branch
2. Go to Actions tab on GitHub
3. View build progress and download artifacts

### Build Workflow

```yaml
# .github/workflows/build.yml runs on every push
- Checkout code
- Install devkitPPC toolchain
- Build Wii version
- Build GameCube version
- Upload artifacts
- Create pre-release (on master branch)
```

## Troubleshooting

### Common Issues

**Error: "devkitPPC not found"**
```bash
# Solution: Set environment variables
export DEVKITPRO=/opt/devkitpro
export DEVKITPPC=$DEVKITPRO/devkitPPC
```

**Error: "libogc not found"**
```bash
# Solution: Install libogc via dkp-pacman
sudo dkp-pacman -S libogc2
```

**Error: "Undefined reference to..."**
```bash
# Solution: Clean and rebuild
make -f Makefile.wii clean
make -f Makefile.wii
```

**Build is very slow**
```bash
# Solution: Use parallel compilation
make -f Makefile.wii -j8  # Use 8 cores
```

**Out of memory during link**
```bash
# Solution: Reduce optimization or build without LTO
# Edit Makefile and remove -flto if present
```

## Build Output

### Wii Build Artifacts

```
genplus_wii.dol          # Main executable
genplus_wii.elf          # ELF format (debugging)
genplus_wii.elf.map      # Symbol map
build_wii/               # Object files
```

### GameCube Build Artifacts

```
genplus_cube.dol         # Main executable
genplus_cube.elf         # ELF format (debugging)
genplus_cube.elf.map     # Symbol map
build_cube/              # Object files
```

### Expected Build Size

- **Wii**: ~2.5-3.0 MB (.dol)
- **GameCube**: ~2.0-2.5 MB (.dol)

## Installation

### Wii (Homebrew Channel)

1. Create directory structure:
```
sd:/apps/genplusgx/
sd:/apps/genplusgx/boot.dol
sd:/apps/genplusgx/icon.png
sd:/apps/genplusgx/meta.xml
```

2. Copy files:
```bash
cp genplus_wii.dol sd:/apps/genplusgx/boot.dol
cp hbc/icon.png sd:/apps/genplusgx/
cp hbc/meta.xml sd:/apps/genplusgx/
```

3. Create ROM directories:
```
sd:/genplusgx/roms/
sd:/genplusgx/saves/
sd:/genplusgx/cheats/
```

### GameCube (Swiss)

1. Copy .dol to SD card:
```bash
cp genplus_cube.dol sd:/
```

2. Boot Swiss
3. Navigate to genplus_cube.dol
4. Launch

## Testing

### Dolphin Emulator

```bash
# Install Dolphin
# Set up devkitPPC in Dolphin settings

# Run Wii build
dolphin-emu -e genplus_wii.dol

# Run GameCube build
dolphin-emu -e genplus_cube.dol
```

### Real Hardware

**Wii:**
- Copy to SD card
- Install via Homebrew Channel
- Launch and test

**GameCube:**
- Requires modchip, Swiss, or other loader
- Copy .dol to memory card or SD (via Swiss)
- Boot and test

## Development Builds

### Quick Development Cycle

```bash
# 1. Make code changes
# 2. Quick build
make -f Makefile.wii -j8

# 3. Test in Dolphin
dolphin-emu -e genplus_wii.dol

# 4. Push to GitHub for CI build
git commit -am "Feature: description"
git push

# 5. Download CI artifacts for real hardware testing
```

### Debugging

```bash
# Build with debug symbols
make -f Makefile.wii DEBUG=1

# Use GDB with Dolphin
# See Dolphin debugging documentation
```

## Advanced Options

### Custom Optimization

Edit Makefile.wii or Makefile.gc:

```makefile
# Maximum optimization
CFLAGS = -O3 -flto

# Optimize for size
CFLAGS = -Os

# Optimize for speed
CFLAGS = -O3 -funroll-loops
```

### Feature Toggles

```makefile
# Disable CHD support (smaller binary)
# Remove: -DUSE_LIBCHDR

# Disable NTSC filter (faster)
# Add: -DNO_NTSC_FILTER

# Enable extra debugging
# Add: -DDEBUG -DLOG_ERROR
```

## Cross-Platform Notes

This build system is designed for:
- **Linux** (native, recommended)
- **macOS** (native, works well)
- **Windows** (via WSL2 or MSYS2)

### Windows (WSL2)

```bash
# Install WSL2 Ubuntu
# Install devkitPro for Linux
# Build normally
```

### Windows (MSYS2)

```bash
# Install MSYS2
# Install devkitPro via pacman
# Build using MSYS2 terminal
```

## CI/CD Integration

### GitHub Actions

The project builds automatically on push. See `.github/workflows/build.yml`.

**To use CI builds:**
1. Push code to GitHub
2. Wait for Actions to complete (~5-10 minutes)
3. Download artifacts from Actions tab
4. Test on real hardware

**Benefits:**
- No local devkitPPC setup needed
- Consistent build environment
- Automatic testing
- Pre-release artifacts

## Additional Resources

- [devkitPro Wiki](https://devkitpro.org/wiki/)
- [libogc Documentation](https://libogc.devkitpro.org/)
- [GameCube Development](https://gc-forever.com/wiki/)
- [Wii Homebrew Guide](https://wiibrew.org/wiki/Homebrew_setup)

---

**Last Updated**: 2024  
**Build System Version**: Phase 1 (C-based)
