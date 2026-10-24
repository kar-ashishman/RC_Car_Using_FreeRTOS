#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class Mutex {
    public:
        Mutex(long unsigned int timeOut);
        ~Mutex();
        bool lock();
        void unlock();
    
    private:
        long unsigned int timeOut_;
        xSemaphoreHandle handle_;
};