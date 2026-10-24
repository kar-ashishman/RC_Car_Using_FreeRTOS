#pragma once

#include "src/Includes/Task.hpp"

class LightsTask : public Task {
public:
    LightsTask();    
    ~LightsTask() override;

private:
    void updateRoutine() override;
};