#pragma once
#include <stdint.h>

class HornDriver{
    public:
    HornDriver(uint8_t pin);
    void hornON();
    void hornOFF();
    uint8_t getPin();
    ~HornDriver() = default;

    private:
    uint8_t hornPin_;
};

class DancingHorn:public HornDriver{
    public:
    DancingHorn(uint8_t pin);
    void Dance();
};