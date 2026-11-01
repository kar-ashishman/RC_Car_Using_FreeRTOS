#include "src/Tasks/Includes/consumer_task.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/queue.hpp"

//Constructor of producer class 
//This file is responsible for consuming data from the queue and printing it to the serial monitor
Consumer::Consumer(Queue<int>& consumer_queue):Task("Consumer",
                        2048,
                        nullptr,
                        1,
                        nullptr,
                        CoreId::CONTROL_CORE,
                        1000),
                        data_queue{consumer_queue}
                        {}
Consumer::~Consumer(){
    vTaskDelete(nullptr);
}

void Consumer::updateRoutine(){
    int received_data = 0;
    while(true){
        //wait for data to be available in the queue and receive it
        BaseType_t status = data_queue.QueueReceive(
        received_data,
        portMAX_DELAY);

        if(status == pdPASS){
            Serial.printf("[CONSUMER] Consumed: %d\n", received_data);
        }
        else{
            Serial.printf("[CONSUMER] Queue is empty. Failed to consume data.\n");
        }
    }
}