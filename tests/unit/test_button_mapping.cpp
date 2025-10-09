#include "../framework/simple_test.h"
#include "../mocks/mock_libogc.h"

/**
 * Button mapping and controller tests
 * 
 * Tests for:
 * - Button constant validation
 * - Controller type detection
 * - Button combinations
 * - Mapping logic
 */

//-----------------------------------------------------------------------------
// GameCube Controller Button Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_GC_SingleButtons) {
    u16 buttons = PAD_BUTTON_A;
    ASSERT_TRUE(buttons & PAD_BUTTON_A);
    ASSERT_FALSE(buttons & PAD_BUTTON_B);
}

TEST(ButtonMapping_GC_Combinations) {
    u16 buttons = PAD_BUTTON_A | PAD_BUTTON_B;
    ASSERT_TRUE(buttons & PAD_BUTTON_A);
    ASSERT_TRUE(buttons & PAD_BUTTON_B);
    ASSERT_FALSE(buttons & PAD_BUTTON_X);
}

TEST(ButtonMapping_GC_DPad) {
    u16 buttons = PAD_BUTTON_UP | PAD_BUTTON_DOWN;
    ASSERT_TRUE(buttons & PAD_BUTTON_UP);
    ASSERT_TRUE(buttons & PAD_BUTTON_DOWN);
}

TEST(ButtonMapping_GC_Triggers) {
    u16 buttons = PAD_TRIGGER_L | PAD_TRIGGER_R;
    ASSERT_TRUE(buttons & PAD_TRIGGER_L);
    ASSERT_TRUE(buttons & PAD_TRIGGER_R);
    // Z trigger is separate, not included unless explicitly added
}

TEST(ButtonMapping_GC_AllButtons) {
    u16 all = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y |
              PAD_BUTTON_START | PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT |
              PAD_BUTTON_UP | PAD_BUTTON_DOWN | PAD_TRIGGER_Z |
              PAD_TRIGGER_L | PAD_TRIGGER_R;
    
    ASSERT_TRUE(all & PAD_BUTTON_A);
    ASSERT_TRUE(all & PAD_BUTTON_START);
    ASSERT_TRUE(all & PAD_TRIGGER_L);
}

TEST(ButtonMapping_GC_NoButtons) {
    u16 buttons = 0;
    ASSERT_FALSE(buttons & PAD_BUTTON_A);
    ASSERT_FALSE(buttons & PAD_BUTTON_B);
    ASSERT_EQ(0, buttons);
}

//-----------------------------------------------------------------------------
// Wii Controller Button Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_Wii_WiimoteBasic) {
    u16 buttons = WPAD_BUTTON_A | WPAD_BUTTON_B;
    ASSERT_TRUE(buttons & WPAD_BUTTON_A);
    ASSERT_TRUE(buttons & WPAD_BUTTON_B);
}

TEST(ButtonMapping_Wii_WiimoteDPad) {
    u16 buttons = WPAD_BUTTON_UP | WPAD_BUTTON_DOWN | 
                  WPAD_BUTTON_LEFT | WPAD_BUTTON_RIGHT;
    ASSERT_TRUE(buttons & WPAD_BUTTON_UP);
    ASSERT_TRUE(buttons & WPAD_BUTTON_DOWN);
    ASSERT_TRUE(buttons & WPAD_BUTTON_LEFT);
    ASSERT_TRUE(buttons & WPAD_BUTTON_RIGHT);
}

TEST(ButtonMapping_Wii_WiimoteHomeMenu) {
    u16 buttons = WPAD_BUTTON_HOME | WPAD_BUTTON_MINUS | WPAD_BUTTON_PLUS;
    ASSERT_TRUE(buttons & WPAD_BUTTON_HOME);
    ASSERT_TRUE(buttons & WPAD_BUTTON_MINUS);
    ASSERT_TRUE(buttons & WPAD_BUTTON_PLUS);
}

