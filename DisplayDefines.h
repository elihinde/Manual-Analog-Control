//-------------------------------------------------------------------------------------------------
/*
 *  DisplayDefines.h
 *  
 *  Should be included in all display project header files.
 *  Mostly consists of register addresses and bitmasks useful in display project files.
 *  Contains some configuration values for the display.
 *    - Note there is no bounds-checking.
 */
//-------------------------------------------------------------------------------------------------

#ifndef _DISPLAYDEFINES_H_
#define _DISPLAYDEFINES_H_

// Additional defines for display board.

/* --- MCP23017 --- */

// I2C Address


// Pins
    // Register A
#define C_LED_bm        0b00000001
#define F_LED_bm        0b00000010
#define RH_LED_bm       0b00000100
#define TEMP_LED_bm     0b00001000
#define HUMID_LED_bm    0b00010000
#define DIFF_LED_bm     0b00100000
#define HEATER_LED_bm   0b01000000
#define ALARM_LED_bm    0b10000000

    // Register B
#define ENC_BUTTON_bm   0b00001000
#define ENC_A_bm        0b00010000
#define ENC_B_bm        0b00100000

// Register assignments (not exhaustive)
    // Assuming pointer toggling is on and registers are together (BANK = 0).
#define IODIRA          0x00 // Input (1) or output (0)
#define IODIRB          0x01
#define GPIOA           0x12 // Set output state or read input
#define GPIOB           0x13

#define IOCON           0x0A // Important configurations for chip
#define SEQOP_bm        0b00100000 // Auto address incrementer, set to 1

// Configurations
#define ENC_TURN_TIMEOUT        200U // EDITABLE


/* --- LP5868 --- */

// I2C Address
    // This device is unique in that it has a 5-bit address.
    // Bit 2 & 1 (Bit 0 is R/W) serve as bits 9-8 of the register address.
    // The next byte sent contains the rest of the register address.
#define SEG_DRIVER_addr 0b1000000U 

// Control registers
    // Refer to the following for register descriptions.
    // https://www.ti.com/lit/ug/snvu786/snvu786.pdf

#define CHIP_EN         0x000U // Standby bit, set 1 for operation.
#define DEV_INITAL      0x001U // Important configurations
    // Max scanlines (we need 7)
    // Data refresh mode
    // PWM frequency setting
#define DEV_CONFIG1     0x002U // Other configs
    // Dimming scale
    // PWM phase shift
    // Current sink turn-on delay
#define DEV_CONFIG2     0x003U // Other configs
    // Low brightness compensation
    // LOD & LSD removal (opens & shorts)
#define DEV_CONFIG3     0x004U // Deghosting & current
    // Down deghost
    // Up deghost
    // Maximum current
    // Up deghost enable
#define FAULT_STATE     0x064U // LOD & LSD flag bits
#define RESET           0x0A9U // Write 0xFF to reset the device

// Configurations
#define RED_MAX_CURR_PERCENT        100.0f // EDITABLE
#define GRN_MAX_CURR_PERCENT        60.0f // EDITABLE
    // Maximum current is set to 50mA, which is the maximum for the chip
        // Max (continuous) rated current for green is 30mA
        // Max (continuous) rated current for red is 25mA
        // This chip does not supply continuous current, each segment is on at 1/7 duty cycle
        // There is a drop-in replacement chip with a larger max current, if desired.

// Dot ON/OFF registers
    // Outputs default to on
    // The lower registers [7..0] are [G2..A]
    // The upper registers [15..8] are [NC, DP..H]
    // The unused register [17..16] is all NC
#define B_DIG_1_LOWER_SEG       0x043U
#define B_DIG_1_UPPER_SEG       0x044U
#define B_DIG_1_UNUSED          0x045U

#define B_DIG_2_LOWER_SEG       0x046U
#define B_DIG_2_UPPER_SEG       0x047U
#define B_DIG_2_UNUSED          0x048U

#define B_DIG_3_LOWER_SEG       0x049U
#define B_DIG_3_UPPER_SEG       0x04AU
#define B_DIG_3_UNUSED          0x04BU

#define T_DIG_4_LOWER_SEG       0x04CU
#define T_DIG_4_UPPER_SEG       0x04DU
#define T_DIG_4_UNUSED          0x04EU

