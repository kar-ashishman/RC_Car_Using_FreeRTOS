#pragma once

#include "src/Includes/Task.hpp"

class RemoteSyncTask:public Task {
public:
    RemoteSyncTask();
    ~RemoteSyncTask() override;

private:
    void updateRoutine() override;
};