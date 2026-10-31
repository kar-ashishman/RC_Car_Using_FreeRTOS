#include "src/Tasks/Includes/RemoteSignalTask.hpp"
#include "src/Services/Includes/WifiDriver.hpp"

void setup() {
    Serial.begin(115200);
    
    // Initialize wifi
    WifiDriver wifiDriver;
    wifiDriver.connect();

    // Create RemoteSignalTask
    RemoteSignalTask remoteSignalTask(wifiDriver);
    configASSERT(remoteSignalTask.taskCreate());

    


    
    
    
    // Infinite Loop
    while(true) {
        //Serial.println("Bad Program");
    }
}

void loop() {
}
