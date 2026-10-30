#include "src/Tasks/Includes/IndicatorTask.hpp"
#include "src/Services/Includes/IndicatorDriver.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include <memory>

IndicatorTask::IndicatorTask() : Task(
        "IndicatorTask", 
        2048, 
        nullptr, 
        1, 
        nullptr, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

IndicatorTask::~IndicatorTask() {
    vTaskDelete(nullptr);
}

void IndicatorTask::updateRoutine() {
    while(true)
    {   
        std::unique_ptr<IndicatorDriver> indicator = std::make_unique<IndicatorDriver>(2,2);
        switch(indicatorStatus)
        {
            case Indicator::LEFT:
                indicator->setLeftIndicatorON();
                vTaskDelay(500);
                indicator->setLeftIndicatorOFF();
                vTaskDelay(500);
                break;

            case Indicator::RIGHT:
                //std::unique_ptr<IndicatorDriver> indicator = std::make_unique<IndicatorDriver>(2,2);
                indicator->setRightIndicatorON();
                vTaskDelay(500);
                indicator->setRightIndicatorOFF();
                vTaskDelay(500);
                break;

            case Indicator::HAZARD:
                //std::unique_ptr<IndicatorDriver> indicator = std::make_unique<IndicatorDriver>(2,2);
                indicator->setLeftIndicatorON();
                vTaskDelay(500);
                indicator->setLeftIndicatorOFF();
                vTaskDelay(500);

                indicator->setRightIndicatorON();
                vTaskDelay(500);
                indicator->setRightIndicatorOFF();
                vTaskDelay(500);
                break;

            default:
                indicator->setLeftIndicatorOFF();
                indicator->setRightIndicatorOFF();

        }
        
    }   
}
