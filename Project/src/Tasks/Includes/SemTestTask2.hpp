#pragma once

#include "src/Includes/Task.hpp"

class SemTestTask2 : public Task {
public:
    SemTestTask2();    
    ~SemTestTask2() override;

private:
    void updateRoutine() override;
};