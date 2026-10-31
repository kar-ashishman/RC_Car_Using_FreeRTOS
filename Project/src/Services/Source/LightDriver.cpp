#include "src/Services/Includes/LightDriver.hpp"
#include "Arduino.h"
#include "src/Includes/Task.hpp"

LightDriver::LightDriver(uint8_t pin):
             pin_(pin){}

uint8_t LightDriver::getPin(){
    return pin_;
}

LeftIndicator::LeftIndicator(uint8_t pin):
    LightDriver(pin){}

RightIndicator::RightIndicator(uint8_t pin):
    LightDriver(pin){}

void LeftIndicator::lightON(uint16_t delay) {
        digitalWrite(getPin(),HIGH);
        vTaskDelay(delay);
        digitalWrite(getPin(),LOW);
        vTaskDelay(delay);
}

void RightIndicator::lightON(uint16_t delay) {
        digitalWrite(getPin(),HIGH);
        vTaskDelay(delay);
        digitalWrite(getPin(),LOW);
        vTaskDelay(delay);
}

void LeftIndicator::lightOFF() {
        digitalWrite(getPin(),LOW);
}

void RightIndicator::lightOFF() {
        digitalWrite(getPin(),LOW);
}
