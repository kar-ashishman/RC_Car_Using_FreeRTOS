#pragma once
#include "src/Includes/Task.hpp"

class MotorTask : public Task {
public:
    MotorTask();    
    void update(void *parameters) override;
    ~MotorTask() override;
};