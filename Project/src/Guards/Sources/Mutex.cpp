#include "src/Guards/Includes/Mutex.hpp"

Mutex::Mutex() : 
    handle_(xSemaphoreCreateMutex()) {}

Mutex::~Mutex() {
    if (handle_ != nullptr) {
        unlock();
        vSemaphoreDelete(handle_);
    }
}

bool Mutex::lock(long unsigned int timeOut) {
    return xSemaphoreTake(handle_, timeOut) == pdPASS;
}

void Mutex::unlock() {
    if (handle_ == nullptr) {
        return;
    } else {
        xSemaphoreGive(handle_);
    }
}