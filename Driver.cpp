<<<<<<< HEAD

#include "Defines.h"
#include <Wire.h>
#include "DisplayDefines.h"
#include "Driver.h"
#include <EEPROM.h>



volatile int counter;
volatile int counterF;
volatile int counterC;
volatile int counterP;
volatile int counterV;
volatile int mode;
volatile int counterF_New;
unsigned long last_run = 0;
double counterNew;
int tracker;
bool button;
unsigned int dacValue;

void ISR_ENCA(){
    switch (tracker){
    case 1: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterF < 350){
                    counterF++;
                    EEPROM.write(0, counterF);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterF > 0){
                counterF--;
                EEPROM.write(0, counterF);
            }  
        }
        last_run = millis();
    } 
    break;
    case 2: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterC < 180){
                    counterC++;
                    EEPROM.write(4, counterC);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterC > 0){
                counterC--;
                EEPROM.write(4, counterC);
            }  
        }
        last_run = millis();
    } 

    break;
    case 3: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterP < 100){
                    counterP++;
                    EEPROM.write(8, counterP);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterP > 0){
                counterP--;
                EEPROM.write(8, counterP);
            }  
        }
        last_run = millis();
    } 

    break;
    case 4: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterV < 10){
                    counterV++;
                    EEPROM.write(12, counterV);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterV > 0){
                counterV--;
                EEPROM.write(12, counterV);
            }  
        }
        last_run = millis();
    } 

    break;
    }
}

void ISR_monitorButton(){
    button = 0;
}

void AnalogControl::SetupCore(){

    Wire.begin();
    Wire.setClock(400000);
    delay(10);

    //initialize serial monitor
    dbgSerial.begin(baudRate);

    //Button
    pinMode(mode_Button, INPUT);

    //LED pins
    pinMode(degF_LED, OUTPUT);
    pinMode(degC_LED, OUTPUT);
    pinMode(percent_LED, OUTPUT);
    pinMode(volt_LED, OUTPUT);

    //set up encoder pins
    pinMode(encA, INPUT_PULLUP);
    pinMode(encB, INPUT_PULLUP);

    counterF = EEPROM.read(0) + EEPROM.read(1);
    counterC = EEPROM.read(4);
    counterP = EEPROM.read(8);
    counterV = EEPROM.read(12);

    button = 1;
    tracker = EEPROM.read(E_MODE);
    dbgSerial.println(EEPROM.read(E_MODE));
    

    attachInterrupt(digitalPinToInterrupt(encA), ISR_ENCA, LOW);
    attachInterrupt(digitalPinToInterrupt(mode_Button), ISR_monitorButton, FALLING);

    
}

void AnalogControl::SetupDisplay()
{

    /* --- Matrix Driver --- */
    WriteDriver(RESET, 0xFF); // The chip stays powered even if the microcontroller resets (or reprograms)
        // This ensures no previous settings carry over.
    
    delay(10);

    // Enable display (should be all on)
    WriteDriver(CHIP_EN, 0x01);
    WriteDriver(DEV_INITAL, 0b00111000); // Scanlines & mode setting
    WriteDriver(DEV_CONFIG1, 0x0C); // Timings
    WriteDriver(DEV_CONFIG2, 0x03); // Enable LOD and LSD removal
    WriteDriver(DEV_CONFIG3, 0b01010111); // Max current setting
        // Set to maximum of 50mA per segment.
        // Default is 15mA per segment.
        
    // Turn off unused segments (segments default on)
    WriteDriver(B_DIG_3_UNUSED, 0x00);
    WriteDriver(B_DIG_2_UNUSED, 0x00);
    WriteDriver(B_DIG_1_UNUSED, 0x00);
    WriteDriver(T_DIG_4_UNUSED, 0x00);
    WriteDriver(T_DIG_3_UNUSED, 0x00);
    WriteDriver(T_DIG_2_UNUSED, 0x00);
    WriteDriver(T_DIG_3_UNUSED, 0x00);

    // Current limiting registers
    WriteDriver(0x009, 0xFF); // Current limit on line R to 100%
    WriteDriver(0x00A, 0xFF); // Current limit on line G to 100%
    WriteDriver(0x00B, 0xFF); // Current limit on line B to 100%
        // Each individual dot also has a current limit at 50%

    // Set dash as the default value to limit startup current.
    WriteCharacter(CHAR_MINUS, T_DIG_1_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, T_DIG_2_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, T_DIG_3_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, T_DIG_4_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, B_DIG_1_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, B_DIG_2_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, B_DIG_3_LOWER_SEG, 0x00);

    // Turn on all PWMs for all LEDs
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_1_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_2_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_3_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_4_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_3_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_2_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_1_PWM_CS0 + i, 0xFF);
    }

    // Set the current limit for all the dots
    uint8_t redCurr = 255 * (RED_MAX_CURR_PERCENT / 100.0f);
    uint8_t grnCurr = 255 * (GRN_MAX_CURR_PERCENT / 100.0f);
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_1_CURR_CS0 + i, grnCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_2_CURR_CS0 + i, grnCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_3_CURR_CS0 + i, grnCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_4_CURR_CS0 + i, redCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_3_CURR_CS0 + i, redCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_2_CURR_CS0 + i, redCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_1_CURR_CS0 + i, redCurr);
    }

    delay(500);
}

