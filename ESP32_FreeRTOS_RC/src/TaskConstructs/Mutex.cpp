#include "src/Includes/Mutex.hpp"

Mutex::Mutex(uint16_t timeOut = portMAX_DELAY) : 
    handle_(xSemaphoreCreateMutex()),
    timeOut_(timeOut) {}

Mutex::~Mutex() {
    if (handle_ != nullptr) {
        unlock();
        vSemaphoreDelete(handle_);
    }
}

bool Mutex::lock() {
    return xSemaphoreTake(handle_, timeOut_) == pdPASS;
}

void Mutex::unlock() {
    if (handle_ == nullptr) {
        return;
    } else {
        xSemaphoreGive(handle_);
    }
}
