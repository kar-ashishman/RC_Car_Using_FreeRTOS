#pragma once

#include <stdint.h>
#include "src/Includes/GlobalTypes.hpp"

class MotorDriver {
    public:
    MotorDriver(uint8_t pinHigh, uint8_t pinLow, uint8_t resolution, uint32_t freq);  
    ~MotorDriver() = default;
    void motorControl(uint8_t speed, Direction direction);

    private:
    uint8_t pinHigh_, pinLow_;
    uint8_t resolution_;
    uint32_t freq_;
};