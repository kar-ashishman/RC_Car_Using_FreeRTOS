#pragma once

#include "src/Includes/Task.hpp"
#include "src/Services/Includes/WifiDriver.hpp"
#include "src/Tasks/Includes/HornTask.hpp"
#include "src/Tasks/Includes/IndicatorTask.hpp"

class RemoteSignalTask : public Task {
public:
    RemoteSignalTask(WifiDriver& wifiDriver, HornTask& hornTask, IndicatorTask& indicatorTask);   
    ~RemoteSignalTask() override;

private:
    void updateRoutine() override;
    WifiDriver& wifiDriver_;
    HornTask& hornTask_;
    IndicatorTask& indicatorTask_;
};