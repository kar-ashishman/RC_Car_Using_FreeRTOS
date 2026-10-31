#include "src/Tasks/Includes/LightsTask.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include <memory>
#include "src/Includes/Notify.hpp"

LightsTask::LightsTask() : Task(
        "LightsTask", 
        2048, 
        nullptr, 
        1, 
        CoreId::CONTROL_CORE,
        2000
    ) {
        pinMode(2, OUTPUT);
    }

LightsTask::~LightsTask() {
    Serial.println("Deleting Lights");
    digitalWrite(2, LOW);
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

void LightsTask::updateRoutine() {
    static char state = 0;
    Serial.println("Waiting for notification");
    Notify::waitForNotification();
    while (true) {
        state = state ^ 1;
        digitalWrite(2, state);
        vTaskDelay(500);
    }
}