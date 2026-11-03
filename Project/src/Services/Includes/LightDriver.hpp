#pragma once
#include <stdint.h>
#include "src/Tasks/Includes/IndicatorTask.hpp"

class LightDriver{
    public:
    LightDriver() = default;
    ~LightDriver() = default;
    void static lightON(uint8_t pin, uint16_t delay);
    void static lightOFF(uint8_t pin);
};

class LeftIndicator : public LightDriver {
public:
    LeftIndicator() = default;
    void static lightON(uint8_t pin, uint16_t delay, IndicatorTask& indicatorTask);
    void static lightOFF(uint8_t pin);
};

class RightIndicator : public LightDriver {
public:
    RightIndicator() = default;
    void static lightON(uint8_t pin, uint16_t delay, IndicatorTask& indicatorTask);
    void static lightOFF(uint8_t pin);
};

class HeadLight: public LightDriver {
public:
    HeadLight() = default;
    ~HeadLight() = default;
    void static lightON(uint8_t pin);
    void static lightOFF(uint8_t pin);
};