#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

/**
 * Video mode selection and validation tests
 * 
 * Tests for:
 * - NTSC/PAL detection
 * - Progressive/interlaced modes
 * - Resolution validation
 * - Video mode structures
 */

//-----------------------------------------------------------------------------
// Video Mode Constant Tests
//-----------------------------------------------------------------------------

TEST(VideoMode_Constants_Unique) {
    // Each video mode should have a unique value
    ASSERT_NE(VI_TVMODE_NTSC_INT, VI_TVMODE_NTSC_DS);
    ASSERT_NE(VI_TVMODE_NTSC_INT, VI_TVMODE_NTSC_PROG);
    ASSERT_NE(VI_TVMODE_NTSC_INT, VI_TVMODE_PAL_INT);
}

TEST(VideoMode_Constants_NTSC) {
    // NTSC modes
    ASSERT_EQ(0, VI_TVMODE_NTSC_INT);
    ASSERT_TRUE(VI_TVMODE_NTSC_DS >= 0);
    ASSERT_TRUE(VI_TVMODE_NTSC_PROG >= 0);
}

TEST(VideoMode_Constants_PAL) {
    // PAL modes
    ASSERT_TRUE(VI_TVMODE_PAL_INT >= 0);
    ASSERT_TRUE(VI_TVMODE_PAL_DS >= 0);
}

TEST(VideoMode_Constants_RGB) {
    // RGB60 modes
    ASSERT_TRUE(VI_TVMODE_EURGB60_INT >= 0);
    ASSERT_TRUE(VI_TVMODE_EURGB60_DS >= 0);
}

//-----------------------------------------------------------------------------
// Video Mode Structure Tests
//-----------------------------------------------------------------------------

TEST(VideoMode_Structure_NTSC480i) {
    ASSERT_EQ(VI_TVMODE_NTSC_INT, TVNtsc480IntDf.viTVMode);
    ASSERT_EQ(640, TVNtsc480IntDf.fbWidth);
    ASSERT_EQ(480, TVNtsc480IntDf.efbHeight);
    ASSERT_EQ(480, TVNtsc480IntDf.xfbHeight);
}

TEST(VideoMode_Structure_NTSC480p) {
    ASSERT_EQ(VI_TVMODE_NTSC_PROG, TVNtsc480Prog.viTVMode);
    ASSERT_EQ(640, TVNtsc480Prog.fbWidth);
    ASSERT_EQ(480, TVNtsc480Prog.efbHeight);
}

TEST(VideoMode_Structure_PAL576i) {
    ASSERT_EQ(VI_TVMODE_PAL_INT, TVPal528IntDf.viTVMode);
    ASSERT_EQ(640, TVPal528IntDf.fbWidth);
    ASSERT_EQ(528, TVPal528IntDf.efbHeight);
}

TEST(VideoMode_Structure_RGB60) {
    ASSERT_EQ(VI_TVMODE_EURGB60_INT, TVEurgb60Hz480IntDf.viTVMode);
    ASSERT_EQ(640, TVEurgb60Hz480IntDf.fbWidth);
    ASSERT_EQ(480, TVEurgb60Hz480IntDf.efbHeight);
}

//-----------------------------------------------------------------------------
// Resolution Tests
//-----------------------------------------------------------------------------

TEST(VideoMode_Resolution_StandardWidth) {
    // Standard width is 640
    ASSERT_EQ(640, TVNtsc480IntDf.fbWidth);
    ASSERT_EQ(640, TVNtsc480Prog.fbWidth);
    ASSERT_EQ(640, TVPal528IntDf.fbWidth);
}

TEST(VideoMode_Resolution_NTSCHeight) {
    // NTSC modes are 480 lines
    ASSERT_EQ(480, TVNtsc480IntDf.efbHeight);
    ASSERT_EQ(480, TVNtsc480Prog.efbHeight);
    ASSERT_EQ(480, TVEurgb60Hz480IntDf.efbHeight);
}

TEST(VideoMode_Resolution_PALHeight) {
    // PAL mode is 528 lines (576 visible)
    ASSERT_EQ(528, TVPal528IntDf.efbHeight);
}

