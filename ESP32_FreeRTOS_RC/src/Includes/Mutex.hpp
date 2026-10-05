#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class Mutex {
    public:
        Mutex(uint16_t timeOut);
        ~Mutex();
        bool lock();
        void unlock();
    
    private:
        uint16_t timeOut_;
        xSemaphoreHandle handle_;
};
