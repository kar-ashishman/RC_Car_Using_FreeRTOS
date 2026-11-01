#pragma once

#include "src/Includes/Task.hpp"
#include "src/Services/Includes/WifiDriver.hpp"
#include "src/Tasks/Includes/HornTask.hpp"

class RemoteSignalTask : public Task {
public:
    RemoteSignalTask(WifiDriver& wifiDriver, HornTask& hornTask);   
    ~RemoteSignalTask() override;

private:
    void updateRoutine() override;
    WifiDriver& wifiDriver_;
    HornTask& hornTask_;
};