TEST(VideoMode_Resolution_FramebufferMatch) {
    // Frame buffer height should match EFB height for these modes
    ASSERT_EQ(TVNtsc480IntDf.efbHeight, TVNtsc480IntDf.xfbHeight);
    ASSERT_EQ(TVNtsc480Prog.efbHeight, TVNtsc480Prog.xfbHeight);
}

//-----------------------------------------------------------------------------
// Mode Detection Helper Tests
//-----------------------------------------------------------------------------

// Helper function to detect if mode is NTSC
static bool is_ntsc_mode(u32 mode) {
    return (mode == VI_TVMODE_NTSC_INT || 
            mode == VI_TVMODE_NTSC_DS ||
            mode == VI_TVMODE_NTSC_PROG);
}

// Helper function to detect if mode is PAL
static bool is_pal_mode(u32 mode) {
    return (mode == VI_TVMODE_PAL_INT || 
            mode == VI_TVMODE_PAL_DS);
}

// Helper function to detect if mode is progressive
static bool is_progressive_mode(u32 mode) {
    return (mode == VI_TVMODE_NTSC_PROG);
}

// Helper function to get vertical resolution
static u16 get_vertical_resolution(const GXRModeObj* mode) {
    if (!mode) return 0;
    return mode->efbHeight;
}

TEST(VideoMode_Detection_NTSC) {
    ASSERT_TRUE(is_ntsc_mode(VI_TVMODE_NTSC_INT));
    ASSERT_TRUE(is_ntsc_mode(VI_TVMODE_NTSC_DS));
    ASSERT_TRUE(is_ntsc_mode(VI_TVMODE_NTSC_PROG));
    ASSERT_FALSE(is_ntsc_mode(VI_TVMODE_PAL_INT));
}

TEST(VideoMode_Detection_PAL) {
    ASSERT_TRUE(is_pal_mode(VI_TVMODE_PAL_INT));
    ASSERT_TRUE(is_pal_mode(VI_TVMODE_PAL_DS));
    ASSERT_FALSE(is_pal_mode(VI_TVMODE_NTSC_INT));
}

TEST(VideoMode_Detection_Progressive) {
    ASSERT_TRUE(is_progressive_mode(VI_TVMODE_NTSC_PROG));
    ASSERT_FALSE(is_progressive_mode(VI_TVMODE_NTSC_INT));
    ASSERT_FALSE(is_progressive_mode(VI_TVMODE_PAL_INT));
}

TEST(VideoMode_Detection_Resolution) {
    ASSERT_EQ(480, get_vertical_resolution(&TVNtsc480IntDf));
    ASSERT_EQ(480, get_vertical_resolution(&TVNtsc480Prog));
    ASSERT_EQ(528, get_vertical_resolution(&TVPal528IntDf));
}

//-----------------------------------------------------------------------------
// Aspect Ratio Tests
//-----------------------------------------------------------------------------

// Helper to calculate aspect ratio (width/height)
static float get_aspect_ratio(const GXRModeObj* mode) {
    if (!mode || mode->efbHeight == 0) return 0.0f;
    return (float)mode->fbWidth / (float)mode->efbHeight;
}

TEST(VideoMode_AspectRatio_NTSC) {
    float ratio = get_aspect_ratio(&TVNtsc480IntDf);
    // 640/480 = 1.333... (4:3)
    ASSERT_GT(ratio, 1.3f);
    ASSERT_LT(ratio, 1.4f);
}

TEST(VideoMode_AspectRatio_PAL) {
    float ratio = get_aspect_ratio(&TVPal528IntDf);
    // 640/528 ≈ 1.21 (but PAL uses different pixel aspect ratio)
    ASSERT_GT(ratio, 1.2f);
    ASSERT_LT(ratio, 1.3f);
}

TEST(VideoMode_AspectRatio_Progressive) {
    float ratio = get_aspect_ratio(&TVNtsc480Prog);
    // Same as interlaced NTSC
    ASSERT_GT(ratio, 1.3f);
    ASSERT_LT(ratio, 1.4f);
}

//-----------------------------------------------------------------------------
// Mode Compatibility Tests
//-----------------------------------------------------------------------------

// Helper to check if mode is valid for GameCube
static bool is_gamecube_compatible(u32 mode) {
    // GameCube supports all modes except progressive requires component cable
    (void)mode;  // Suppress unused parameter warning
    return true;  // All defined modes are compatible
}

