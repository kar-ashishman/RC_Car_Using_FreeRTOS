/*
FileName: Task.hpp
LastEditedBy: Ashish
Date: 23-10-2026
Description:
    Class definition for creating freeRTOS tasks.
    This class abstracts freeRTOS task create API calls
    with modern C++ RAII constructs.

    A freeRTOS task create API call for ESP
    BaseType_t xTaskCreatePinnedToCore(
    TaskFunction_t pvTaskCode,
    const char * const pcName,
    const uint32_t usStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t * const pxCreatedTask,
    const BaseType_t xCoreID)
*/

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "src/Includes/GlobalTypes.hpp"
#include <stdint.h>
#include <memory>

class Task {
public:
    Task(
        const char* const taskName_,
        const uint32_t stackDepth_,
        UBaseType_t priority_,
        const CoreId coreId_,
        const uint16_t taskRate_ 
    );

    virtual ~Task();

    bool taskCreate();

    /* Static method for the FreeRTOS task entry point */
    static void taskEntry(void* thisTask);

    uint16_t getTaskRate();
    TaskHandle_t getHandle();

    void updateParam(Signals& signal) {
        signal_ = signal;
    }

    Signals getParam() {
        return signal_;
    }

protected:
    const char * const taskName_;
    const uint32_t stackDepth_;
    Signals signal_ = Signals::NONE;
    UBaseType_t priority_;
    TaskHandle_t taskHandle_;
    const CoreId coreId_;
    const uint16_t taskRate_;

    /* Task Functionality */
    virtual void updateRoutine() = 0;

};