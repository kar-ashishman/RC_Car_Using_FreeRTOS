#pragma once

#include "src/Includes/Task.hpp"
#include "src/Tasks/Includes/IndicatorTask.hpp"

class IndicatorSchTask : public Task {
public:
    IndicatorSchTask();    
    ~IndicatorSchTask() override;
private:
    void updateRoutine() override;
    IndicatorTask *indicatorTask_ = nullptr;
};