#define T_DIG_3_LOWER_SEG       0x04FU
#define T_DIG_3_UPPER_SEG       0x050U
#define T_DIG_3_UNUSED          0x051U

#define T_DIG_2_LOWER_SEG       0x052U
#define T_DIG_2_UPPER_SEG       0x053U
#define T_DIG_2_UNUSED          0x054U

#define T_DIG_1_LOWER_SEG       0x055U
#define T_DIG_1_UPPER_SEG       0x056U  
#define T_DIG_1_UNUSED          0x057U

// Dot PWM registers
    // Default to 0% duty cycle
#define B_DIG_1_PWM_CS0         0x200U
#define B_DIG_1_PWM_CS1         0x201U
#define B_DIG_1_PWM_CS2         0x202U
#define B_DIG_1_PWM_CS3         0x203U
#define B_DIG_1_PWM_CS4         0x204U
#define B_DIG_1_PWM_CS5         0x205U
#define B_DIG_1_PWM_CS6         0x206U
#define B_DIG_1_PWM_CS7         0x207U
#define B_DIG_1_PWM_CS8         0x208U
#define B_DIG_1_PWM_CS9         0x209U
#define B_DIG_1_PWM_CS10        0x20AU
#define B_DIG_1_PWM_CS11        0x20BU
#define B_DIG_1_PWM_CS12        0x20CU
#define B_DIG_1_PWM_CS13        0x20DU
#define B_DIG_1_PWM_CS14        0x20EU

#define B_DIG_2_PWM_CS0         0x212U
#define B_DIG_2_PWM_CS1         0x213U
#define B_DIG_2_PWM_CS2         0x214U
#define B_DIG_2_PWM_CS3         0x215U
#define B_DIG_2_PWM_CS4         0x216U
#define B_DIG_2_PWM_CS5         0x217U
#define B_DIG_2_PWM_CS6         0x218U
#define B_DIG_2_PWM_CS7         0x219U
#define B_DIG_2_PWM_CS8         0x21aU
#define B_DIG_2_PWM_CS9         0x21BU
#define B_DIG_2_PWM_CS10        0x21CU
#define B_DIG_2_PWM_CS11        0x21DU
#define B_DIG_2_PWM_CS12        0x21EU
#define B_DIG_2_PWM_CS13        0x21FU
#define B_DIG_2_PWM_CS14        0x220U

#define B_DIG_3_PWM_CS0         0x224U
#define B_DIG_3_PWM_CS1         0x225U
#define B_DIG_3_PWM_CS2         0x226U
#define B_DIG_3_PWM_CS3         0x227U
#define B_DIG_3_PWM_CS4         0x228U
#define B_DIG_3_PWM_CS5         0x229U
#define B_DIG_3_PWM_CS6         0x22AU
#define B_DIG_3_PWM_CS7         0x22BU
#define B_DIG_3_PWM_CS8         0x22CU
#define B_DIG_3_PWM_CS9         0x22DU
#define B_DIG_3_PWM_CS10        0x22EU
#define B_DIG_3_PWM_CS11        0x22FU
#define B_DIG_3_PWM_CS12        0x230U
#define B_DIG_3_PWM_CS13        0x231U
#define B_DIG_3_PWM_CS14        0x232U

#define T_DIG_4_PWM_CS0         0x236U
#define T_DIG_4_PWM_CS1         0x237U
#define T_DIG_4_PWM_CS2         0x238U
#define T_DIG_4_PWM_CS3         0x239U
#define T_DIG_4_PWM_CS4         0x23aU
#define T_DIG_4_PWM_CS5         0x23BU
#define T_DIG_4_PWM_CS6         0x23CU
#define T_DIG_4_PWM_CS7         0x23DU
#define T_DIG_4_PWM_CS8         0x23EU
#define T_DIG_4_PWM_CS9         0x23FU
#define T_DIG_4_PWM_CS10        0x240U
#define T_DIG_4_PWM_CS11        0x241U
#define T_DIG_4_PWM_CS12        0x242U
#define T_DIG_4_PWM_CS13        0x243U
#define T_DIG_4_PWM_CS14        0x244U

