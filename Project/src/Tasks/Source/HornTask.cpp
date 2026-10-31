#include "src/Tasks/Includes/HornTask.hpp"
#include "src/Services/Includes/HornDriver.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include <memory>

HornTask::HornTask() : Task(
        "HornTask", 
        2048, 
        nullptr, 
        1, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

HornTask::~HornTask() {
    vTaskDelete(nullptr);
}

void HornTask::updateRoutine() {
    while(true)
    {   
        std::unique_ptr<DancingHorn> horn = std::make_unique<DancingHorn>(2);
        horn->Dance();
    }   
}
