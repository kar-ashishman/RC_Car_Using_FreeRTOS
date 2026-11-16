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
                                   IndicatorTask& indicatorTask,
                                   LightsTask& lightTask,
                                   RemoteSyncTask& remoteSyncTask,
                                   MotorTask& motorTask) 
       : Task("RemoteSignal",2048,1,CoreId::COMMUNICATION_CORE,50), 
       wifiDriver_(wifiDriver), 
       hornTask_(hornTask), 
       indicatorTask_(indicatorTask),
       lightTask_(lightTask),
       remoteSyncTask_(remoteSyncTask),
       motorTask_(motorTask){}

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
            
            //Converting the received buffer to uint16_t and swapping the bytes to get the correct signal
            uint16_t signal_swapped = (static_cast<uint16_t>(wifiDriver_.getBuffer()) >> 8)
                            | (static_cast<uint16_t>(wifiDriver_.getBuffer()) << 8);

            Signals byte_1 = static_cast<Signals>((signal_swapped >> 8) & 0xFF); // Extract the first byte
            int8_t byte_2 = signal_swapped & 0xFF;        // Extract the second byte

            // Return ack
            //wifiDriver_.sendUDP(wifiDriver_.getBuffer(), REMOTEIP, (uint16_t)PORT);
            Serial.println("Received Signal: " + String((signal_swapped >> 8) & 0xFF) + " with value: " + String(byte_2));
            
            Signals signal = static_cast<Signals>(signal_swapped);
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
                
                //Handling headlight commands
                case Signals::HEADLIGHTON:
                case Signals::HEADLIGHTOFF:
                    lightTask_.updateParam(signal);
                    Notify::wakeTask(lightTask_);
                    break;

                case Signals::SYNC:
                    remoteSyncTask_.updateParam(signal);
                    Notify::wakeTask(remoteSyncTask_);
                    break;
                case Signals::FORWARD:
                case Signals::REVERSE:
                case Signals::STOP:
                    motorTask_.updateParam(signal);
                    motorTask_.updateSpeed(byte_2);
                    Notify::wakeTask(motorTask_);
                    break;
            }
        }
        else
            vTaskDelayUntil(&lastWakeTime, taskRate_);
    }
}