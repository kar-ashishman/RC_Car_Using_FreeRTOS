#include "src/Tasks/Includes/RemoteSignalTask.hpp"
#include "src/Services/Includes/WifiDriver.hpp"
#include "src/Tasks/Includes/HornTask.hpp"

void setup() {
    Serial.begin(115200);
    // Initialize wifi
    WifiDriver wifiDriver;
    wifiDriver.connect();

    // Create Motor Control Task

    // Create Head Light Control Task

    // Create Horn Control Task
    HornTask hornTask;
    configASSERT(hornTask.taskCreate());

    //Create Indicator Control Task
    IndicatorTask indicatorTask;
    configASSERT(indicatorTask.taskCreate());

    // Create RemoteSignalTask
    RemoteSignalTask remoteSignalTask(wifiDriver, hornTask, indicatorTask);
    configASSERT(remoteSignalTask.taskCreate());    
    
    // Infinite Loop
    while(true) {
        //Serial.println("Bad Program");
    }
}

void loop() {
}
