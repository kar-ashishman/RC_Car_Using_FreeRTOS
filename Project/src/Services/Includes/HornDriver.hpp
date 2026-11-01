#pragma once
#include <stdint.h>
#include "src/Tasks/Includes/HornTask.hpp"

class HornDriver{
    public:
    HornDriver() = default;
    void static hornOn(uint8_t pin);
    void static hornOff(uint8_t pin);
    void static dancingHorn(uint8_t pin, HornTask& hornTask);

    ~HornDriver() = default;

    private:
    uint8_t hornPin_;
};