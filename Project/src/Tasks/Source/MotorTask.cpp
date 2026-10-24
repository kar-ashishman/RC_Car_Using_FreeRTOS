#include "src/Tasks/Includes/MotorTask.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"

MotorTask::MotorTask() : Task(
        "MotorTask", 
        2048, 
        nullptr, 
        1, 
        nullptr, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

MotorTask::~MotorTask() {
    vTaskDelete(nullptr);
}

void MotorTask::updateRoutine() {
    while (true) {
        // Critical Section
        {   
            LockGuard<Mutex> lock(serialMutex);
            Serial.println("Motor control task running");
        }
        vTaskDelay(getTaskRate());
    }
}