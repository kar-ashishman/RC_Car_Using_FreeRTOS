#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class Semaphore final{
    /*
    This class creates a counting semaphore 
    - maxCount: maximum semaphore count
    - initialCount: initial semaphore count
    - timeOut: timeout used by lock when taking semaphore
    */
    public:
    Semaphore(unsigned int maxCount=0,unsigned int initialCount=0);
    ~Semaphore();

    bool lock(long unsigned int timeOut);
    bool lockFromISR(BaseType_t *higherPriorityTaskWoken);
    bool unlock();
    bool unlockFromISR(BaseType_t *higherPriorityTaskWoken);
    xSemaphoreHandle getHandle();

    private:
    unsigned int maxCount_=0;
    unsigned int intialCount_=0;
    xSemaphoreHandle handle_;
};
