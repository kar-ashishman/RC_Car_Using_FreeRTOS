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

uint8_t HornDriver::getPin(){
    return hornPin_;
}

DancingHorn::DancingHorn(uint8_t pin):
    HornDriver(pin){}

void DancingHorn::Dance(){
    digitalWrite(getPin(),HIGH);
    vTaskDelay(500);
    digitalWrite(getPin(),LOW);
    vTaskDelay(200);
    digitalWrite(getPin(),HIGH);
    vTaskDelay(1000);
    digitalWrite(getPin(),LOW);
    vTaskDelay(500);
}

