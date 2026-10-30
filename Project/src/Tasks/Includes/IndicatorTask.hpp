#pragma once

#include "src/Includes/Task.hpp"

class IndicatorTask : public Task {
public:
    IndicatorTask();    
    ~IndicatorTask() override;

private:
    void updateRoutine() override;
};