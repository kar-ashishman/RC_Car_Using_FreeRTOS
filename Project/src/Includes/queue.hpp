#ifndef PROJECT_QUEUE_HPP
#define PROJECT_QUEUE_HPP

#include "freertos/FreeRTOS.h"
#include "src/Includes/GlobalTypes.hpp"
#include <stdint.h>

/* Queue class:
1. Implements the FreeRTOS queue wrapper
2. This class can't be inherited 
*/
template <typename T>
class Queue final{
    public:
    static constexpr UBaseType_t MAX_QUEUE_SIZE = 10; 
    //constructor
    Queue( UBaseType_t length):
    queue_length{length},
    queue_handle{nullptr}
    {
        if(queue_length > MAX_QUEUE_SIZE){
            queue_length = MAX_QUEUE_SIZE;
        }
    }
    //destructor
    virtual ~Queue();

    //create Queue 
    bool create_queue();

    //add elements to the front of queue
    BaseType_t QueueSendToFront(const T& ele,
                                TickType_t Ticks);

    BaseType_t QueueSendToBack(const T& ele,
                                TickType_t Ticks);

    //receive items from queue
    BaseType_t QueueReceive(T& ele, 
                                TickType_t Ticks);

    //peek from the Queue
    BaseType_t QueuePeek( T& ele,
                                TickType_t Ticks);

    // Add elements to queue from ISR
    BaseType_t QueueSendToBackFromISR(
                        QueueHandle_t queue_handle,
                        const T&ele,
                        BaseType_t *pxHigherPriorityTaskWoken);
    BaseType_t QueueSendToFrontFromISR(
                        QueueHandle_t queue_handle,
                        const T&ele,
                        BaseType_t *pxHigherPriorityTaskWoken);

    protected:
    UBaseType_t  queue_length;
    QueueHandle_t queue_handle;
};

template <typename T>
bool Queue<T>::create_queue(){
    queue_handle = xQueueCreate(queue_length, sizeof(T));
    if(queue_handle!=nullptr){
        return true;
    }
    else{ return false;}
}

template <typename T>
BaseType_t Queue<T>::QueueSendToFront(const T& ele,TickType_t Ticks){
    return xQueueSendToFront( queue_handle,&ele,Ticks );
}

template <typename T>
BaseType_t Queue<T>::QueueSendToFrontFromISR(
    QueueHandle_t queue_handle,
    const T& ele,
    BaseType_t *pxHigherPriorityTaskWoken)
{
    BaseType_t status = xQueueSendToFrontFromISR(
        queue_handle,
        &ele,
        pxHigherPriorityTaskWoken
    );
    portYIELD_FROM_ISR(*pxHigherPriorityTaskWoken);
    return status;
}

template <typename T>
BaseType_t Queue<T>::QueueSendToBack(const T& ele,TickType_t Ticks){
    return xQueueSendToBack( queue_handle,&ele,Ticks );
}

template <typename T>
BaseType_t Queue<T>::QueueSendToBackFromISR(
    QueueHandle_t queue_handle,
    const T& ele,
    BaseType_t *pxHigherPriorityTaskWoken)
{
    BaseType_t status = xQueueSendToBackFromISR(
        queue_handle,
        &ele,
        pxHigherPriorityTaskWoken
    );
    portYIELD_FROM_ISR(*pxHigherPriorityTaskWoken);
    return status;
}

template <typename T>
BaseType_t Queue<T>::QueueReceive(T& ele,TickType_t Ticks){
    return xQueueReceive(queue_handle,&ele,Ticks );
}

template <typename T>
BaseType_t Queue<T>::QueuePeek(T& ele,TickType_t Ticks){
    return xQueuePeek(queue_handle,&ele,Ticks );
}

template <typename T>
Queue<T>::~Queue(){
    if(queue_handle!=nullptr) {
        vQueueDelete(queue_handle);
        queue_handle = nullptr;
    }
}

#endif //PROJECT_QUEUE_HPP
