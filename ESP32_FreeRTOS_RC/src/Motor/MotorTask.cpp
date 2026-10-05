#include "src/Includes/MotorTask.hpp"
#include "Arduino.h"
#include "src/Includes/LockGuard.hpp"

extern Mutex serialMonitorMutex;

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
        // can call multiple function calls here
        LockGuard<Mutex> lock(serialMonitorMutex);
        Serial.println("Motor control task running"); 
    }
}
