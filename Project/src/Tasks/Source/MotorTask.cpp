#include "src/Tasks/Includes/MotorTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "src/Includes/Notify.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"

MotorTask::MotorTask() : Task(
        "MotorTask", 
        2048, 
        nullptr, 
        1, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

MotorTask::~MotorTask() {
    vTaskDelete(nullptr);
}

void MotorTask::updateRoutine() {
    
    LightsTask* lt = new LightsTask;
    vTaskDelay(3000);
    lt->taskCreate();
    vTaskDelay(3000);
    Notify::wakeTask(*lt);
    Serial.println("LightTask Notified Going to sleep");
    vTaskDelay(3000);
    Serial.println("Wakingup and going to delete light");
    delete lt;
    Serial.println("Deleting light");

    while (true) {
        // Critical Section
        {   
            //LockGuard<Mutex> lock(serialMutex);
            Serial.println("Motor control task running");
        }
        vTaskDelay(getTaskRate());
    }
}