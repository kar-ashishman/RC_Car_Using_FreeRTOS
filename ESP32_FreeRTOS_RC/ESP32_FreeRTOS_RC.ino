#include "src/Includes/MotorTask.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void application() {
    // Create a MotorTask instance
    MotorTask motorTask;
    configASSERT(motorTask.createTask());
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