#define T_DIG_3_PWM_CS0         0x248U
#define T_DIG_3_PWM_CS1         0x249U
#define T_DIG_3_PWM_CS2         0x24AU
#define T_DIG_3_PWM_CS3         0x24BU
#define T_DIG_3_PWM_CS4         0x24CU
#define T_DIG_3_PWM_CS5         0x24DU
#define T_DIG_3_PWM_CS6         0x24EU
#define T_DIG_3_PWM_CS7         0x24FU
#define T_DIG_3_PWM_CS8         0x250U
#define T_DIG_3_PWM_CS9         0x251U
#define T_DIG_3_PWM_CS10        0x252U
#define T_DIG_3_PWM_CS11        0x253U
#define T_DIG_3_PWM_CS12        0x254U
#define T_DIG_3_PWM_CS13        0x255U
#define T_DIG_3_PWM_CS14        0x256U

#define T_DIG_2_PWM_CS0         0x25AU
#define T_DIG_2_PWM_CS1         0x25BU
#define T_DIG_2_PWM_CS2         0x25CU
#define T_DIG_2_PWM_CS3         0x25DU
#define T_DIG_2_PWM_CS4         0x25EU
#define T_DIG_2_PWM_CS5         0x25FU
#define T_DIG_2_PWM_CS6         0x260U
#define T_DIG_2_PWM_CS7         0x261U
#define T_DIG_2_PWM_CS8         0x262U
#define T_DIG_2_PWM_CS9         0x263U
#define T_DIG_2_PWM_CS10        0x264U
#define T_DIG_2_PWM_CS11        0x265U
#define T_DIG_2_PWM_CS12        0x266U
#define T_DIG_2_PWM_CS13        0x267U
#define T_DIG_2_PWM_CS14        0x268U

#define T_DIG_1_PWM_CS0         0x26CU
#define T_DIG_1_PWM_CS1         0x26DU
#define T_DIG_1_PWM_CS2         0x26EU
#define T_DIG_1_PWM_CS3         0x26FU
#define T_DIG_1_PWM_CS4         0x270U
#define T_DIG_1_PWM_CS5         0x271U
#define T_DIG_1_PWM_CS6         0x272U
#define T_DIG_1_PWM_CS7         0x273U
#define T_DIG_1_PWM_CS8         0x274U
#define T_DIG_1_PWM_CS9         0x275U
#define T_DIG_1_PWM_CS10        0x276U
#define T_DIG_1_PWM_CS11        0x277U
#define T_DIG_1_PWM_CS12        0x278U
#define T_DIG_1_PWM_CS13        0x279U
#define T_DIG_1_PWM_CS14        0x27AU

// Dot current registers
    // Default to 50% max current
#define B_DIG_1_CURR_CS0        0x100U
#define B_DIG_1_CURR_CS1        0x101U
#define B_DIG_1_CURR_CS2        0x102U
#define B_DIG_1_CURR_CS3        0x103U
#define B_DIG_1_CURR_CS4        0x104U
#define B_DIG_1_CURR_CS5        0x105U
#define B_DIG_1_CURR_CS6        0x106U
#define B_DIG_1_CURR_CS7        0x107U
#define B_DIG_1_CURR_CS8        0x108U
#define B_DIG_1_CURR_CS9        0x109U
#define B_DIG_1_CURR_CS10       0x10AU
#define B_DIG_1_CURR_CS11       0x10BU
#define B_DIG_1_CURR_CS12       0x10CU
#define B_DIG_1_CURR_CS13       0x10DU
#define B_DIG_1_CURR_CS14       0x10EU

#define B_DIG_2_CURR_CS0        0x112U
#define B_DIG_2_CURR_CS1        0x113U
#define B_DIG_2_CURR_CS2        0x114U
#define B_DIG_2_CURR_CS3        0x115U
#define B_DIG_2_CURR_CS4        0x116U
#define B_DIG_2_CURR_CS5        0x117U
#define B_DIG_2_CURR_CS6        0x118U
#define B_DIG_2_CURR_CS7        0x119U
#define B_DIG_2_CURR_CS8        0x11aU
#define B_DIG_2_CURR_CS9        0x11BU
#define B_DIG_2_CURR_CS10       0x11CU
#define B_DIG_2_CURR_CS11       0x11DU
#define B_DIG_2_CURR_CS12       0x11EU
#define B_DIG_2_CURR_CS13       0x11FU
#define B_DIG_2_CURR_CS14       0x120U

