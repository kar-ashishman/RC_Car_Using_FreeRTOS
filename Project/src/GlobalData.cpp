#include "src/Guards/Includes/Mutex.hpp"
#include "src/Includes/Notify.hpp"
#include "src/Guards/Includes/Semaphore.hpp"
#include "freertos/FreeRTOS.h"
#include "src/Includes/GlobalTypes.hpp"
#include <stdint.h>

Mutex serialMutex;
/*Create Semaphore with maxCount 2, and Initial Count 2*/
Semaphore serialSemaphore(2,2);




