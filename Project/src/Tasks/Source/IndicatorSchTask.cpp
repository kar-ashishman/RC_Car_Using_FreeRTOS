#include "src/Tasks/Includes/IndicatorSchTask.hpp"
#include "src/Services/Includes/LightDriver.hpp"
#include "src/Includes/Notify.hpp"

IndicatorSchTask::IndicatorSchTask() : Task(
        "IndicatorSchTask", 
        2048,  
        1,
        CoreId::CONTROL_CORE,
        0
    ) {}

IndicatorSchTask::~IndicatorSchTask() {
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

void IndicatorSchTask::updateRoutine() {
    while(true)
    {   
        Notify::waitForNotification();
        switch(signal_)
        {
            case Signals::IND_LEFT:
            case Signals::IND_RIGHT:
            case Signals::HAZARD:
                //Spawn Indicator Task Dynamically if not already created and notify it
                if(this->indicatorTask_ == nullptr) {
                    IndicatorTask *it = new IndicatorTask();
                    it->taskCreate();
                    this->indicatorTask_ = it;
                }
                Notify::wakeTask(*this->indicatorTask_);
                this->indicatorTask_->updateParam(signal_);
                break;

            case Signals::IND_OFF:
                //Turn off the indicators and delete the indicator task if it exists
                if(this->indicatorTask_ != nullptr){
                    LeftIndicator::lightOFF(this->indicatorTask_->lPin_);
                    RightIndicator::lightOFF(this->indicatorTask_->rPin_);
                    delete this->indicatorTask_;
                    this->indicatorTask_ = nullptr;
                }
                break;
        }
        signal_ = Signals::NONE;
    }   
}