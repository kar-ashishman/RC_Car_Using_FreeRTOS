#include "src/Tasks/Includes/RemoteSignalTask.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Includes/Notify.hpp"
#include "src/Services/Includes/WifiDriver.hpp"
#include "src/Includes/GlobalTypes.hpp"

#define REMOTEIP                "192.168.1.7"
#define PORT                    5000

RemoteSignalTask::RemoteSignalTask(WifiDriver& wifiDriver,
                                   HornTask& hornTask, 
                                   IndicatorTask& indicatorTask
) : Task(
        "RemoteSignal", 
        2048, 
        1, 
        CoreId::COMMUNICATION_CORE,
        50
    ), wifiDriver_(wifiDriver), hornTask_(hornTask), indicatorTask_(indicatorTask) {}

RemoteSignalTask::~RemoteSignalTask() {
    if(taskHandle_ != nullptr) vTaskDelete(taskHandle_);
}

// This is a scheduled task in communication core at REMOTESIGNALTASKRATE ms
void RemoteSignalTask::updateRoutine() {
    TickType_t lastWakeTime = xTaskGetTickCount();  // vTaskDelayUntil( &xLastWakeTime, xFrequency );
    while(1) {
        // Look for wifiData, this needs an initialized wifidriver object
        // ensure connect method is called already
        int len = wifiDriver_.receiveUDP();
        if(len > 0) {
            // Type casting the received buffer to Signals enum
            Signals signal = static_cast<Signals>(wifiDriver_.getBuffer());

            // Return ack
            wifiDriver_.sendUDP(wifiDriver_.getBuffer(), REMOTEIP, (uint16_t)PORT);
            
            /******** NOTIFY TASKS BASED ON REMOTE SIGNALS *********/
            switch(signal) {
                //Handling horn commands
                case Signals::HORNOFF:
                case Signals::HORNON:
                case Signals::HORNDANCE:
                    hornTask_.updateParam(signal);
                    Notify::wakeTask(hornTask_);
                    break;

                //Handling indicator commands
                case Signals::IND_LEFT:
                case Signals::IND_RIGHT:
                case Signals::HAZARD:
                case Signals::IND_OFF:
                    indicatorTask_.updateParam(signal);
                    Notify::wakeTask(indicatorTask_);
                    break; 
            }
        }
        else
            vTaskDelayUntil(&lastWakeTime, taskRate_);
    }
}