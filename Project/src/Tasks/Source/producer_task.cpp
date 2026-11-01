#include "src/Tasks/Includes/producer_task.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/queue.hpp"

//Constructor of producer class 
//This file is responsible for producing data and adding it to the queue
Producer::Producer(Queue<int>& producer_queue):Task("Producer",
                        2048,
                        nullptr,
                        1,
                        nullptr,
                        CoreId::CONTROL_CORE,
                        1000),
                        data_queue{producer_queue}
                        {}

Producer::~Producer(){
    vTaskDelete(nullptr);
}

void Producer::updateRoutine(){
    static int data = 0;
    while(true){
        if(data_queue.QueueSendToFront(data++,100)){
            Serial.printf("[PRODUCER] Produced: %d\n", data);
        }
        else{
            Serial.printf("[PRODUCER] Queue is full. Failed to produce: %d\n", data);
        }
    }
}
