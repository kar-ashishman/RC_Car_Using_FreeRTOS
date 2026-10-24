#include "src/Includes/Task.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

Task::Task(
    const char * const taskName_,
    const uint32_t stackDepth_,
    void * const parameters_,
    UBaseType_t priority_,
    TaskHandle_t * const taskHandle_,
    const CoreId coreId_,
    const uint16_t taskRate_) :
                                taskName_(taskName_),
                                stackDepth_(stackDepth_),
                                parameters_(parameters_),
                                priority_(priority_),
                                taskHandle_(taskHandle_),
                                coreId_(coreId_),
                                taskRate_(taskRate_) {}

bool Task::taskCreate() {
    return xTaskCreatePinnedToCore(
        Task::taskEntry,
        taskName_,
        stackDepth_,
        this,
        priority_,
        taskHandle_,
        static_cast<BaseType_t>(coreId_)
    ) == pdPASS;
}

/* Static method that implements the task functions 
   Override this method in each task
*/
void Task::taskEntry(void* task) {
    auto* currentTask = static_cast<Task*>(task);
    while (1) {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        currentTask->updateRoutine();
        vTaskDelayUntil(&xLastWakeTime, currentTask->taskRate_ / portTICK_PERIOD_MS);
    }
}

Task::~Task() {
    /* Delete the Task, This may never occur but in case it does
       we make sure we clear resources */
    if (taskHandle_ != nullptr) {
        vTaskDelete(*taskHandle_);
    }
}
