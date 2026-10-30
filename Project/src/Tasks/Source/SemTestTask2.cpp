#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/Task.hpp"
#include "src/Tasks/Includes/SemTestTask2.hpp"


SemTestTask2::SemTestTask2():Task(
        "SemtestTask2", 
        2048, 
        nullptr, 
        1, 
        nullptr, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

SemTestTask2::~SemTestTask2() {
    vTaskDelete(nullptr);
}

void SemTestTask2::updateRoutine() {
    while (true) {
        // Critical Section 
        {
            LockGuard<Semaphore> lock(serialSemaphore,0xFFFF);
            Serial.println("Semaphore test task 2 running");
        }  
        vTaskDelay(getTaskRate());
    }
}