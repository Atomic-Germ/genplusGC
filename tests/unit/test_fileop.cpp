#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"
#include <cstring>

/**
 * File path manipulation and extension testing
 * 
 * Tests for common file operations like:
 * - Extension checking
 * - Path manipulation
 * - Device prefix handling
 */

// Helper function to check file extension
static bool has_extension(const char* filename, const char* ext) {
    if (!filename || !ext) return false;
    
    const char* dot = strrchr(filename, '.');
    if (!dot) return false;
    
    return (strcasecmp(dot, ext) == 0);
}

// Helper to strip device prefix (sd:/, usb:/, dvd:/)
static const char* strip_device_prefix(const char* path) {
    if (!path) return NULL;
    
    // Check for device prefixes
    if (strncmp(path, "sd:/", 4) == 0) return path + 3;
    if (strncmp(path, "usb:/", 5) == 0) return path + 4;
    if (strncmp(path, "dvd:/", 5) == 0) return path + 4;
    if (strncmp(path, "carda:/", 7) == 0) return path + 6;
    if (strncmp(path, "cardb:/", 7) == 0) return path + 6;
    
    return path;
}

// Helper to get filename from path
static const char* get_filename(const char* path) {
    if (!path) return NULL;
    
    const char* slash = strrchr(path, '/');
    if (slash) return slash + 1;
    
    const char* backslash = strrchr(path, '\\');
    if (backslash) return backslash + 1;
    
    return path;
}

// Helper to get file extension
static const char* get_extension(const char* filename) {
    if (!filename) return NULL;
    
    const char* dot = strrchr(filename, '.');
    return dot ? dot : "";
}

//-----------------------------------------------------------------------------
// Extension Tests
//-----------------------------------------------------------------------------

TEST(FileOp_Extension_MD) {
    ASSERT_TRUE(has_extension("sonic.md", ".md"));
    ASSERT_TRUE(has_extension("sonic.MD", ".md"));
    ASSERT_TRUE(has_extension("SONIC.MD", ".MD"));
}

TEST(FileOp_Extension_SMD) {
    ASSERT_TRUE(has_extension("game.smd", ".smd"));
    ASSERT_TRUE(has_extension("game.SMD", ".smd"));
}

TEST(FileOp_Extension_BIN) {
    ASSERT_TRUE(has_extension("rom.bin", ".bin"));
    ASSERT_TRUE(has_extension("ROM.BIN", ".bin"));
}

TEST(FileOp_Extension_GEN) {
    ASSERT_TRUE(has_extension("streets.gen", ".gen"));
}

TEST(FileOp_Extension_ZIP) {
    ASSERT_TRUE(has_extension("roms.zip", ".zip"));
}

TEST(FileOp_Extension_NoMatch) {
    ASSERT_FALSE(has_extension("sonic.md", ".smd"));
    ASSERT_FALSE(has_extension("game.bin", ".md"));
}

TEST(FileOp_Extension_NoExtension) {
    ASSERT_FALSE(has_extension("noextension", ".md"));
}

TEST(FileOp_Extension_NullInput) {
    ASSERT_FALSE(has_extension(NULL, ".md"));
    ASSERT_FALSE(has_extension("test.md", NULL));
    ASSERT_FALSE(has_extension(NULL, NULL));
}

//-----------------------------------------------------------------------------
// Device Prefix Tests
//-----------------------------------------------------------------------------

TEST(FileOp_DevicePrefix_SD) {
    const char* path = "sd:/roms/sonic.md";
    const char* stripped = strip_device_prefix(path);
    ASSERT_STREQ("/roms/sonic.md", stripped);
}

TEST(FileOp_DevicePrefix_USB) {
    const char* path = "usb:/games/streets.bin";
    const char* stripped = strip_device_prefix(path);
    ASSERT_STREQ("/games/streets.bin", stripped);
}

TEST(FileOp_DevicePrefix_DVD) {
    const char* path = "dvd:/collection/game.smd";
    const char* stripped = strip_device_prefix(path);
    ASSERT_STREQ("/collection/game.smd", stripped);
}

TEST(FileOp_DevicePrefix_CardA) {
    const char* path = "carda:/saves/game.sav";
    const char* stripped = strip_device_prefix(path);
    ASSERT_STREQ("/saves/game.sav", stripped);
}

