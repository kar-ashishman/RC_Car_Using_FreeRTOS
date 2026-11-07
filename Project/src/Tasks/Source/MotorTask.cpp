#include "src/Tasks/Includes/MotorTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "src/Includes/Notify.hpp"
#include "Arduino.h"
#include "src/Guards/Includes/LockGuard.hpp"
#include "src/Includes/GlobalData.hpp"
#include "src/Services/Includes/MotorDriver.hpp"

MotorTask::MotorTask() : Task(
        "MotorTask", 
        2048, 
        1, 
        CoreId::CONTROL_CORE,
        1000
    ) {}

MotorTask::~MotorTask() {
    vTaskDelete(nullptr);
}

void MotorTask::updateSpeed(int8_t speed) {
    speed_ = speed;
}

void MotorTask::updateRoutine(){

    MotorDriver frontLeftMotor(12, 13, 1, 1000); // Initializing Motor 1
    MotorDriver frontRightMotor(14, 27, 1, 1000); // Initializing Motor 2
    MotorDriver rearLeftMotor(25, 26, 1, 1000); // Initializing Motor 3
    MotorDriver rearRightMotor(32, 33, 1, 1000); // Initializing Motor 4
    while(1){

        Notify::waitForNotification();
        switch(signal_){
            
            case Signals::FORWARD:
                frontLeftMotor.motorControl(speed_, Direction::FORWARD);
                frontRightMotor.motorControl(speed_, Direction::FORWARD);
                rearLeftMotor.motorControl(speed_, Direction::FORWARD);
                rearRightMotor.motorControl(speed_, Direction::FORWARD);
                break;
            case Signals::REVERSE:
                frontLeftMotor.motorControl(speed_, Direction::BACKWARD);
                frontRightMotor.motorControl(speed_, Direction::BACKWARD);
                rearLeftMotor.motorControl(speed_, Direction::BACKWARD);
                rearRightMotor.motorControl(speed_, Direction::BACKWARD);
                break;
            case Signals::STEER_LEFT:
                frontLeftMotor.motorControl(speed_, Direction::FORWARD);
                frontRightMotor.motorControl(speed_, Direction::STOP);
                rearLeftMotor.motorControl(speed_, Direction::FORWARD);
                rearRightMotor.motorControl(speed_, Direction::STOP);
                break;
            case Signals::STEER_RIGHT:
                frontLeftMotor.motorControl(speed_, Direction::STOP);
                frontRightMotor.motorControl(speed_, Direction::FORWARD);
                rearLeftMotor.motorControl(speed_, Direction::STOP);
                rearRightMotor.motorControl(speed_, Direction::FORWARD);
                break;
            case Signals::STOP:
            default:
                frontLeftMotor.motorControl(0, Direction::STOP);
                frontRightMotor.motorControl(0, Direction::STOP);
                rearLeftMotor.motorControl(0, Direction::STOP);
                rearRightMotor.motorControl(0, Direction::STOP);
                break;
        }
        signal_ = Signals::NONE;
    }
    
}

/*
void MotorTask::updateRoutine() {
    LightsTask* lt = new LightsTask;
    vTaskDelay(3000);
    lt->taskCreate();
    vTaskDelay(3000);
    Notify::wakeTask(*lt);
    Serial.println("LightTask Notified Going to sleep");
    vTaskDelay(3000);
    Serial.println("Wakingup and going to delete light");
    delete lt;
    Serial.println("Deleting light");

    while (true) {
        // Critical Section
        {   
            //LockGuard<Mutex> lock(serialMutex);
            Serial.println("Motor control task running");
        }
        vTaskDelay(getTaskRate());
    }
}
*/