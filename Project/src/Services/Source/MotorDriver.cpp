#include "src/Services/Includes/MotorDriver.hpp"
#include "Arduino.h"

MotorDriver::MotorDriver(uint8_t pinHigh, uint8_t pinLow, uint8_t resolution, uint32_t freq) : 
                                        pinHigh_(pinHigh),
                                        pinLow_(pinLow),
                                        resolution_(resolution),
                                        freq_(freq) {
    // code to check for pin validity
    analogWriteResolution(pinHigh_, resolution_);
    analogWriteFrequency(pinHigh_, freq_);
    analogWriteResolution(pinLow_, resolution_);
    analogWriteFrequency(pinLow_, freq_);

    // set pins default low
    analogWrite(pinHigh_, 0);
    analogWrite(pinLow_, 0);
}

void MotorDriver::motorControl(uint8_t speed, Direction direction) {
    // 2 << resolution_ = 100%
    // For forward, make pinHigh high and pinLow low, and viceversa
    // For active stop, make both pins HIGH
    int scaledSpeed = (speed * (2 << resolution_)) / 100;

    if(direction == Direction::FORWARD) {
        analogWrite(pinLow_, 0);
        analogWrite(pinHigh_, scaledSpeed);
    
    } else if(direction == Direction::STOP) {
        analogWrite(pinHigh_, (2 << resolution_ - 1));
        analogWrite(pinLow_, (2 << resolution_ - 1));
    
    } else if(direction == Direction::BACKWARD) {
        analogWrite(pinHigh_, 0);
        analogWrite(pinLow_, scaledSpeed);
    } else {
        analogWrite(pinLow_, 0);
        analogWrite(pinHigh_, 0);
    }
}
