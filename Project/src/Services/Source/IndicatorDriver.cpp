#include "src/Services/Includes/IndicatorDriver.hpp"
#include "Arduino.h"

IndicatorDriver::IndicatorDriver(uint8_t lPin, uint8_t rPin):
            lPin_(lPin),rPin_(rPin){}

void IndicatorDriver::setLeftIndicatorON(){
    digitalWrite(lPin_,HIGH);
}

void IndicatorDriver::setLeftIndicatorOFF(){
    digitalWrite(lPin_,LOW);
}

void IndicatorDriver::setRightIndicatorON(){
    digitalWrite(rPin_,HIGH);
}

void IndicatorDriver::setRightIndicatorOFF(){
    digitalWrite(rPin_,LOW);
}
