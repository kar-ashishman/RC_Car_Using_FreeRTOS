#include "src/Tasks/Includes/MotorTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "src/Includes/GlobalData.hpp"
#include "src/Tasks/Includes/consumer_task.hpp"
#include "src/Tasks/Includes/producer_task.hpp"
#include "src/Includes/queue.hpp"

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 SETUP EXECUTED");
}


void loop() {
    MotorTask motorTask;
    UBaseType_t length = 1;
    Queue<int> common_queue(static_cast<UBaseType_t>(length));
    common_queue.create_queue();
    configASSERT(motorTask.taskCreate());
    LightsTask lightsTask;
    configASSERT(lightsTask.taskCreate());
    
    Producer producer_task(common_queue);
    configASSERT(producer_task.taskCreate());
    Consumer consumer_task(common_queue);
    configASSERT(consumer_task.taskCreate());
}
