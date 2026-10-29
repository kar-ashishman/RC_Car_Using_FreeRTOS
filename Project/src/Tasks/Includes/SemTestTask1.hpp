#pragma once

#include "src/Includes/Task.hpp"

class SemTestTask1 : public Task {
public:
    SemTestTask1();    
    ~SemTestTask1() override;

private:
    void updateRoutine() override;
};