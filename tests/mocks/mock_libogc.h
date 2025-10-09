#ifndef MOCK_LIBOGC_H
#define MOCK_LIBOGC_H

/**
 * Mock libogc types and constants for unit testing
 * This allows tests to run on standard Linux/macOS/Windows without requiring
 * the actual GameCube/Wii libogc library
 */

#include <stdint.h>
#include <stddef.h>

// Basic types from gctypes.h
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float  f32;
typedef double f64;

typedef volatile f32 vf32;
typedef volatile f64 vf64;

// NULL
#ifndef NULL
#define NULL ((void*)0)
#endif

// Alignment attribute (ignored in mock)
#define ATTRIBUTE_ALIGN(x)

// PAD button constants (from pad.h)
#define PAD_BUTTON_LEFT   0x0001
#define PAD_BUTTON_RIGHT  0x0002
#define PAD_BUTTON_DOWN   0x0004
#define PAD_BUTTON_UP     0x0008
#define PAD_TRIGGER_Z     0x0010
#define PAD_TRIGGER_R     0x0020
#define PAD_TRIGGER_L     0x0040
#define PAD_BUTTON_A      0x0100
#define PAD_BUTTON_B      0x0200
#define PAD_BUTTON_X      0x0400
#define PAD_BUTTON_Y      0x0800
#define PAD_BUTTON_START  0x1000

// WPAD button constants (Wii only, from wpad.h)
#define WPAD_BUTTON_2     0x0001
#define WPAD_BUTTON_1     0x0002
#define WPAD_BUTTON_B     0x0004
#define WPAD_BUTTON_A     0x0008
#define WPAD_BUTTON_MINUS 0x0010
#define WPAD_BUTTON_HOME  0x0080
#define WPAD_BUTTON_LEFT  0x0100
#define WPAD_BUTTON_RIGHT 0x0200
#define WPAD_BUTTON_DOWN  0x0400
#define WPAD_BUTTON_UP    0x0800
#define WPAD_BUTTON_PLUS  0x1000

// Classic Controller constants
#define WPAD_CLASSIC_BUTTON_UP    0x0001
#define WPAD_CLASSIC_BUTTON_LEFT  0x0002
#define WPAD_CLASSIC_BUTTON_ZR    0x0004
#define WPAD_CLASSIC_BUTTON_X     0x0008
#define WPAD_CLASSIC_BUTTON_A     0x0010
#define WPAD_CLASSIC_BUTTON_Y     0x0020
#define WPAD_CLASSIC_BUTTON_B     0x0040
#define WPAD_CLASSIC_BUTTON_ZL    0x0080
#define WPAD_CLASSIC_BUTTON_FULL_R  0x0200
#define WPAD_CLASSIC_BUTTON_PLUS    0x0400
#define WPAD_CLASSIC_BUTTON_HOME    0x0800
#define WPAD_CLASSIC_BUTTON_MINUS   0x1000
#define WPAD_CLASSIC_BUTTON_FULL_L  0x2000
#define WPAD_CLASSIC_BUTTON_DOWN    0x4000
#define WPAD_CLASSIC_BUTTON_RIGHT   0x8000

// Nunchuk constants
#define WPAD_NUNCHUK_BUTTON_Z 0x0001
#define WPAD_NUNCHUK_BUTTON_C 0x0002

// Video mode constants (from video.h)
#define VI_TVMODE_NTSC_INT      0
#define VI_TVMODE_NTSC_DS       1
#define VI_TVMODE_NTSC_PROG     2
#define VI_TVMODE_PAL_INT       3
#define VI_TVMODE_PAL_DS        4
#define VI_TVMODE_EURGB60_INT   5
#define VI_TVMODE_EURGB60_DS    6

// GX render mode structure (simplified mock)
typedef struct _GXRModeObj {
    u32 viTVMode;
    u16 fbWidth;
    u16 efbHeight;
    u16 xfbHeight;
    u16 viXOrigin;
    u16 viYOrigin;
    u16 viWidth;
    u16 viHeight;
    // ... other fields omitted for mock
} GXRModeObj;

// Mock video modes (simplified)
extern GXRModeObj TVNtsc480IntDf;
extern GXRModeObj TVNtsc480Prog;
extern GXRModeObj TVPal528IntDf;
extern GXRModeObj TVEurgb60Hz480IntDf;

// CARD (Memory Card) constants
#define CARD_WORKAREA_SIZE (5*8*1024)

// File system constants
#define DEVOPTAB_MAX_NAME_LEN 32

#ifdef __cplusplus
extern "C" {
#endif

// Mock function declarations (not implemented in header-only mock)
// These would be implemented in mock_libogc.cpp if needed

#ifdef __cplusplus
}
#endif

#endif // MOCK_LIBOGC_H
