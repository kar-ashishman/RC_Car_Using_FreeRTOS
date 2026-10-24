#include "src/Guards/Includes/Mutex.hpp"
#include "freertos/FreeRTOS.h"

Mutex serialMutex(portMAX_DELAY);