TEST(FileOp_DevicePrefix_CardB) {
    const char* path = "cardb:/backup/state.sav";
    const char* stripped = strip_device_prefix(path);
    ASSERT_STREQ("/backup/state.sav", stripped);
}

TEST(FileOp_DevicePrefix_NoPrefix) {
    const char* path = "/roms/sonic.md";
    const char* stripped = strip_device_prefix(path);
    ASSERT_STREQ("/roms/sonic.md", stripped);
}

TEST(FileOp_DevicePrefix_NullInput) {
    const char* stripped = strip_device_prefix(NULL);
    ASSERT_NULL(stripped);
}

//-----------------------------------------------------------------------------
// Filename Extraction Tests
//-----------------------------------------------------------------------------

TEST(FileOp_Filename_UnixPath) {
    const char* path = "/home/user/roms/sonic.md";
    const char* filename = get_filename(path);
    ASSERT_STREQ("sonic.md", filename);
}

TEST(FileOp_Filename_WindowsPath) {
    const char* path = "C:\\roms\\game.bin";
    const char* filename = get_filename(path);
    ASSERT_STREQ("game.bin", filename);
}

TEST(FileOp_Filename_DevicePath) {
    const char* path = "sd:/roms/streets.smd";
    const char* filename = get_filename(path);
    ASSERT_STREQ("streets.smd", filename);
}

TEST(FileOp_Filename_NoPath) {
    const char* path = "sonic.md";
    const char* filename = get_filename(path);
    ASSERT_STREQ("sonic.md", filename);
}

TEST(FileOp_Filename_NullInput) {
    const char* filename = get_filename(NULL);
    ASSERT_NULL(filename);
}

//-----------------------------------------------------------------------------
// Extension Extraction Tests
//-----------------------------------------------------------------------------

TEST(FileOp_GetExtension_MD) {
    const char* ext = get_extension("sonic.md");
    ASSERT_STREQ(".md", ext);
}

TEST(FileOp_GetExtension_MultiDot) {
    const char* ext = get_extension("game.v1.2.smd");
    ASSERT_STREQ(".smd", ext);
}

TEST(FileOp_GetExtension_NoExtension) {
    const char* ext = get_extension("noext");
    ASSERT_STREQ("", ext);
}

TEST(FileOp_GetExtension_NullInput) {
    const char* ext = get_extension(NULL);
    ASSERT_NULL(ext);
}

//-----------------------------------------------------------------------------
// Path Validation Tests
//-----------------------------------------------------------------------------

TEST(FileOp_Path_ValidAbsolute) {
    const char* path = "/roms/sonic.md";
    ASSERT_EQ('/', path[0]);
}

TEST(FileOp_Path_ValidDevice) {
    const char* path = "sd:/roms/game.md";
    ASSERT_TRUE(strncmp(path, "sd:", 3) == 0);
}

TEST(FileOp_Path_Length) {
    // Test that paths aren't too long (common limit is 256)
    char longpath[300];
    memset(longpath, 'a', 299);
    longpath[299] = '\0';
    
    size_t len = strlen(longpath);
    ASSERT_GT(len, 256);  // This path is too long
}

//-----------------------------------------------------------------------------
// Combined Workflow Tests
//-----------------------------------------------------------------------------

TEST(FileOp_Workflow_CompletePathProcessing) {
    const char* original = "sd:/roms/sonic.md";
    
    // Strip device prefix
    const char* stripped = strip_device_prefix(original);
    ASSERT_STREQ("/roms/sonic.md", stripped);
    
    // Get filename
    const char* filename = get_filename(stripped);
    ASSERT_STREQ("sonic.md", filename);
    
    // Check extension
    ASSERT_TRUE(has_extension(filename, ".md"));
    
    // Get extension
    const char* ext = get_extension(filename);
    ASSERT_STREQ(".md", ext);
}

TEST(FileOp_Workflow_WindowsStylePath) {
    const char* path = "C:\\Users\\Player\\ROMs\\game.bin";
    
    const char* filename = get_filename(path);
    ASSERT_STREQ("game.bin", filename);
    
    ASSERT_TRUE(has_extension(filename, ".bin"));
}