void AnalogControl::WriteDriver(uint16_t reg, uint8_t data) {
    uint8_t topTwoBits = ((reg & 0x300) >> 8);
    Wire.beginTransmission(uint8_t(SEG_DRIVER_addr) | topTwoBits);
    Wire.write(uint8_t(reg));
    Wire.write(data);

    uint8_t temp = Wire.endTransmission();

    return;
    
}

void AnalogControl::WriteCharacter(Character chr, uint16_t lowerSegAddr, bool dp) {
    
    uint16_t tempCharCode = charCodes[chr];

    if(dp) { // Add decimal point
        tempCharCode = tempCharCode | 0x4000; // bitmask
    }

    Wire.beginTransmission(uint8_t(SEG_DRIVER_addr));
    Wire.write(lowerSegAddr);
    Wire.write(uint8_t(tempCharCode));
    Wire.write(uint8_t(tempCharCode >> 8));
    Wire.endTransmission();

    return;
}

void AnalogControl::Loop(){

    //bool button goes to 0 when toggle mode button is pressed
    //variable "tracker" controls switch statement
    if(button == 0){
        tracker += 1;
        if(tracker > 4){
            tracker = 1;
        }

        switch(tracker){
            case 1: counterF = EEPROM.read(0) + EEPROM.read(1); break;
            case 2: counterC = EEPROM.read(4); break;
            case 3: counterP = EEPROM.read(8); break;
            case 4: counterV = EEPROM.read(12); break;
        }

        button = 1;
        delay(1);
    }

    //Turn on mode LED
    switch (tracker){
        case 1: 
            digitalWrite(degF_LED, HIGH);
            digitalWrite(degC_LED, LOW);
            digitalWrite(percent_LED, LOW);
            digitalWrite(volt_LED, LOW);
            EEPROM.update(E_MODE, 1);
            break;
        case 2: 
            digitalWrite(degF_LED, LOW);
            digitalWrite(degC_LED, HIGH);
            digitalWrite(percent_LED, LOW);
            digitalWrite(volt_LED, LOW);
            EEPROM.update(E_MODE, 2);
            break;
        case 3: 
            digitalWrite(degF_LED, LOW);
            digitalWrite(degC_LED, LOW);
            digitalWrite(percent_LED, HIGH);
            digitalWrite(volt_LED, LOW);
            EEPROM.update(E_MODE, 3);
            break;
        case 4: 
            digitalWrite(degF_LED, LOW);
            digitalWrite(degC_LED, LOW);
            digitalWrite(percent_LED, LOW);
            digitalWrite(volt_LED, HIGH);
            EEPROM.update(E_MODE, 4);
            break;
    }

    //dacValue(variable) = counter(variable) * slope
    //Find bytes
    //Write to DAC
    //Seperate counter(variable) into single digits
    //Write digits to segments
    
    if(tracker == 1){

        //multiply encoder value by slope for DAC I2C 
        dacValue = counterF * 4.095;
        
        // limit to 12-bit
        if(dacValue > 4095)
        {
            dacValue = 4095;
        }

        // First byte:
        // 0000 D11 D10 D9 D8
        byte firstByte = dacValue >> 8;

        // Second byte:
        // D7 D6 D5 D4 D3 D2 D1 D0
        byte secondByte = dacValue & 0xFF;

        //write to DAC
        Wire.beginTransmission(0x60);
        Wire.write(firstByte);
        Wire.write(secondByte);
        Wire.endTransmission();


        //Seperate counter(variable) into single digits
        int a = counterF / 1000;
        int b = (counterF / 100) % 10;
        int c = (counterF / 10) % 10;
        int d = counterF % 10;

        //Write digits to segments
        WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
        WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
        WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
        WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
        
        if(counterF > 255){
            EEPROM.update(0, 255);
            EEPROM.update(1, counterF - 255); 
        }
        else{
            EEPROM.update(1, 0);
        }
    
    }
        else if(tracker == 2){

            //multiply encoder value by slope for DAC I2C 
            float tempF = (counterC * 9.0 / 5.0) + 32.0;
            dacValue = tempF * 4.095;
            
            // limit to 12-bit
            if(dacValue > 4095)
            {
                dacValue = 4095;
            }

            // First byte:
            // 0000 D11 D10 D9 D8
            byte firstByte = dacValue >> 8;

            // Second byte:
            // D7 D6 D5 D4 D3 D2 D1 D0
            byte secondByte = dacValue & 0xFF;

            //write to DAC
            Wire.beginTransmission(0x60);
            Wire.write(firstByte);
            Wire.write(secondByte);
            Wire.endTransmission();


            //Seperate counter(variable) into single digits
            int a = counterC / 1000;
            int b = (counterC / 100) % 10;
            int c = (counterC / 10) % 10;
            int d = counterC % 10;

            //Write digits to segments
            WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
            WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
            WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
            WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
        }
            else if(tracker == 3){

                //multiply encoder value by slope for DAC I2C 
                dacValue = counterP * 40.95;
                
                // limit to 12-bit
                if(dacValue > 4095)
                {
                    dacValue = 4095;
                }

                // First byte:
                // 0000 D11 D10 D9 D8
                byte firstByte = dacValue >> 8;

                // Second byte:
                // D7 D6 D5 D4 D3 D2 D1 D0
                byte secondByte = dacValue & 0xFF;

                //write to DAC
                Wire.beginTransmission(0x60);
                Wire.write(firstByte);
                Wire.write(secondByte);
                Wire.endTransmission();


                //Seperate counter(variable) into single digits
                int a = counterP / 1000;
                int b = (counterP / 100) % 10;
                int c = (counterP / 10) % 10;
                int d = counterP % 10;

                //Write digits to segments
                WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
                WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
            }
                else if(tracker == 4){

                //multiply encoder value by slope for DAC I2C 
                dacValue = counterV * 409.5;
                
                // limit to 12-bit
                if(dacValue > 4095)
                {
                    dacValue = 4095;
                }

                // First byte:
                // 0000 D11 D10 D9 D8
                byte firstByte = dacValue >> 8;

                // Second byte:
                // D7 D6 D5 D4 D3 D2 D1 D0
                byte secondByte = dacValue & 0xFF;

                //write to DAC
                Wire.beginTransmission(0x60);
                Wire.write(firstByte);
                Wire.write(secondByte);
                Wire.endTransmission();


                //Seperate counter(variable) into single digits
                int a = counterV / 1000;
                int b = (counterV / 100) % 10;
                int c = (counterV / 10) % 10;
                int d = counterV % 10;

                //Write digits to segments
                WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
                WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
            }







}
=======

