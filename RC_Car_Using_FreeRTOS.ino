#include "src/Includes/MotorTask.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "src/Includes/Mutex.hpp"

Mutex serialMonitorMutex(portMAX_DELAY);

void application() {
    // Create a MotorTask instance
    MotorTask motorTask;
    configASSERT(motorTask.taskCreate());
}

void setup() {
    Serial.begin(115200);
}

void fun(void *parameters) {
    while (true) {
        Serial.println("Hello from Task");
    }
}

void loop() {
    application();
    while(true);
}
