#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

/**
 * Framework verification tests
 * These tests verify that the testing framework itself is working correctly
 */

TEST(DummyTest_AlwaysPass) {
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_EQ(1, 1);
    ASSERT_NE(1, 2);
}

TEST(DummyTest_BasicMath) {
    int result = 2 + 2;
    ASSERT_EQ(4, result);
    ASSERT_NE(5, result);
    ASSERT_GT(result, 3);
    ASSERT_LT(result, 5);
}

TEST(DummyTest_StringComparison) {
    const char* str1 = "hello";
    const char* str2 = "hello";
    const char* str3 = "world";
    
    ASSERT_STREQ(str1, str2);
    ASSERT_STRNE(str1, str3);
}

TEST(DummyTest_PointerChecks) {
    int* nullPtr = nullptr;
    int value = 42;
    int* validPtr = &value;
    
    ASSERT_NULL(nullPtr);
    ASSERT_NOT_NULL(validPtr);
}

TEST(DummyTest_MockLibOGC_Types) {
    u8 byte = 255;
    u16 word = 65535;
    u32 dword = 0xFFFFFFFF;
    
    ASSERT_EQ(255, byte);
    ASSERT_EQ(65535, word);
    ASSERT_EQ(0xFFFFFFFF, dword);
}

TEST(DummyTest_MockLibOGC_ButtonConstants) {
    u16 buttons = PAD_BUTTON_A | PAD_BUTTON_B;
    
    ASSERT_TRUE(buttons & PAD_BUTTON_A);
    ASSERT_TRUE(buttons & PAD_BUTTON_B);
    ASSERT_FALSE(buttons & PAD_BUTTON_X);
}

TEST(DummyTest_MockLibOGC_VideoMode) {
    ASSERT_EQ(VI_TVMODE_NTSC_INT, TVNtsc480IntDf.viTVMode);
    ASSERT_EQ(640, TVNtsc480IntDf.fbWidth);
    ASSERT_EQ(480, TVNtsc480IntDf.efbHeight);
}