#include "Defines.h"
#include <Wire.h>
#include "DisplayDefines.h"
#include "Driver.h"
#include <EEPROM.h>



volatile int counter;
volatile int counterF;
volatile int counterC;
volatile int counterP;
volatile int counterV;
volatile int mode;
volatile int counterF_New;
volatile int dipSwitch;
unsigned long last_run = 0;
double counterNew;
int tracker;
unsigned int dacValue;

void ISR_ENCA(){
    switch (tracker){
    case 1: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterF < 350){
                    counterF++;
                    EEPROM.write(0, counterF);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterF > 0){
                counterF--;
                EEPROM.write(0, counterF);
            }  
        }
        last_run = millis();
    } 
    break;
    case 2: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterC < 180){
                    counterC++;
                    EEPROM.write(4, counterC);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterC > 0){
                counterC--;
                EEPROM.write(4, counterC);
            }  
        }
        last_run = millis();
    } 

    break;
    case 3: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterP < 100){
                    counterP++;
                    EEPROM.write(8, counterP);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterP > 0){
                counterP--;
                EEPROM.write(8, counterP);
            }  
        }
        last_run = millis();
    } 

    break;
    case 4: 
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterV < 10){
                    counterV++;
                    EEPROM.write(12, counterV);
                }
            }
        if(digitalRead(encB) == 0){
            if(counterV > 0){
                counterV--;
                EEPROM.write(12, counterV);
            }  
        }
        last_run = millis();
    } 

    break;
    }
}

