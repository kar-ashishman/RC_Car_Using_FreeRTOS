#include "src/Includes/MotorTask.hpp"
#include "Arduino.h"

MotorTask::MotorTask() : Task(
        "MotorTask", 
        2048, 
        nullptr, 
        1, 
        nullptr, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

void MotorTask::update(void *parameters) {
    while (true) {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        // Motor control logic here


        Serial.println("Motor control task running");
        vTaskDelayUntil(&xLastWakeTime, taskRate_ / portTICK_PERIOD_MS);
    }
}

MotorTask::~MotorTask() {
    vTaskDelete(nullptr);
}