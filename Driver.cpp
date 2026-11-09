
#include "Defines.h"
#include <Wire.h>
#include "DisplayDefines.h"
#include "Driver.h"
#include <EEPROM.h>



volatile int counter;
volatile long counterRTDF;
volatile int counterRTDC;
volatile int counterF;
volatile int counterC;
volatile int counterP;
volatile int counterV;
volatile int mode;
volatile int counterF_New;
volatile int dipSwitch;
volatile int dipOne;
volatile int dipTwo;
volatile int dipFour;
volatile int dipEight;
unsigned long last_run = 0;
double counterNew;
int tracker;
int dacValue;

// ***NEW*** tracking variables to find when eeprom updates
volatile int trackcounterRTDF;
volatile int trackcounterRTDC;
volatile int trackcounterF;
volatile int trackcounterC;
volatile int trackcounterV;
volatile int trackcounterP;

// ***NEW*** bools to control when eeprom does change
volatile bool eepromsave = false;


// ***NEW*** Global variables
bool encpush;                       //tracks the enc button

volatile bool editMode = false;     //holds edit mode
bool lastEncPush = LOW;             //holds value for last 
unsigned long lastButtonTime = 0;   //holds time for last button press

// ***NEW*** display flashing
bool displayVisible = true;
unsigned long lastDisplayFlash = 0;
bool blinkMode = false;             //tracks if display is blinking

