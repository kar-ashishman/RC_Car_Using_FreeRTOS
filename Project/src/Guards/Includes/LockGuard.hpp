#pragma once
#include "freertos/FreeRTOS.h"
#include "src/Guards/Includes/Mutex.hpp"

/* Class to provide RAII-style locking for a Mutex */
template<typename T>
class LockGuard {
    public:
    LockGuard(T& kernelObj, long unsigned int timeOut = portMAX_DELAY) : handle_t(kernelObj) {
        kernelObj.lock(timeOut);
    }
    // LockGuard(Semaphore& semaphore)
    ~LockGuard() {
        handle_t.unlock();
    }

    private:
    T &handle_t;
};