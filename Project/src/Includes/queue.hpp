#include "freertos/FreeRTOS.h"
#include "src/Includes/GlobalTypes.hpp"
#include <stdint.h>

template <typename T>
class Queue{
    public:
    //constructor
    Queue( UBaseType_t queue_length):
    queue_length{queue_length},
    queue_handle{nullptr}
    {
    }
    //destructor
    virtual ~Queue();

    //create Queue 
    bool create_queue();

    //add elements to the front of queue
    BaseType_t QueueSendToFront(const void* ele,TickType_t Ticks);
    BaseType_t QueueSendToBack(const void* ele,TickType_t Ticks);

    //receive items from queue
    BaseType_t QueueReceive(void *Buffer, TickType_t Ticks);

    //peek from the Queue
    BaseType_t QueuePeek( void *Buffer,TickType_t Ticks);

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
BaseType_t Queue<T>::QueueSendToBack(const T& ele,TickType_t Ticks){
    return xQueueSendToBack( queue_handle,&ele,Ticks );
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
