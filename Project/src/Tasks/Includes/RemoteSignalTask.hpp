#pragma once

#include "src/Includes/Task.hpp"
#include "src/Services/Includes/WifiDriver.hpp"
#include "src/Tasks/Includes/HornTask.hpp"
#include "src/Tasks/Includes/LightsTask.hpp"
#include "src/Tasks/Includes/IndicatorTask.hpp"
#include "src/Tasks/Includes/RemoteSyncTask.hpp"

class RemoteSignalTask : public Task {
public:
    RemoteSignalTask(WifiDriver& wifiDriver, 
                    HornTask& hornTask, 
                    IndicatorTask& indicatorTask,
                    LightsTask& lightTask,
                    RemoteSyncTask& remoteSyncTask);   

    ~RemoteSignalTask() override;

private:
    void updateRoutine() override;
    WifiDriver& wifiDriver_;
    HornTask& hornTask_;
    IndicatorTask& indicatorTask_;
    LightsTask& lightTask_;
    RemoteSyncTask& remoteSyncTask_;
};