// Testing Wifi connectivity and UDP communication

#include "src/Tasks/Includes/MotorTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "src/Includes/GlobalData.hpp"

void setup() {
    Serial.begin(115200);
    wifiDriver.connect();
}

void loop() {
    wifiDriver.receiveUDP();
}