void ISR_ENCA(){
    //***NEW***
    //Ignore encoder movement unless edit mode is enabled
    if(editMode == false){
        return;
    }

    switch (tracker){
        case 0:
        
            if(millis() - last_run > 5){                //only run every 5ms - acts as a debouncing technique so the encoder isnt read too frequently
                if(digitalRead(encB) == 1){             //if enc b is high
                    if(counterRTDF < 250){              //top limit (prevent from going over this value)
                        counterRTDF++;                  //Increase counter
                        eepromsave = true;
                    }
                }
                        if(digitalRead(encB) == 0){     //if enc b is low aka turning counter clockwise
                            if(counterRTDF > 0){        //set lower limit to 0
                                counterRTDF--;          //subtract from counter variable
                                eepromsave = true;
                            }  
                        }
                last_run = millis();                    //save current time
            } 

        break;

        case 1:
        
            if(millis() - last_run > 5){
                if(digitalRead(encB) == 1){
                    if(counterRTDC < 180){
                        counterRTDC++;
                        eepromsave = true;
                    }
                }
                        if(digitalRead(encB) == 0){
                            if(counterRTDC > 0){
                                counterRTDC--;
                                eepromsave = true;
                            }  
                        }
                last_run = millis();
            }

        break;

        case 2:
            if(millis() - last_run > 5){
                if(digitalRead(encB) == 1){
                    if(counterF < 350){
                        counterF++;
                        eepromsave = true;
                    }
                }
                        if(digitalRead(encB) == 0){
                            if(counterF > 0){
                                counterF--;
                                eepromsave = true;
                            }  
                        }
                last_run = millis();
            } 
        break;

        case 3:
        
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterC < 180){
                    counterC++;
                    eepromsave = true;
                }
            }
        if(digitalRead(encB) == 0){
            if(counterC > 0){
                counterC--;
                eepromsave = true;
            }  
        }
        last_run = millis();
    } 


        break;

        case 4:
        
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterV < 100){
                    counterV++;
                    eepromsave = true;
                }
            }
        if(digitalRead(encB) == 0){
            if(counterV > 0){
                counterV--;
                eepromsave = true;
            }  
        }
        last_run = millis();
    } 

        break;

        case 5:
        
        if(millis() - last_run > 5){
            if(digitalRead(encB) == 1){
                if(counterP < 100){
                    counterP++;
                    eepromsave = true;
                }
            }
        if(digitalRead(encB) == 0){
            if(counterP > 0){
                counterP--;
                eepromsave = true;
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
    dbgSerial.println("Test");

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

    counterRTDF = EEPROM.read(0);
    counterRTDC = EEPROM.read(4);
    counterF = EEPROM.read(8);
    counterC = EEPROM.read(12);
    counterV = EEPROM.read(16);
    counterP = EEPROM.read(20);
    // counterF = EEPROM.read(0) + EEPROM.read(1);
    // counterC = EEPROM.read(4);
    // counterP = EEPROM.read(8);
    // counterV = EEPROM.read(12);

    attachInterrupt(digitalPinToInterrupt(encA), ISR_ENCA, LOW);

    //set bool to false 
    encpush = false;
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
dipOne = digitalRead(dipStatusOne);
dipTwo = digitalRead(dipStatusTwo);
dipFour = digitalRead(dipStatusFour);
dipEight = digitalRead(dipStatusEight);
encpush = digitalRead(enc_Button);

if(encpush == HIGH && lastEncPush == LOW && millis() - lastButtonTime > 100){
    bool wasEditing = editMode;

    editMode = !editMode;

        if(wasEditing && !editMode && eepromsave)
        {
            switch(tracker)
            {
                case 0:
                    EEPROM.put(0, counterRTDF);
                    break;

                case 1:
                    EEPROM.put(4, counterRTDC);
                    break;

                case 2:
                    EEPROM.put(8, counterF);
                    break;

                case 3:
                    EEPROM.put(12, counterC);
                    break;

                case 4:
                    EEPROM.put(16, counterV);
                    break;

                case 5:
                    EEPROM.put(20, counterP);
                    break;
            }

            dbgSerial.println("EEPROM Saved");
            eepromsave = false;
}       // lock or unlock edit mode
    lastButtonTime = millis();
}

lastEncPush = encpush;

// ************************************************
// NEW - Flash display while editing
// ************************************************

// Flash display while editing
if(editMode)
{
    blinkMode = true; //this bool controls when data is written to the dac for output signal

    if(displayVisible && millis() - lastDisplayFlash > 1100)
    {
        // Turn display OFF after 1 second
        displayVisible = false;
        lastDisplayFlash = millis();
    }

    if(!displayVisible && millis() - lastDisplayFlash > 150)
    {
        // Turn display back ON after 100 ms
        displayVisible = true;
        lastDisplayFlash = millis();
    }
}
else
{
    displayVisible = true; 

    blinkMode = false; //bool control for output signal
}

//Dip swtich inputs manipulate the variable of tracker
delay(25); //used to prevent imperfections from the mechanical dipswitch
if(dipEight == 1 && dipFour == 1 && dipTwo == 1 && dipOne == 1){
    tracker = 0;
}
    else if(dipEight == 1 && dipFour == 1 && dipTwo == 1 && dipOne == 0){
        tracker = 1;
    } 
        else if(dipEight == 1 && dipFour == 1 && dipTwo == 0 && dipOne == 1){   
            tracker = 2;
        }
            else if(dipEight == 1 && dipFour == 1 && dipTwo == 0 && dipOne == 0){   
                tracker = 3;
            }
                else if(dipEight == 1 && dipFour == 0 && dipTwo == 1 && dipOne == 1){   
                    tracker = 4;
                }
                    else if(dipEight == 1 && dipFour == 0 && dipTwo == 1 && dipOne == 0){   
                        tracker = 5;
                    }
                        else{
                            tracker = 11;
                        }

//Turn on mode LED
switch (tracker){
    case 0 : case 2: 
        digitalWrite(degF_LED, HIGH);
        digitalWrite(degC_LED, LOW);
        digitalWrite(percent_LED, LOW);
        digitalWrite(volt_LED, LOW);
        break;
    case 1 : case 3:
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, HIGH);
        digitalWrite(percent_LED, LOW);
        digitalWrite(volt_LED, LOW);
        break;
    case 4: 
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, LOW);
        digitalWrite(volt_LED, HIGH);
        digitalWrite(percent_LED, LOW);
        break;
    case 5: 
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, LOW);
        digitalWrite(volt_LED, LOW);
        digitalWrite(percent_LED, HIGH);
        break;
    case 11:
        digitalWrite(degF_LED, HIGH);
        digitalWrite(degC_LED, HIGH);
        digitalWrite(volt_LED, HIGH);
        digitalWrite(percent_LED, HIGH);
        delay(500);
        digitalWrite(degF_LED, LOW);
        digitalWrite(degC_LED, LOW);
        digitalWrite(volt_LED, LOW);
        digitalWrite(percent_LED, LOW);
        delay(500);        
        break; 
}

    //dacValue(variable) = counter(variable) * slope
    //Find bytes
    //Write to DAC
    //Seperate counter(variable) into single digits
    //Write digits to segments

    if (tracker == 0){
                //multiply encoder value by slope for DAC I2C 
                dacValue = ((counterRTDF * 6552) +409500) / 1000;
                
                // limit to 12-bit
                if(dacValue > 4095)
                {
                    dacValue = 4095;
                }

                dacValue = dacValue +5;

                // First byte:
                // 0000 D11 D10 D9 D8
                byte firstByte = dacValue >> 8;

                // Second byte:
                // D7 D6 D5 D4 D3 D2 D1 D0
                byte secondByte = dacValue & 0xFF;

                if(blinkMode == false){
                    //write to DAC
                    Wire.beginTransmission(0x60);
                    Wire.write(firstByte);
                    Wire.write(secondByte);
                    Wire.endTransmission();
                    // EEPROM.write(0, counterRTDF);
                }

                //Seperate counter(variable) into single digits
                int a = counterRTDF / 1000;
                int b = (counterRTDF / 100) % 10;
                int c = (counterRTDF / 10) % 10;
                int d = counterRTDF % 10;

                //***NEW*** Display Flashing
                if(displayVisible)
                {
                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                }
                else
                {
                    // Blank display while flashing
                    WriteCharacter(CHAR_BLANK, B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, T_DIG_4_LOWER_SEG, 0);
                }
                // dbgSerial.print("counterRTD: ");
                // dbgSerial.println(counterRTDF);

                // dbgSerial.print("dacValue: ");
                // dbgSerial.println(dacValue);
                
                // dbgSerial.print("first byte: ");
                // dbgSerial.println(firstByte);

                // dbgSerial.print("second byte: ");
                // dbgSerial.println(secondByte);

                // dbgSerial.print(a);
                // dbgSerial.print(b);
                // dbgSerial.print(c);
                // dbgSerial.println(d);

                // dbgSerial.println("");

    }   
        else if(tracker == 1){
                //multiply encoder value by slope for DAC I2C 
                dacValue = (counterRTDC * 11.796) + 619.473;
                
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

                if(blinkMode == false){
                    //write to DAC
                    Wire.beginTransmission(0x60);
                    Wire.write(firstByte);
                    Wire.write(secondByte);
                    Wire.endTransmission();
                    // EEPROM.write(4, counterRTDC);
                    // dbgSerial.print("EEPROM RTDF: ");
                    // dbgSerial.println(EEPROM.read(0));
                }

                //Seperate counter(variable) into single digits
                int a = counterRTDC / 1000;
                int b = (counterRTDC / 100) % 10;
                int c = (counterRTDC / 10) % 10;
                int d = counterRTDC % 10;

                //***NEW*** Display Flashing
                if(displayVisible)
                {
                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                }
                else
                {
                    // Blank display while flashing
                    WriteCharacter(CHAR_BLANK, B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, T_DIG_4_LOWER_SEG, 0);
                }
        }
            else if(tracker == 2){
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

                if(blinkMode == false){
                    //write to DAC
                    Wire.beginTransmission(0x60);
                    Wire.write(firstByte);
                    Wire.write(secondByte);
                    Wire.endTransmission();
                    // EEPROM.write(8, counterF);
                }

                //Seperate counter(variable) into single digits
                int a = counterF / 1000;
                int b = (counterF / 100) % 10;
                int c = (counterF / 10) % 10;
                int d = counterF % 10;

                //***NEW*** Display Flashing
                if(displayVisible)
                {
                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                }
                else
                {
                    // Blank display while flashing
                    WriteCharacter(CHAR_BLANK, B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, T_DIG_4_LOWER_SEG, 0);
                }

                // if(counterF > 255){
                //     EEPROM.update(0, 255);
                //     EEPROM.update(1, counterF - 255); 
                // }
                // else{
                //     EEPROM.update(1, 0);
                // }
            }   
                else if(tracker == 3){
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

                    if(blinkMode == false){
                        //write to DAC
                        Wire.beginTransmission(0x60);
                        Wire.write(firstByte);
                        Wire.write(secondByte);
                        Wire.endTransmission();
                        // EEPROM.write(12, counterC);
                    }

                    //Seperate counter(variable) into single digits
                    int a = counterC / 1000;
                    int b = (counterC / 100) % 10;
                    int c = (counterC / 10) % 10;
                    int d = counterC % 10;

                //***NEW*** Display Flashing
                if(displayVisible)
                {
                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                }
                else
                {
                    // Blank display while flashing
                    WriteCharacter(CHAR_BLANK, B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, T_DIG_4_LOWER_SEG, 0);
                }
                }   
                    else if(tracker == 4){
                        //multiply encoder value by slope for DAC I2C 
                        dacValue = counterV * 40.95;
                        
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

                        if(blinkMode == false){
                            //write to DAC
                            Wire.beginTransmission(0x60);
                            Wire.write(firstByte);
                            Wire.write(secondByte);
                            Wire.endTransmission();
                            // EEPROM.write(16, counterV);
                        }

                        //Seperate counter(variable) into single digits
                        int a = counterV / 1000;
                        int b = (counterV / 100) % 10;
                        int c = (counterV / 10) % 10;
                        int d = counterV % 10;

                //***NEW*** Display Flashing
                if(displayVisible)
                {
                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 1);
                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                }
                else
                {
                    // Blank display while flashing
                    WriteCharacter(CHAR_BLANK, B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, T_DIG_4_LOWER_SEG, 0);
                }



                    }
                        else if(tracker == 5){
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

                            if(blinkMode == false){
                                //write to DAC
                                Wire.beginTransmission(0x60);
                                Wire.write(firstByte);
                                Wire.write(secondByte);
                                Wire.endTransmission();
                                // EEPROM.write(20, counterP);
                            }

                            //Seperate counter(variable) into single digits
                            int a = counterP / 1000;
                            int b = (counterP / 100) % 10;
                            int c = (counterP / 10) % 10;
                            int d = counterP % 10;

                //***NEW*** Display Flashing
                if(displayVisible)
                {
                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                }
                else
                {
                    // Blank display while flashing
                    WriteCharacter(CHAR_BLANK, B_DIG_1_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_2_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, B_DIG_3_LOWER_SEG, 0);
                    WriteCharacter(CHAR_BLANK, T_DIG_4_LOWER_SEG, 0);
                }
                        }
                            else{
                                    //multiply encoder value by slope for DAC I2C 
                                    dacValue = 0;
                                    


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
                                    int a = 0;
                                    int b = 0;
                                    int c = 0;
                                    int d = 0;

                                    WriteCharacter(Character (a), B_DIG_1_LOWER_SEG, 0);
                                    WriteCharacter(Character (b), B_DIG_2_LOWER_SEG, 0);
                                    WriteCharacter(Character (c), B_DIG_3_LOWER_SEG, 0);
                                    WriteCharacter(Character (d), T_DIG_4_LOWER_SEG, 0);
                                    
                            }   

    
}
