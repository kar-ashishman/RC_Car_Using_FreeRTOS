#include "src/Services/Includes/HornDriver.hpp"
#include "Arduino.h"

HornDriver::HornDriver(uint8_t pin):
            hornPin_(pin){}

void HornDriver::hornON(){
    digitalWrite(hornPin_,HIGH);
}

void HornDriver::hornOFF(){
    digitalWrite(hornPin_,LOW);
}

