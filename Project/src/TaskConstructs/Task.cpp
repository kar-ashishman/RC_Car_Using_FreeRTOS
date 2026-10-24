/*
FileName: Task.cpp
LastEditedBy: Ashish
Date: 23-10-2026
Description:
    Task Class implementation

*/

#include "src/Includes/Task.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Data copy to create object
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

// Tries to create a task in heap, returns true if successful
// task is created with entry method as Task::taskEntry where 
// entry method argument is the this pointer.
// Hence, taskEntry needs to be defined as taskEntry(void *this)
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


Task::~Task() {
    /* Delete the Task, This may never occur but in case it does
       we make sure we clear resources */
    if (taskHandle_ != nullptr) {
        vTaskDelete(*taskHandle_);
    }
}

// Entry point of a task, takes a task object as argument and executes the updateRoutine
void Task::taskEntry(void* thisTask) {
    Task* currentTask = static_cast<Task*>(thisTask);
    currentTask->updateRoutine();
}

uint16_t Task::getTaskRate() {
    return taskRate_;
}