#pragma once

#include "src/Includes/Task.hpp"

class HornTask : public Task {
public:
    HornTask();    
    ~HornTask() override;

private:
    void updateRoutine() override;
};