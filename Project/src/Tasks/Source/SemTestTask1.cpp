#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/Task.hpp"
#include "src/Tasks/Includes/SemTestTask1.hpp"


SemTestTask1::SemTestTask1():Task(
        "SemtestTask1", 
        2048, 
        nullptr, 
        1, 
        nullptr, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

SemTestTask1::~SemTestTask1() {
    vTaskDelete(nullptr);
}


void SemTestTask1::updateRoutine() {
    while (true) {
        // Critical Section
        {   
            LockGuard<Semaphore> lock(serialSemaphore);
            if(serialSemaphore.lock()){
                Serial.println("Semaphore test task 1 running");       
            }
            LockGuard<Semaphore> unlock(serialSemaphore);
        }
        vTaskDelay(getTaskRate());
    }
}