#pragma once
#include <stdint.h>

class IndicatorDriver{
    public:
    IndicatorDriver(uint8_t lPin, uint8_t rPin);
    ~IndicatorDriver() = default;
    void setLeftIndicatorON();
    void setLeftIndicatorOFF();
    void setRightIndicatorON();
    void setRightIndicatorOFF();

    private:
    uint8_t lPin_;
    uint8_t rPin_;
};