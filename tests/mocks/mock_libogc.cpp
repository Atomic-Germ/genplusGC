#include "mock_libogc.h"

/**
 * Mock libogc implementation for unit testing
 * Provides stub implementations of video modes and other global objects
 */

// Mock video mode objects
GXRModeObj TVNtsc480IntDf = {
    VI_TVMODE_NTSC_INT, // viTVMode
    640,                // fbWidth
    480,                // efbHeight
    480,                // xfbHeight
    0,                  // viXOrigin
    0,                  // viYOrigin
    640,                // viWidth
    480                 // viHeight
};

GXRModeObj TVNtsc480Prog = {
    VI_TVMODE_NTSC_PROG, // viTVMode
    640,                 // fbWidth
    480,                 // efbHeight
    480,                 // xfbHeight
    0,                   // viXOrigin
    0,                   // viYOrigin
    640,                 // viWidth
    480                  // viHeight
};

GXRModeObj TVPal528IntDf = {
    VI_TVMODE_PAL_INT, // viTVMode
    640,               // fbWidth
    528,               // efbHeight
    528,               // xfbHeight
    0,                 // viXOrigin
    0,                 // viYOrigin
    640,               // viWidth
    528                // viHeight
};

GXRModeObj TVEurgb60Hz480IntDf = {
    VI_TVMODE_EURGB60_INT, // viTVMode
    640,                   // fbWidth
    480,                   // efbHeight
    480,                   // xfbHeight
    0,                     // viXOrigin
    0,                     // viYOrigin
    640,                   // viWidth
    480                    // viHeight
};
