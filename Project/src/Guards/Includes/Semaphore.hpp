#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "src/Guards/Includes/Mutex.hpp"

class Semaphore {
    /*
    This class creates a counting semaphore 
    - maxCount: maximum semaphore count
    - initialCount: initial semaphore count
    - timeOut: timeout used by lock when taking semaphore
    */
    public:
    Semaphore(unsigned int maxCount=0,unsigned int initialCount=0);
    ~Semaphore();

    bool lock();
    bool lockFromISR(BaseType_t *higherPriorityTaskWoken);
    bool unlock();
    bool unlockFromISR(BaseType_t *higherPriorityTaskWoken);
    void setTimeout(long unsigned int timeOut);
    xSemaphoreHandle getHandle();

    private:
    unsigned int maxCount_=0;
    unsigned int intialCount_=0;
    xSemaphoreHandle handle_;
    long unsigned int timeOut_=0;
};
