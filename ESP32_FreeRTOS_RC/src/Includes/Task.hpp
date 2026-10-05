#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "src/Includes/Enums.hpp"

/* FreeRTOS Task Abstraction -

A task pinned to a core of Target

BaseType_t xTaskCreatePinnedToCore(
    TaskFunction_t pvTaskCode,
    const char * const pcName,
    const uint32_t usStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t * const pxCreatedTask,
    const BaseType_t xCoreID
);
  
*/
class Task {
    public:
    Task(
        const char * const taskName_,
        const uint32_t stackDepth_,
        void * const parameters_,
        UBaseType_t priority_,
        TaskHandle_t * const taskHandle_,
        const CoreId coreId_,
        const uint16_t taskRate_);
    
    /* Each task has to manage resource clearning 
       Independently */
    virtual ~Task();
    
    /* Create a FreeRTOS task and return status 
       failed: Return false
       success: Return true */
    static bool createTask();

    /* Task Update Routine */
    virtual void update(void *parameters) = 0;

    protected:
    const char * const taskName_;
    const uint32_t stackDepth_;
    void * const parameters_;
    UBaseType_t priority_;
    TaskHandle_t * const taskHandle_;
    const CoreId coreId_;
    const uint16_t taskRate_;
};