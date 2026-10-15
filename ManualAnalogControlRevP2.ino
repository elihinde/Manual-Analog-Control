
#include "Defines.h"
#include <Wire.h>
#include "DisplayDefines.h"
#include "Driver.h"
#include <EEPROM.h>

AnalogControl Core;

void setup() {

    Core.SetupCore();
    Core.SetupDisplay();

}


void loop() {

    Core.Loop();

}

