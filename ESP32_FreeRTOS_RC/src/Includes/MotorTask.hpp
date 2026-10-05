#pragma once
#include "src/Includes/Task.hpp"

class MotorTask : public Task {
public:
    MotorTask();    
    ~MotorTask() override;

private:
    void updateRoutine() override;
};