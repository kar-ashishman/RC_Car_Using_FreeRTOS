#include "src/Services/Includes/LightDriver.hpp"
#include "Arduino.h"
#include "src/Includes/Task.hpp"


void LeftIndicator::lightON(uint8_t pin, uint16_t delay, IndicatorTask& indicatorTask) {
        while(1){
           digitalWrite(pin,HIGH);
           vTaskDelay(delay);
           digitalWrite(pin,LOW);
           vTaskDelay(delay);  
        }  
}

void RightIndicator::lightON(uint8_t pin, uint16_t delay, IndicatorTask& indicatorTask) {
        while(1){
           digitalWrite(pin,HIGH);
           vTaskDelay(delay);
           digitalWrite(pin,LOW);
           vTaskDelay(delay);
        }
}

void LeftIndicator::lightOFF(uint8_t pin) {
        digitalWrite(pin,LOW);
}

void RightIndicator::lightOFF(uint8_t pin) {
        digitalWrite(pin,LOW);
}

void HeadLight::lightON(uint8_t pin) {
        digitalWrite(pin,HIGH);
}

void HeadLight::lightOFF(uint8_t pin) {
        digitalWrite(pin,LOW);
}