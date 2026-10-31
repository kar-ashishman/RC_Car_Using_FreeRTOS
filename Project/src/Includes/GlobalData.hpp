#include "src/Guards/Includes/Mutex.hpp"
#include "src/Guards/Includes/Semaphore.hpp"
#include "src/Includes/GlobalTypes.hpp"

extern Mutex serialMutex;
extern Semaphore serialSemaphore;
extern Indicator indicatorStatus;
extern WifiDriver wifiDriver;