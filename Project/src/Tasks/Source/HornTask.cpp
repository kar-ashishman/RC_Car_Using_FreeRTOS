#include "src/Tasks/Includes/HornTask.hpp"
#include "src/Services/Includes/HornDriver.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/Notify.hpp"
#include <memory>

HornTask::HornTask() : Task(
        "HornTask", 
        2048, 
        1, 
        CoreId::CONTROL_CORE,
        0
    ) {
        pinMode(hornPin_, OUTPUT);
    }

HornTask::~HornTask() {
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

void HornTask::updateRoutine() {
    while(true)
    {   
        Notify::waitForNotification();
        switch(signal_) {
            case Signals::HORNOFF:
                HornDriver::hornOff(hornPin_);
                break;
            case Signals::HORNON:
                HornDriver::hornOn(hornPin_);
                break;
            case Signals::HORNDANCE:
                HornDriver::dancingHorn(hornPin_, *this);
                break;
        }
        signal_ = Signals::NONE;
    }   
}
