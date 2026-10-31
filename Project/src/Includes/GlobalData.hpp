#include "src/Guards/Includes/Mutex.hpp"
#include "src/Guards/Includes/Semaphore.hpp"
#include "src/Includes/GlobalTypes.hpp"
#include "src/Services/Includes/WifiDriver.hpp"

extern Mutex serialMutex;
extern Semaphore serialSemaphore;
extern Indicator indicatorStatus;
extern WifiDriver wifiDriver;