TEST(ButtonMapping_Wii_Classic) {
    u16 buttons = WPAD_CLASSIC_BUTTON_A | WPAD_CLASSIC_BUTTON_B;
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_A);
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_B);
}

TEST(ButtonMapping_Wii_ClassicDPad) {
    u16 buttons = WPAD_CLASSIC_BUTTON_UP | WPAD_CLASSIC_BUTTON_DOWN |
                  WPAD_CLASSIC_BUTTON_LEFT | WPAD_CLASSIC_BUTTON_RIGHT;
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_UP);
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_DOWN);
}

TEST(ButtonMapping_Wii_ClassicShoulders) {
    u16 buttons = WPAD_CLASSIC_BUTTON_FULL_L | WPAD_CLASSIC_BUTTON_FULL_R |
                  WPAD_CLASSIC_BUTTON_ZL | WPAD_CLASSIC_BUTTON_ZR;
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_FULL_L);
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_FULL_R);
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_ZL);
    ASSERT_TRUE(buttons & WPAD_CLASSIC_BUTTON_ZR);
}

TEST(ButtonMapping_Wii_Nunchuk) {
    u16 buttons = WPAD_NUNCHUK_BUTTON_Z | WPAD_NUNCHUK_BUTTON_C;
    ASSERT_TRUE(buttons & WPAD_NUNCHUK_BUTTON_Z);
    ASSERT_TRUE(buttons & WPAD_NUNCHUK_BUTTON_C);
}

//-----------------------------------------------------------------------------
// Button Masking Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_Mask_SingleButton) {
    u16 input = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X;
    u16 mask = PAD_BUTTON_A;
    
    u16 result = input & mask;
    ASSERT_TRUE(result == PAD_BUTTON_A);
}

TEST(ButtonMapping_Mask_Multiple) {
    u16 input = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y;
    u16 mask = PAD_BUTTON_A | PAD_BUTTON_B;
    
    u16 result = input & mask;
    ASSERT_TRUE(result & PAD_BUTTON_A);
    ASSERT_TRUE(result & PAD_BUTTON_B);
    ASSERT_FALSE(result & PAD_BUTTON_X);
}

TEST(ButtonMapping_Mask_None) {
    u16 input = PAD_BUTTON_A | PAD_BUTTON_B;
    u16 mask = PAD_BUTTON_X | PAD_BUTTON_Y;
    
    u16 result = input & mask;
    ASSERT_EQ(0, result);
}

//-----------------------------------------------------------------------------
// Button State Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_State_Pressed) {
    u16 current = PAD_BUTTON_A;
    u16 previous = 0;
    
    // Button is newly pressed
    u16 pressed = current & ~previous;
    ASSERT_TRUE(pressed & PAD_BUTTON_A);
}

TEST(ButtonMapping_State_Released) {
    u16 current = 0;
    u16 previous = PAD_BUTTON_A;
    
    // Button was released
    u16 released = previous & ~current;
    ASSERT_TRUE(released & PAD_BUTTON_A);
}

TEST(ButtonMapping_State_Held) {
    u16 current = PAD_BUTTON_A;
    u16 previous = PAD_BUTTON_A;
    
    // Button is held
    u16 held = current & previous;
    ASSERT_TRUE(held & PAD_BUTTON_A);
}

TEST(ButtonMapping_State_Changed) {
    u16 current = PAD_BUTTON_A | PAD_BUTTON_B;
    u16 previous = PAD_BUTTON_A | PAD_BUTTON_X;
    
    // Buttons that changed state
    u16 changed = current ^ previous;
    ASSERT_TRUE(changed & PAD_BUTTON_B);   // newly pressed
    ASSERT_TRUE(changed & PAD_BUTTON_X);   // released
    ASSERT_FALSE(changed & PAD_BUTTON_A);  // still held
}

//-----------------------------------------------------------------------------
// Button Combination Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_Combo_TwoButtons) {
    u16 buttons = PAD_BUTTON_A | PAD_BUTTON_B;
    u16 combo = PAD_BUTTON_A | PAD_BUTTON_B;
    
    ASSERT_EQ(combo, buttons & combo);
}

