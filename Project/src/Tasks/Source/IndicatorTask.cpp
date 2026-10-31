#include "src/Tasks/Includes/IndicatorTask.hpp"
#include "src/Services/Includes/LightDriver.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include <memory>

IndicatorTask::IndicatorTask() : Task(
        "IndicatorTask", 
        2048, 
        nullptr, 
        1,
        CoreId::CONTROL_CORE,
        1000
    ) {}

IndicatorTask::~IndicatorTask() {
    vTaskDelete(nullptr);
}

void IndicatorTask::updateRoutine() {
    while(true)
    {   
        std::unique_ptr<LeftIndicator> l_ind = std::make_unique<LeftIndicator>(2);
        std::unique_ptr<RightIndicator> r_ind = std::make_unique<RightIndicator>(2);
        switch(indicatorStatus)
        {
            case Indicator::LEFT:
                l_ind->lightON(500);
                r_ind->lightOFF();
                break;
            
            case Indicator::RIGHT:
                r_ind->lightON(500);
                l_ind->lightOFF();
                break;
            
            case Indicator::HAZARD:
                l_ind->lightON(500);
                r_ind->lightON(500);
                break;

            default:
                l_ind->lightOFF();
                r_ind->lightOFF();
        }
        
    }   
}
