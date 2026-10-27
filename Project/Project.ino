#include "src/Tasks/Includes/MotorTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setup() {
    Serial.begin(115200);
}

// Application entry
void loop() {
    MotorTask motorTask;
    configASSERT(motorTask.taskCreate());
    LightsTask lightsTask;
    configASSERT(lightsTask.taskCreate());
}
