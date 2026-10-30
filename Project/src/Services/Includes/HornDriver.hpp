#pragma once
#include <stdint.h>

class HornDriver{
    public:
    HornDriver(uint8_t pin);
    void hornON();
    void hornOFF();
    ~HornDriver() = default;

    private:
    uint8_t hornPin_;
};