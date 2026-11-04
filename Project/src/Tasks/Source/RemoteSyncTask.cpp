#include "src/Tasks/Includes/RemoteSyncTask.hpp"
#include "src/Includes/Notify.hpp"
#include "Arduino.h"

RemoteSyncTask::RemoteSyncTask() : Task(
        "RemoteSyncTask", 
        2048, 
        1, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

RemoteSyncTask::~RemoteSyncTask() {
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

void RemoteSyncTask::updateRoutine() {
    while (true) {
        TickType_t lastWakeTime = xTaskGetTickCount();
        bool notified = Notify::waitForNotification(3000);

        if(!notified) {
            // Timeout occurred, perform timeout actions here
            // For example, you can log a message or take some other action
            Serial.println("Timeout occurred in RemoteSyncTask");
            continue;
        }

        switch(signal_) {
            case Signals::SYNC:
                // Perform synchronization actions here
                break;
            case Signals::NONE:
                // No action needed
                break;
        }
    }
}