<<<<<<< HEAD


#ifndef _DEFINES_H_
#define _DEFINES_H_


// See Manual Analog Control Pinout AVR64DB32 for pinout map

/*---------------DEFINES---------------*/

    //I2C  
#define SDA_PIN             PIN_PA2
#define SCL_PIN             PIN_PA3

    //addresses
#define DAC
#define DRIVER

    //LED Indicators
#define degF_LED            PIN_PC3      
#define degC_LED            PIN_PC2
#define percent_LED         PIN_PC0
#define volt_LED            PIN_PC1

    //Encoder
#define encA                PIN_PA5
#define encB                PIN_PA4
#define enc_Button          PIN_PA6

    //Tactile Button
#define mode_Button         PIN_PA7

    //Serial monitor
#define dbgSerial           Serial2          
#define DBG_TxD             PIN_PF0
#define DBG_RxD             PIN_PF1
#define baudRate            9600

    //delay
#define DELAY               2000

    //EEPROM Addresses
#define E_TEMP_F_LOW        0
#define E_TEMP_F_HIGH       1
#define E_TEMP_C            4
#define E_PERCENT           8   
#define E_VOLTAGE           12
#define E_MODE              14


=======


#ifndef _DEFINES_H_
#define _DEFINES_H_


// See Manual Analog Control Pinout AVR64DB32 for pinout map

/*---------------DEFINES---------------*/

    //I2C  
#define SDA_PIN             PIN_PA2
#define SCL_PIN             PIN_PA3

    //addresses
#define DAC
#define DRIVER

    //LED Indicators
#define degF_LED            PIN_PC3      
#define degC_LED            PIN_PC2
#define percent_LED         PIN_PC0
#define volt_LED            PIN_PC1

    //Encoder
#define encA                PIN_PA5
#define encB                PIN_PA4
#define enc_Button          PIN_PA6

    //Dip Switch Inputs
#define dipStatusOne        PIN_PD3
#define dipStatusTwo        PIN_PD1
#define dipStatusFour       PIN_PD4
#define dipStatusEight      PIN_PD2

    //Serial monitor
#define dbgSerial           Serial2          
#define DBG_TxD             PIN_PF0
#define DBG_RxD             PIN_PF1
#define baudRate            9600

    //delay
#define DELAY               2000

    //EEPROM Addresses
#define E_TEMP_F_LOW        0
#define E_TEMP_F_HIGH       1
#define E_TEMP_C            4
#define E_PERCENT           8   
#define E_VOLTAGE           12
#define E_MODE              14


>>>>>>> revP1
#endif