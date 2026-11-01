#ifndef HORNTASK_HPP
#define HORNTASK_HPP

#include "src/Includes/Task.hpp"
#include "src/Includes/GlobalTypes.hpp"

class HornTask : public Task {
public:
    HornTask();    
    ~HornTask() override;

private:
    void updateRoutine() override;
    uint8_t hornPin_ = static_cast<uint8_t>(Pinout::HORN_PIN);
};

#endif // HORNTASK_HPP