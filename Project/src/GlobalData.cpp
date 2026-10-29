#include "src/Guards/Includes/Mutex.hpp"
#include "src/Guards/Includes/Semaphore.hpp"
#include "freertos/FreeRTOS.h"

Mutex serialMutex(portMAX_DELAY);
/*Create Semaphore with maxCount 2, and Initial Count 2*/
Semaphore serialSemaphore(2,2);

