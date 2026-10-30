#include "src/Guards/Includes/Semaphore.hpp"

Semaphore::Semaphore(unsigned int maxCount, unsigned int initialCount):
            maxCount_(maxCount),intialCount_(initialCount){
                handle_ = xSemaphoreCreateCounting(maxCount_,intialCount_);
            }

Semaphore::~Semaphore() {
    if (handle_ != nullptr) {
        unlock();
        vSemaphoreDelete(handle_);
    }
}

bool Semaphore::lock(long unsigned int timeOut) {
    return xSemaphoreTake(handle_,timeOut) == pdTRUE;
};

bool Semaphore::unlock() {
    return xSemaphoreGive(handle_) == pdTRUE;
};

xSemaphoreHandle Semaphore::getHandle() {
    return Semaphore::handle_;
}

bool Semaphore::lockFromISR(BaseType_t *higherPriorityTaskWoken) {
    return xSemaphoreTakeFromISR(handle_,higherPriorityTaskWoken)==pdTRUE;
}

bool Semaphore::unlockFromISR(BaseType_t *higherPriorityTaskWoken) {
    return xSemaphoreGiveFromISR(handle_,higherPriorityTaskWoken)==pdTRUE;
}