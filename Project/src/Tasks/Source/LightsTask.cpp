#include "src/Tasks/Includes/LightsTask.hpp"
#include "src/Services/Includes/LightDriver.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include <memory>
#include "src/Includes/Notify.hpp"

LightsTask::LightsTask() : Task(
        "LightsTask", 
        2048, 
        1, 
        CoreId::CONTROL_CORE,
        50
    ) {
        pinMode(pin_, OUTPUT);
    }

LightsTask::~LightsTask() {
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

void LightsTask::updateRoutine() {

    while (true) {
        Notify::waitForNotification();
        switch(signal_) {
            case Signals::HEADLIGHTON:
                HeadLight::lightON(pin_);
                break;
            case Signals::HEADLIGHTOFF:
                HeadLight::lightOFF(pin_);
                break;
        }
        signal_ = Signals::NONE;
    }
}