#pragma once

#include "src/Includes/Task.hpp"
#include "src/Includes/GlobalTypes.hpp"

class MotorTask : public Task {
public:
    MotorTask();    
    ~MotorTask() override;
    void updateSpeed(int8_t speed);

private:
    void updateRoutine() override;
    int8_t speed_ = 0;
};