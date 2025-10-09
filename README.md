Genesis Plus GX is an open-source Sega 8/16 bit emulator focused on accuracy and optimized performance for **GameCube and Wii** consoles. Built with [libogc / devkitPPC](http://sourceforge.net/projects/devkitpro/), this version is exclusively focused on delivering the best experience on Nintendo GameCube and Wii hardware.

**Note:** This is a GameCube/Wii-focused fork. For other platforms, see the original [Genesis Plus GX](https://github.com/ekeeke/Genesis-Plus-GX) or platform-specific ports like [Retroarch (libretro)](http://www.libretro.com).

----

The source code, initially based on Genesis Plus 1.2a by [Charles MacDonald](http://www.techno-junk.org/ ) has been heavily modified & enhanced, with respect to original goals and design, in order to improve emulation accuracy as well as adding support for new peripherals, cartridge or console hardware and many other exciting [features](https://bitbucket.org/eke/genesis-plus-gx/src/master/wiki/Features.md).

The result is that Genesis Plus GX is now more a continuation of the original project than a simple port, providing very accurate emulation and [100% compatibility](https://bitbucket.org/eke/genesis-plus-gx/src/master/wiki/Compatibility.md) with Genesis / Mega Drive, Sega/Mega CD, Master System, Game Gear & SG-1000 released software (including all unlicensed or pirate known dumps), also emulating backwards compatibility modes when available. All the people who contributed (directly or indirectly) to this project are listed on the [Credits](https://bitbucket.org/eke/genesis-plus-gx/src/master/wiki/Credits.md) page.

----

The emulation core is based on the multi-platform Genesis Plus GX, available under a specific non-commercial [license](LICENSE.txt). This GameCube/Wii port focuses on optimizing performance and features specifically for these platforms, removing multi-platform abstractions to create a leaner, more efficient emulator.

----

## Features

- **Complete Sega Emulation**: Genesis/Mega Drive, Sega/Mega CD, Master System, Game Gear, SG-1000
- **High Accuracy**: 100% compatibility with all known commercial releases
- **GameCube/Wii Optimized**: Focused development for these platforms
- **Save States**: Quick save and load with screenshot thumbnails
- **Cheat Support**: Game Genie and Action Replay codes
- **Controller Support**: GameCube pad, Wiimote, Classic Controller, and more
- **Multiple Storage**: SD Card, USB (Wii), DVD, and Memory Card support

## Installation

### Wii (Homebrew Channel)
1. Download the latest release from [Releases](https://github.com/Atomic-Germ/genplusGC/releases)
2. Extract to your SD card or USB drive
3. Launch from Homebrew Channel

### GameCube
1. Download the GameCube build from [Releases](https://github.com/Atomic-Germ/genplusGC/releases)
2. Use Swiss or other loader to run `genplus_cube.dol`

## Building

See [REFACTORING_PLAN.md](REFACTORING_PLAN.md) for current development status and build instructions.

```bash
# For Wii
make -f Makefile.wii

# For GameCube
make -f Makefile.gc
```

## Development

This project is undergoing active refactoring to modernize the codebase and optimize for GameCube/Wii. See our refactoring documentation:

- [REFACTORING_SUMMARY.md](REFACTORING_SUMMARY.md) - Quick overview
- [REFACTORING_PLAN.md](REFACTORING_PLAN.md) - Detailed plan
- [REFACTORING_PROGRESS.md](REFACTORING_PROGRESS.md) - Current status



----

## Credits

Based on [Genesis Plus GX](https://github.com/ekeeke/Genesis-Plus-GX) by Eke-Eke and the original Genesis Plus by Charles MacDonald. See [HISTORY.txt](HISTORY.txt) for full credits and changelog.

## License

Available under a non-commercial license. See [LICENSE.txt](LICENSE.txt) for details.