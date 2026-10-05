#pragma once

#include "src/Includes/Mutex.hpp"

/* Class to provide RAII-style locking for a Mutex */
template<typename T>
class LockGuard {
    public:

    LockGuard(T& kernelObj) : handle_t(kernelObj) {
        kernelObj.lock();
    }
    // LockGuard(Semaphore& semaphore)
    ~LockGuard() {
        handle_t.unlock();
    }

    private:
    T &handle_t;
};
