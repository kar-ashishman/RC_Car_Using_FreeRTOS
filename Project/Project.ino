#include "src/Tasks/Includes/MotorTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "src/Tasks/Includes/HornTask.hpp"
#include "src/Tasks/Includes/IndicatorTask.hpp"
#include "src/Tasks/Includes/SemTestTask1.hpp"
#include "src/Tasks/Includes/SemTestTask2.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setup() {
    Serial.begin(115200);
    MotorTask motorTask;
    configASSERT(motorTask.taskCreate());
    while(true);
}

void loop() {
}