void AnalogControl::SetupCore(){

    Wire.begin();
    Wire.setClock(400000);
    delay(10);

    //initialize serial monitor
    dbgSerial.begin(baudRate);

    //Dip Switch Inputs
    pinMode(dipStatusOne, INPUT);
    pinMode(dipStatusTwo, INPUT);
    pinMode(dipStatusFour, INPUT);
    pinMode(dipStatusEight, INPUT);

    //LED pins
    pinMode(degF_LED, OUTPUT);
    pinMode(degC_LED, OUTPUT);
    pinMode(percent_LED, OUTPUT);
    pinMode(volt_LED, OUTPUT);

    //set up encoder pins
    pinMode(encA, INPUT_PULLUP);
    pinMode(encB, INPUT_PULLUP);

    counterF = EEPROM.read(0) + EEPROM.read(1);
    counterC = EEPROM.read(4);
    counterP = EEPROM.read(8);
    counterV = EEPROM.read(12);

    //button = 1;
    tracker = EEPROM.read(E_MODE);
    dbgSerial.println(EEPROM.read(E_MODE));
    

    attachInterrupt(digitalPinToInterrupt(encA), ISR_ENCA, LOW);
}

void AnalogControl::SetupDisplay()
{

    /* --- Matrix Driver --- */
    WriteDriver(RESET, 0xFF); // The chip stays powered even if the microcontroller resets (or reprograms)
        // This ensures no previous settings carry over.
    
    delay(10);

    // Enable display (should be all on)
    WriteDriver(CHIP_EN, 0x01);
    WriteDriver(DEV_INITAL, 0b00111000); // Scanlines & mode setting
    WriteDriver(DEV_CONFIG1, 0x0C); // Timings
    WriteDriver(DEV_CONFIG2, 0x03); // Enable LOD and LSD removal
    WriteDriver(DEV_CONFIG3, 0b01010111); // Max current setting
        // Set to maximum of 50mA per segment.
        // Default is 15mA per segment.
        
    // Turn off unused segments (segments default on)
    WriteDriver(B_DIG_3_UNUSED, 0x00);
    WriteDriver(B_DIG_2_UNUSED, 0x00);
    WriteDriver(B_DIG_1_UNUSED, 0x00);
    WriteDriver(T_DIG_4_UNUSED, 0x00);
    WriteDriver(T_DIG_3_UNUSED, 0x00);
    WriteDriver(T_DIG_2_UNUSED, 0x00);
    WriteDriver(T_DIG_3_UNUSED, 0x00);

    // Current limiting registers
    WriteDriver(0x009, 0xFF); // Current limit on line R to 100%
    WriteDriver(0x00A, 0xFF); // Current limit on line G to 100%
    WriteDriver(0x00B, 0xFF); // Current limit on line B to 100%
        // Each individual dot also has a current limit at 50%

    // Set dash as the default value to limit startup current.
    WriteCharacter(CHAR_MINUS, T_DIG_1_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, T_DIG_2_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, T_DIG_3_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, T_DIG_4_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, B_DIG_1_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, B_DIG_2_LOWER_SEG, 0x00);
    WriteCharacter(CHAR_MINUS, B_DIG_3_LOWER_SEG, 0x00);

    // Turn on all PWMs for all LEDs
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_1_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_2_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_3_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_4_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_3_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_2_PWM_CS0 + i, 0xFF);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_1_PWM_CS0 + i, 0xFF);
    }

    // Set the current limit for all the dots
    uint8_t redCurr = 255 * (RED_MAX_CURR_PERCENT / 100.0f);
    uint8_t grnCurr = 255 * (GRN_MAX_CURR_PERCENT / 100.0f);
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_1_CURR_CS0 + i, grnCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_2_CURR_CS0 + i, grnCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(B_DIG_3_CURR_CS0 + i, grnCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_4_CURR_CS0 + i, redCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_3_CURR_CS0 + i, redCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_2_CURR_CS0 + i, redCurr);
    }
    for(uint8_t i = 0; i < 15; i++) {
        WriteDriver(T_DIG_1_CURR_CS0 + i, redCurr);
    }

    delay(500);
}