#define B_DIG_3_CURR_CS0        0x124U
#define B_DIG_3_CURR_CS1        0x125U
#define B_DIG_3_CURR_CS2        0x126U
#define B_DIG_3_CURR_CS3        0x127U
#define B_DIG_3_CURR_CS4        0x128U
#define B_DIG_3_CURR_CS5        0x129U
#define B_DIG_3_CURR_CS6        0x12AU
#define B_DIG_3_CURR_CS7        0x12BU
#define B_DIG_3_CURR_CS8        0x12CU
#define B_DIG_3_CURR_CS9        0x12DU
#define B_DIG_3_CURR_CS10       0x12EU
#define B_DIG_3_CURR_CS11       0x12FU
#define B_DIG_3_CURR_CS12       0x130U
#define B_DIG_3_CURR_CS13       0x131U
#define B_DIG_3_CURR_CS14       0x132U

#define T_DIG_4_CURR_CS0        0x136U
#define T_DIG_4_CURR_CS1        0x137U
#define T_DIG_4_CURR_CS2        0x138U
#define T_DIG_4_CURR_CS3        0x139U
#define T_DIG_4_CURR_CS4        0x13aU
#define T_DIG_4_CURR_CS5        0x13BU
#define T_DIG_4_CURR_CS6        0x13CU
#define T_DIG_4_CURR_CS7        0x13DU
#define T_DIG_4_CURR_CS8        0x13EU
#define T_DIG_4_CURR_CS9        0x13FU
#define T_DIG_4_CURR_CS10       0x140U
#define T_DIG_4_CURR_CS11       0x141U
#define T_DIG_4_CURR_CS12       0x142U
#define T_DIG_4_CURR_CS13       0x143U
#define T_DIG_4_CURR_CS14       0x144U

#define T_DIG_3_CURR_CS0        0x148U
#define T_DIG_3_CURR_CS1        0x149U
#define T_DIG_3_CURR_CS2        0x14AU
#define T_DIG_3_CURR_CS3        0x14BU
#define T_DIG_3_CURR_CS4        0x14CU
#define T_DIG_3_CURR_CS5        0x14DU
#define T_DIG_3_CURR_CS6        0x14EU
#define T_DIG_3_CURR_CS7        0x14FU
#define T_DIG_3_CURR_CS8        0x150U
#define T_DIG_3_CURR_CS9        0x151U
#define T_DIG_3_CURR_CS10       0x152U
#define T_DIG_3_CURR_CS11       0x153U
#define T_DIG_3_CURR_CS12       0x154U
#define T_DIG_3_CURR_CS13       0x155U
#define T_DIG_3_CURR_CS14       0x156U

#define T_DIG_2_CURR_CS0        0x15AU
#define T_DIG_2_CURR_CS1        0x15BU
#define T_DIG_2_CURR_CS2        0x15CU
#define T_DIG_2_CURR_CS3        0x15DU
#define T_DIG_2_CURR_CS4        0x15EU
#define T_DIG_2_CURR_CS5        0x15FU
#define T_DIG_2_CURR_CS6        0x160U
#define T_DIG_2_CURR_CS7        0x161U
#define T_DIG_2_CURR_CS8        0x162U
#define T_DIG_2_CURR_CS9        0x163U
#define T_DIG_2_CURR_CS10       0x164U
#define T_DIG_2_CURR_CS11       0x165U
#define T_DIG_2_CURR_CS12       0x166U
#define T_DIG_2_CURR_CS13       0x167U
#define T_DIG_2_CURR_CS14       0x168U

#define T_DIG_1_CURR_CS0        0x16CU
#define T_DIG_1_CURR_CS1        0x16DU
#define T_DIG_1_CURR_CS2        0x16EU
#define T_DIG_1_CURR_CS3        0x16FU
#define T_DIG_1_CURR_CS4        0x170U
#define T_DIG_1_CURR_CS5        0x171U
#define T_DIG_1_CURR_CS6        0x172U
#define T_DIG_1_CURR_CS7        0x173U
#define T_DIG_1_CURR_CS8        0x174U
#define T_DIG_1_CURR_CS9        0x175U
#define T_DIG_1_CURR_CS10       0x176U
#define T_DIG_1_CURR_CS11       0x177U
#define T_DIG_1_CURR_CS12       0x178U
#define T_DIG_1_CURR_CS13       0x179U
#define T_DIG_1_CURR_CS14       0x17AU

#endif //_DISPLAYDEFINES_H_