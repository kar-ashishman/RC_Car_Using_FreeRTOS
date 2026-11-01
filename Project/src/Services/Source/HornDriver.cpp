#include "src/Services/Includes/HornDriver.hpp"
#include "src/Tasks/Includes/HornTask.hpp"
#include "Arduino.h"

void HornDriver::hornOn(uint8_t pin){
    digitalWrite(pin ,HIGH);
}

void HornDriver::hornOff(uint8_t pin){
    digitalWrite(pin ,LOW);
}

void HornDriver::dancingHorn(uint8_t pin, HornTask& hornTask){
    digitalWrite(pin ,HIGH);
    vTaskDelay(500);
    if(hornTask.getParam() == Signals::HORNOFF) {HornDriver::hornOff(pin); return;};
    digitalWrite(pin ,LOW);
    vTaskDelay(200);
    if(hornTask.getParam() == Signals::HORNOFF) {HornDriver::hornOff(pin); return;}
    digitalWrite(pin ,HIGH);
    vTaskDelay(1000);
    if(hornTask.getParam() == Signals::HORNOFF) {HornDriver::hornOff(pin); return;};
    digitalWrite(pin ,LOW);
    vTaskDelay(500);
}