TEST(ButtonMapping_Combo_ThreeButtons) {
    u16 buttons = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START;
    u16 combo = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START;
    
    ASSERT_EQ(combo, buttons & combo);
}

TEST(ButtonMapping_Combo_PartialMatch) {
    u16 buttons = PAD_BUTTON_A | PAD_BUTTON_B;
    u16 combo = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START;
    
    // Not all combo buttons are pressed
    ASSERT_NE(combo, buttons & combo);
}

TEST(ButtonMapping_Combo_ResetCombo) {
    // Common reset combination: A+B+X+Y+Start
    u16 buttons = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | 
                  PAD_BUTTON_Y | PAD_BUTTON_START;
    u16 reset_combo = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | 
                      PAD_BUTTON_Y | PAD_BUTTON_START;
    
    ASSERT_EQ(reset_combo, buttons & reset_combo);
}

//-----------------------------------------------------------------------------
// Direction Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_Direction_Opposite) {
    // Can't press up and down simultaneously (hardware check)
    u16 up_down = PAD_BUTTON_UP | PAD_BUTTON_DOWN;
    
    // Both bits can be set, but game logic should handle this
    ASSERT_TRUE(up_down & PAD_BUTTON_UP);
    ASSERT_TRUE(up_down & PAD_BUTTON_DOWN);
}

TEST(ButtonMapping_Direction_LeftRight) {
    u16 left_right = PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT;
    ASSERT_TRUE(left_right & PAD_BUTTON_LEFT);
    ASSERT_TRUE(left_right & PAD_BUTTON_RIGHT);
}

TEST(ButtonMapping_Direction_Diagonal) {
    u16 diagonal = PAD_BUTTON_UP | PAD_BUTTON_LEFT;
    ASSERT_TRUE(diagonal & PAD_BUTTON_UP);
    ASSERT_TRUE(diagonal & PAD_BUTTON_LEFT);
}

//-----------------------------------------------------------------------------
// Button Priority Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_Priority_Face) {
    // Face buttons (A, B, X, Y)
    u16 face = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y;
    u16 face_mask = PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y;
    
    ASSERT_EQ(face, face & face_mask);
}

TEST(ButtonMapping_Priority_Triggers) {
    // Trigger buttons
    u16 triggers = PAD_TRIGGER_L | PAD_TRIGGER_R | PAD_TRIGGER_Z;
    u16 trigger_mask = PAD_TRIGGER_L | PAD_TRIGGER_R | PAD_TRIGGER_Z;
    
    ASSERT_EQ(triggers, triggers & trigger_mask);
}

//-----------------------------------------------------------------------------
// Button Value Tests
//-----------------------------------------------------------------------------

TEST(ButtonMapping_Values_Unique) {
    // Each button should have a unique bit value
    ASSERT_NE(PAD_BUTTON_A, PAD_BUTTON_B);
    ASSERT_NE(PAD_BUTTON_A, PAD_BUTTON_X);
    ASSERT_NE(PAD_BUTTON_B, PAD_BUTTON_Y);
}

TEST(ButtonMapping_Values_PowerOfTwo) {
    // Button values should be powers of 2
    // Check that each button has only one bit set
    auto is_power_of_two = [](u16 n) {
        return n && !(n & (n - 1));
    };
    
    ASSERT_TRUE(is_power_of_two(PAD_BUTTON_A));
    ASSERT_TRUE(is_power_of_two(PAD_BUTTON_B));
    ASSERT_TRUE(is_power_of_two(PAD_BUTTON_X));
    ASSERT_TRUE(is_power_of_two(PAD_BUTTON_Y));
}

TEST(ButtonMapping_Values_NonZero) {
    // No button should have a value of 0
    ASSERT_NE(0, PAD_BUTTON_A);
    ASSERT_NE(0, PAD_BUTTON_B);
    ASSERT_NE(0, PAD_BUTTON_START);
    ASSERT_NE(0, PAD_TRIGGER_Z);
}
