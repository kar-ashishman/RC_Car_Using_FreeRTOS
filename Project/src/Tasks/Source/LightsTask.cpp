#include "src/Tasks/Includes/LightsTask.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"

LightsTask::LightsTask() : Task(
        "LightsTask", 
        2048, 
        nullptr, 
        1, 
        nullptr, 
        CoreId::CONTROL_CORE,
        2000
    ) {}

LightsTask::~LightsTask() {
    vTaskDelete(nullptr);
}

void LightsTask::updateRoutine() {
    while (true) {
        // Critical Section
        {   
            LockGuard<Mutex> lock(serialMutex);
            Serial.println("Light control task running");
        }
        vTaskDelay(getTaskRate());
    }
}