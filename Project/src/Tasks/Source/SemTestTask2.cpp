#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/Task.hpp"
#include "src/Tasks/Includes/SemTestTask2.hpp"


SemTestTask2::SemTestTask2():Task(
        "SemtestTask2", 
        2048, 
        nullptr, 
        2, 
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
            LockGuard<Semaphore> lock(serialSemaphore);
            if(serialSemaphore.lock()){
                Serial.println("Semaphore test task 2 running");
            } 
            LockGuard<Semaphore> unlock(serialSemaphore);
        }
        vTaskDelay(getTaskRate());
    }
}