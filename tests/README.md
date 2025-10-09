# Genesis Plus GX Unit Tests

This directory contains unit tests for Genesis Plus GX GameCube/Wii emulator.

## Overview

The testing framework is a lightweight custom implementation based on the snes9xGC/fceuGC pattern. It provides:

- **Test registration and automatic discovery**
- **Rich assertion macros** with line numbers and detailed failure messages
- **Colored output** for easy result identification
- **No external dependencies** (uses only C++ standard library)
- **Fast execution** (<1 second for all tests)

## Directory Structure

```
tests/
├── framework/
│   └── simple_test.h       # Test framework implementation
├── mocks/
│   ├── mock_libogc.h       # Mock GameCube/Wii types and constants
│   └── mock_libogc.cpp     # Mock implementations
├── unit/
│   ├── test_main.cpp       # Test entry point
│   ├── test_dummy.cpp      # Framework verification tests
│   └── test_*.cpp          # Additional test files
├── Makefile                # Build system for tests
└── README.md               # This file
```

## Building and Running Tests

### Prerequisites

- **g++** (7.0 or later) with C++11 support
- **make**
- Standard UNIX tools (Linux, macOS, WSL)

### Quick Start

```bash
# Build and run all tests
make run

# Build only (no run)
make tests

# Clean build artifacts
make clean

# Rebuild from scratch
make rebuild
```

### Example Output

```
========================================
Genesis Plus GX Test Suite
========================================

Running: DummyTest_AlwaysPass ... PASSED
Running: DummyTest_BasicMath ... PASSED
Running: DummyTest_StringComparison ... PASSED
Running: DummyTest_PointerChecks ... PASSED
Running: DummyTest_MockLibOGC_Types ... PASSED
Running: DummyTest_MockLibOGC_ButtonConstants ... PASSED
Running: DummyTest_MockLibOGC_VideoMode ... PASSED

========================================
Test Results:
  Total:  7
  Passed: 7
  Failed: 0
========================================

All tests passed!
```

## Writing Tests

### Basic Test Structure

```cpp
#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

TEST(MyTestName) {
    // Test code here
    int result = 2 + 2;
    ASSERT_EQ(4, result);
}
```

### Available Assertions

- **`ASSERT_TRUE(condition)`** - Assert condition is true
- **`ASSERT_FALSE(condition)`** - Assert condition is false
- **`ASSERT_EQ(expected, actual)`** - Assert equality
- **`ASSERT_NE(not_expected, actual)`** - Assert inequality
- **`ASSERT_LT(val1, val2)`** - Assert less than
- **`ASSERT_LE(val1, val2)`** - Assert less than or equal
- **`ASSERT_GT(val1, val2)`** - Assert greater than
- **`ASSERT_GE(val1, val2)`** - Assert greater than or equal
- **`ASSERT_STREQ(str1, str2)`** - Assert string equality
- **`ASSERT_STRNE(str1, str2)`** - Assert string inequality
- **`ASSERT_NULL(ptr)`** - Assert pointer is null
- **`ASSERT_NOT_NULL(ptr)`** - Assert pointer is not null

### Example Tests

```cpp
// Testing configuration defaults
TEST(Config_DefaultValues) {
    // Assuming config.c exports functions for testing
    config_init();
    
    ASSERT_EQ(60, config_get_framerate());
    ASSERT_TRUE(config_get_auto_save());
}

// Testing file path handling
TEST(FileOp_PathStripping) {
    const char* path = "sd:/roms/game.md";
    char* stripped = strip_device_prefix(path);
    
    ASSERT_STREQ("/roms/game.md", stripped);
}

// Testing button mapping
TEST(ButtonMapping_GameCubePad) {
    u16 gc_buttons = PAD_BUTTON_A | PAD_BUTTON_B;
    u16 genesis_buttons = map_gc_to_genesis(gc_buttons);
    
    ASSERT_TRUE(genesis_buttons & GENESIS_A);
    ASSERT_TRUE(genesis_buttons & GENESIS_B);
}
```

## Mock libogc

The `mock_libogc.h/cpp` files provide mock implementations of GameCube/Wii types and constants, allowing tests to run on standard development machines without requiring the actual libogc library.

### Mocked Components

- **Basic types**: `u8`, `u16`, `u32`, `s8`, `s16`, `s32`, etc.
- **PAD constants**: `PAD_BUTTON_A`, `PAD_TRIGGER_Z`, etc.
- **WPAD constants**: `WPAD_BUTTON_A`, `WPAD_CLASSIC_BUTTON_X`, etc.
- **Video modes**: `TVNtsc480IntDf`, `TVPal528IntDf`, etc.
- **Other constants**: `CARD_WORKAREA_SIZE`, etc.

## Test Organization

### Current Test Files

1. **`test_dummy.cpp`** - Framework verification tests
   - Basic assertion testing
   - Mock libogc validation
   - Framework functionality checks

### Planned Test Files (Phase 2.3)

1. **`test_config.cpp`** - Configuration system tests
2. **`test_fileop.cpp`** - File operation tests
3. **`test_button_mapping.cpp`** - Controller mapping tests
4. **`test_video_mode.cpp`** - Video mode selection tests

## CI Integration

Tests are automatically run in GitHub Actions before building the emulator.

See `.github/workflows/build.yml` for CI configuration.

## Advanced Features

### Static Analysis

```bash
# Run cppcheck (if installed)
make static-analysis
```

### Code Coverage

```bash
# Generate coverage report (requires lcov)
make coverage

# View report
open build/coverage_html/index.html
```

## Testing Philosophy

### What to Test

- **Pure logic functions** - Configuration, parsing, calculations
- **State management** - Save states, settings, memory
- **Input handling** - Button mapping, controller detection
- **File operations** - Path manipulation, extension validation
- **Edge cases** - Boundary conditions, error handling

### What Not to Test

- **Hardware-specific code** - GX rendering, actual I/O
- **Emulation core** - CPU emulation (tested separately)
- **GUI rendering** - Visual components (manual testing)

### Test Guidelines

- **Fast**: Each test should run in <10ms
- **Independent**: Tests should not depend on each other
- **Focused**: One test per behavior/edge case
- **Clear**: Test names should describe what they test
- **Maintainable**: Use helper functions for common setup

## Troubleshooting

### Tests won't compile

```bash
# Check g++ version (need 7.0+)
g++ --version

# Clean and rebuild
make clean
make tests
```

### Tests run but fail

- Check test output for specific failure messages
- Each failed test shows file:line and expected vs actual values
- Run individual test files if needed

### Make not found

```bash
# macOS
xcode-select --install

# Linux (Ubuntu/Debian)
sudo apt-get install build-essential

# Windows
# Use WSL or MSYS2
```

## Contributing Tests

When adding new tests:

1. Create a new `test_*.cpp` file in `unit/`
2. Include the test framework and mocks
3. Write tests using `TEST(TestName)` macro
4. Use appropriate assertions
5. Run `make run` to verify
6. Tests are automatically discovered and run

## References

- **Test Framework**: Based on snes9xGC/fceuGC pattern
- **Best Practices**: Google Test-style assertions
- **CI/CD**: GitHub Actions integration

---

**Last Updated**: 2024  
**Test Count**: 7 framework verification tests  
**Status**: Phase 2 - Initial Setup
