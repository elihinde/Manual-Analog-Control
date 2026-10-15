#pragma once

#include "Defines.h"
#include <Wire.h>
#include "DisplayDefines.h"

    // List of avalible characters to display. 
    // Serves as the index for their respective character in "charCodes".
    enum Character {
        CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4,
        CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9,

        CHAR_A, CHAR_B, CHAR_C, CHAR_D, CHAR_E,
        CHAR_F, CHAR_G, CHAR_H, CHAR_I, CHAR_J,
        CHAR_K, CHAR_L, CHAR_M, CHAR_N, CHAR_O,
        CHAR_P, CHAR_Q, CHAR_R, CHAR_S, CHAR_T,
        CHAR_U, CHAR_V, CHAR_W, CHAR_X, CHAR_Y,
        CHAR_Z,

        CHAR_PLUS, CHAR_MINUS, CHAR_BLANK,
        CHAR_SLASH, CHAR_BSLASH
    };

class AnalogControl {
public:
    void SetupCore();
    
    void SetupDisplay();

    void WriteDriver(uint16_t reg, uint8_t data);

    void WriteCharacter(Character chr, uint16_t lowerSegAddr, bool dp);

    void Loop();

private: 
    //binary values of numbers / characters
    const uint16_t charCodes[41] = {
        0b000110000111111, // '0'
        0b000010000000110, // '1'
        0b000000011011011, // '2'
        0b000000011001111, // '3'
        0b000000011100110, // '4'
        0b000000011101101, // '5'
        0b000000011111101, // '6'
        0b000110000000001, // '7'
        0b000000011111111, // '8'
        0b000000011100111, // '9'

        0b000000011110111, // 'A'
        0b001001010001111, // 'B'
        0b000000000111001, // 'C'
        0b001001000001111, // 'D'
        0b000000001111001, // 'E'
        0b000000001110001, // 'F'
        0b000000010111101, // 'G'
        0b000000011110110, // 'H'
        0b001001000001001, // 'I'
        0b000000000011110, // 'J'
        0b010010001110000, // 'K'
        0b000000000111000, // 'L'
        0b000010100110110, // 'M'
        0b010000100110110, // 'N'
        0b000000000111111, // 'O'
        0b000000011110011, // 'P'
        0b010000000111111, // 'Q'
        0b010000011110011, // 'R'
        0b000000011101101, // 'S'
        0b001001000000001, // 'T'
        0b000000000111110, // 'U'
        0b000110000110000, // 'V'
        0b010100000110110, // 'W'
        0b010110100000000, // 'X'
        0b001010100000000, // 'Y'
        0b000110000001001, // 'Z'

        0b001001011000000, // '+'
        0b000000011000000, // '-'
        0b000000000000000, // ' '
        0b000110000000000, // '/'
        0b010000100000000  // '\'
};    


};