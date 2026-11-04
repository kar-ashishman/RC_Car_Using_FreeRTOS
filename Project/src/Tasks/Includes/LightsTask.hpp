#pragma once

#include "src/Includes/Task.hpp"

class LightsTask : public Task {
public:
    LightsTask();    
    ~LightsTask() override;

private:
    void updateRoutine() override;
    uint8_t pin_ = static_cast<uint8_t>(Pinout::HORN_PIN);
};