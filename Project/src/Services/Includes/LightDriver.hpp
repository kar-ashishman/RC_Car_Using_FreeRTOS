#pragma once
#include <stdint.h>

class LightDriver{
    public:
    LightDriver(uint8_t pin);
    ~LightDriver() = default;
    void lightON(uint16_t delay);
    void lightOFF();
    uint8_t getPin();

    private:
    uint8_t pin_;
};

class LeftIndicator : public LightDriver {
public:
    LeftIndicator(uint8_t pin);
    void lightON(uint16_t delay);
    void lightOFF();
};

class RightIndicator : public LightDriver {
public:
    RightIndicator(uint8_t pin);
    void lightON(uint16_t delay);
    void lightOFF();
};