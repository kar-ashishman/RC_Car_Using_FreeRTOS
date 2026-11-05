#pragma once

#include "src/Includes/Task.hpp"

class IndicatorTask : public Task {
public:
    IndicatorTask();    
    ~IndicatorTask() override;
    uint8_t lPin_ = static_cast<uint8_t>(Pinout::HORN_PIN);
    uint8_t rPin_ = static_cast<uint8_t>(Pinout::HORN_PIN);

private:
    void updateRoutine() override;
    
};