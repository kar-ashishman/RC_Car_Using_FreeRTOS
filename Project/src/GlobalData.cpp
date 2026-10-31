#include "src/Guards/Includes/Mutex.hpp"
#include "freertos/FreeRTOS.h"
#include "src/Services/Includes/WifiDriver.hpp"

Mutex serialMutex(portMAX_DELAY);
WifiDriver wifiDriver;

