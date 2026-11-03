#include "src/Tasks/Includes/IndicatorTask.hpp"
#include "src/Services/Includes/LightDriver.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/Notify.hpp"
#include <memory>

IndicatorTask::IndicatorTask() : Task(
        "IndicatorTask", 
        2048,  
        1,
        CoreId::CONTROL_CORE,
        0
    ) {
        pinMode(lPin_, OUTPUT);
        pinMode(rPin_, OUTPUT);
    }

IndicatorTask::~IndicatorTask() {
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

void IndicatorTask::updateRoutine() {
    while(true)
    {   
        Notify::waitForNotification();
        switch(signal_)
        {
            case Signals::IND_LEFT:
                LeftIndicator::lightON(lPin_,500,*this);
                RightIndicator::lightOFF(rPin_);
                break;
            
            case Signals::IND_RIGHT:
                RightIndicator::lightON(rPin_,500,*this);
                LeftIndicator::lightOFF(lPin_);
                break;
            
            case Signals::HAZARD:
                LeftIndicator::lightON(lPin_,500,*this);
                RightIndicator::lightON(rPin_,500,*this);
                break;

            case Signals::IND_OFF:
                LeftIndicator::lightOFF(lPin_);
                RightIndicator::lightOFF(rPin_);
                break;
        }
        signal_ = Signals::NONE;
    }   
}
