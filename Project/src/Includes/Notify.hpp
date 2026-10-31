#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include "src/Includes/Task.hpp"

class Notify final {
    public:
    Notify() = default;
    ~Notify() = default;
    static void wakeTask(Task& task) {
        xTaskNotifyGive(task.getHandle());
    }
    static bool waitForNotification(uint32_t timeOut = portMAX_DELAY) {
        return ulTaskNotifyTake(pdTRUE, timeOut) > 0;
    }
    static void wakeTaskFromISR(Task& task, BaseType_t *higherPriorityTaskWorken) {
        vTaskNotifyGiveFromISR(task.getHandle(), higherPriorityTaskWorken);
    }
};