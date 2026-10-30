#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class Mutex {
    public:
        Mutex();
        ~Mutex();
        bool lock(long unsigned int timeOut);
        void unlock();
    
    private:
        long unsigned int timeOut_;
        xSemaphoreHandle handle_;
};