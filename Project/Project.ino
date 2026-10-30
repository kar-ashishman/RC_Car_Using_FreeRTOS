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
    pinMode(2,OUTPUT);
    //MotorTask motorTask;
    //configASSERT(motorTask.taskCreate());
    //LightsTask lightsTask;
    //configASSERT(lightsTask.taskCreate());

    /*Semaphore Testing*/
    //SemTestTask1 s1;
    //configASSERT(s1.taskCreate());
    //SemTestTask2 s2;
    //configASSERT(s2.taskCreate());

    /*Horn Test*/
    //HornTask hornTask;
    //configASSERT(hornTask.taskCreate());

    /*Indicator Test*/
    IndicatorTask indicatorTask;
    configASSERT(indicatorTask.taskCreate());
}

void loop() {
    
}