// Helper to check if mode is valid for Wii
static bool is_wii_compatible(u32 mode) {
    // Wii supports all modes
    (void)mode;  // Suppress unused parameter warning
    return true;
}

TEST(VideoMode_Compatibility_GameCube) {
    ASSERT_TRUE(is_gamecube_compatible(VI_TVMODE_NTSC_INT));
    ASSERT_TRUE(is_gamecube_compatible(VI_TVMODE_PAL_INT));
    ASSERT_TRUE(is_gamecube_compatible(VI_TVMODE_NTSC_PROG));
}

TEST(VideoMode_Compatibility_Wii) {
    ASSERT_TRUE(is_wii_compatible(VI_TVMODE_NTSC_INT));
    ASSERT_TRUE(is_wii_compatible(VI_TVMODE_PAL_INT));
    ASSERT_TRUE(is_wii_compatible(VI_TVMODE_NTSC_PROG));
}

//-----------------------------------------------------------------------------
// Refresh Rate Tests
//-----------------------------------------------------------------------------

// Helper to get refresh rate (NTSC=60Hz, PAL=50Hz)
static u8 get_refresh_rate(u32 mode) {
    if (is_pal_mode(mode)) return 50;
    return 60;
}

TEST(VideoMode_RefreshRate_NTSC) {
    ASSERT_EQ(60, get_refresh_rate(VI_TVMODE_NTSC_INT));
    ASSERT_EQ(60, get_refresh_rate(VI_TVMODE_NTSC_PROG));
}

TEST(VideoMode_RefreshRate_PAL) {
    ASSERT_EQ(50, get_refresh_rate(VI_TVMODE_PAL_INT));
    ASSERT_EQ(50, get_refresh_rate(VI_TVMODE_PAL_DS));
}

TEST(VideoMode_RefreshRate_RGB60) {
    ASSERT_EQ(60, get_refresh_rate(VI_TVMODE_EURGB60_INT));
}

//-----------------------------------------------------------------------------
// Mode Selection Logic Tests
//-----------------------------------------------------------------------------

// Helper to select best mode based on region
static u32 select_mode_for_region(bool is_pal_region, bool progressive_supported) {
    if (is_pal_region) {
        return VI_TVMODE_PAL_INT;
    } else {
        if (progressive_supported) {
            return VI_TVMODE_NTSC_PROG;
        } else {
            return VI_TVMODE_NTSC_INT;
        }
    }
}

TEST(VideoMode_Selection_NTSC_Interlaced) {
    u32 mode = select_mode_for_region(false, false);
    ASSERT_EQ(VI_TVMODE_NTSC_INT, mode);
}

TEST(VideoMode_Selection_NTSC_Progressive) {
    u32 mode = select_mode_for_region(false, true);
    ASSERT_EQ(VI_TVMODE_NTSC_PROG, mode);
}

TEST(VideoMode_Selection_PAL) {
    u32 mode = select_mode_for_region(true, false);
    ASSERT_EQ(VI_TVMODE_PAL_INT, mode);
}

TEST(VideoMode_Selection_PAL_NoProgressive) {
    // PAL doesn't have progressive mode in our mock
    u32 mode = select_mode_for_region(true, true);
    ASSERT_EQ(VI_TVMODE_PAL_INT, mode);
}

//-----------------------------------------------------------------------------
// Origin and Dimensions Tests
//-----------------------------------------------------------------------------

TEST(VideoMode_Origin_Default) {
    // Default origin should be 0,0
    ASSERT_EQ(0, TVNtsc480IntDf.viXOrigin);
    ASSERT_EQ(0, TVNtsc480IntDf.viYOrigin);
    ASSERT_EQ(0, TVPal528IntDf.viXOrigin);
    ASSERT_EQ(0, TVPal528IntDf.viYOrigin);
}

TEST(VideoMode_Dimensions_Valid) {
    // VI dimensions should match framebuffer
    ASSERT_EQ(640, TVNtsc480IntDf.viWidth);
    ASSERT_EQ(480, TVNtsc480IntDf.viHeight);
    ASSERT_EQ(640, TVPal528IntDf.viWidth);
    ASSERT_EQ(528, TVPal528IntDf.viHeight);
}