void AnalogControl::WriteDriver(uint16_t reg, uint8_t data) {
    uint8_t topTwoBits = ((reg & 0x300) >> 8);
    Wire.beginTransmission(uint8_t(SEG_DRIVER_addr) | topTwoBits);
    Wire.write(uint8_t(reg));
    Wire.write(data);

    uint8_t temp = Wire.endTransmission();

    return;
    
}

void AnalogControl::WriteCharacter(Character chr, uint16_t lowerSegAddr, bool dp) {
    
    uint16_t tempCharCode = charCodes[chr];

    if(dp) { // Add decimal point
        tempCharCode = tempCharCode | 0x4000; // bitmask
    }

    Wire.beginTransmission(uint8_t(SEG_DRIVER_addr));
    Wire.write(lowerSegAddr);
    Wire.write(uint8_t(tempCharCode));
    Wire.write(uint8_t(tempCharCode >> 8));
    Wire.endTransmission();

    return;
}

void AnalogControl::Loop(){

//Read status of dip switch inputs
digitalRead(dipStatusOne);
digitalRead(dipStatusTwo);
digitalRead(dipStatusFour);
digitalRead(dipStatusEight);

//Dip swtich inputs manipulate the variable of tracker
if(dipStatusEight == 0 && dipStatusFour == 0 && dipStatusTwo == 0 && dipStatusOne == 0){
    tracker = 0;
}
    else if(dipStatusEight == 0 && dipStatusFour == 0 && dipStatusTwo == 0 && dipStatusOne == 1){
        tracker = 1;
    } 
        else if(dipStatusEight == 0 && dipStatusFour == 0 && dipStatusTwo == 1 && dipStatusOne == 0){   
            tracker = 2;
        }
            else if(dipStatusEight == 0 && dipStatusFour == 0 && dipStatusTwo == 1 && dipStatusOne == 1){   
                tracker = 3;
            }
                else if(dipStatusEight == 0 && dipStatusFour == 1 && dipStatusTwo == 0 && dipStatusOne == 0){   
                    tracker = 4;
                }
                    else if(dipStatusEight == 0 && dipStatusFour == 1 && dipStatusTwo == 0 && dipStatusOne == 1){   
                        tracker = 5;
                    }
                        else{
                            tracker = tracker;
                        }

//Turn on mode LED
switch (tracker){
    case 0 : case 2: 
        digitalWrite(degF_LED, HIGH);
        digitalWrite(degC_LED, LOW);
        digitalWrite(percent_LED, LOW);
        digitalWrite(volt_LED, LOW);
        EEPROM.update(E_MODE, 1);
        break;
    case 1 : case 3:
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, HIGH);
        digitalWrite(percent_LED, LOW);
        digitalWrite(volt_LED, LOW);
        EEPROM.update(E_MODE, 2);
        break;
    case 4: 
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, LOW);
        digitalWrite(percent_LED, HIGH);
        digitalWrite(volt_LED, LOW);
        EEPROM.update(E_MODE, 3);
        break;
    case 5: 
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, LOW);
        digitalWrite(percent_LED, LOW);
        digitalWrite(volt_LED, HIGH);
        EEPROM.update(E_MODE, 4);
        break;
}

    //dacValue(variable) = counter(variable) * slope
    //Find bytes
    //Write to DAC
    //Seperate counter(variable) into single digits
    //Write digits to segments
    
    if(tracker == 1){

        //multiply encoder value by slope for DAC I2C 
        dacValue = counterF * 4.095;
        
        // limit to 12-bit
        if(dacValue > 4095)
        {
            dacValue = 4095;
        }

        // First byte:
        // 0000 D11 D10 D9 D8
        byte firstByte = dacValue >> 8;

        // Second byte:
        // D7 D6 D5 D4 D3 D2 D1 D0
        byte secondByte = dacValue & 0xFF;

        //write to DAC
        Wire.beginTransmission(0x60);
        Wire.write(firstByte);
        Wire.write(secondByte);
        Wire.endTransmission();


        //Seperate counter(variable) into single digits
        int a = counterF / 1000;
        int b = (counterF / 100) % 10;
        int c = (counterF / 10) % 10;
        int d = counterF % 10;

        //Write digits to segments
        WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
        WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
        WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
        WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
        
        if(counterF > 255){
            EEPROM.update(0, 255);
            EEPROM.update(1, counterF - 255); 
        }
        else{
            EEPROM.update(1, 0);
        }
    
    }
        else if(tracker == 2){

            //multiply encoder value by slope for DAC I2C 
            float tempF = (counterC * 9.0 / 5.0) + 32.0;
            dacValue = tempF * 4.095;
            
            // limit to 12-bit
            if(dacValue > 4095)
            {
                dacValue = 4095;
            }

            // First byte:
            // 0000 D11 D10 D9 D8
            byte firstByte = dacValue >> 8;

            // Second byte:
            // D7 D6 D5 D4 D3 D2 D1 D0
            byte secondByte = dacValue & 0xFF;

            //write to DAC
            Wire.beginTransmission(0x60);
            Wire.write(firstByte);
            Wire.write(secondByte);
            Wire.endTransmission();


            //Seperate counter(variable) into single digits
            int a = counterC / 1000;
            int b = (counterC / 100) % 10;
            int c = (counterC / 10) % 10;
            int d = counterC % 10;

            //Write digits to segments
            WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
            WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
            WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
            WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
        }
            else if(tracker == 3){

                //multiply encoder value by slope for DAC I2C 
                dacValue = counterP * 40.95;
                
                // limit to 12-bit
                if(dacValue > 4095)
                {
                    dacValue = 4095;
                }

                // First byte:
                // 0000 D11 D10 D9 D8
                byte firstByte = dacValue >> 8;

                // Second byte:
                // D7 D6 D5 D4 D3 D2 D1 D0
                byte secondByte = dacValue & 0xFF;

                //write to DAC
                Wire.beginTransmission(0x60);
                Wire.write(firstByte);
                Wire.write(secondByte);
                Wire.endTransmission();


                //Seperate counter(variable) into single digits
                int a = counterP / 1000;
                int b = (counterP / 100) % 10;
                int c = (counterP / 10) % 10;
                int d = counterP % 10;

                //Write digits to segments
                WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
                WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
            }
                else if(tracker == 4){

                //multiply encoder value by slope for DAC I2C 
                dacValue = counterV * 409.5;
                
                // limit to 12-bit
                if(dacValue > 4095)
                {
                    dacValue = 4095;
                }

                // First byte:
                // 0000 D11 D10 D9 D8
                byte firstByte = dacValue >> 8;

                // Second byte:
                // D7 D6 D5 D4 D3 D2 D1 D0
                byte secondByte = dacValue & 0xFF;

                //write to DAC
                Wire.beginTransmission(0x60);
                Wire.write(firstByte);
                Wire.write(secondByte);
                Wire.endTransmission();


                //Seperate counter(variable) into single digits
                int a = counterV / 1000;
                int b = (counterV / 100) % 10;
                int c = (counterV / 10) % 10;
                int d = counterV % 10;

                //Write digits to segments
                WriteCharacter(Character (a) , B_DIG_1_LOWER_SEG, 0);
                WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
            }







}
>>>>>>> revP1
