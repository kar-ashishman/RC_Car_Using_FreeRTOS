#pragma once

#include "src/Includes/Task.hpp"
#include "src/Includes/queue.hpp"

//this class will consume data and read data from queue
class Consumer: public Task {
    public:
    Consumer(Queue<int>& consumer_queue);
    ~Consumer();

    private:
    Queue<int>& data_queue;
    void updateRoutine() override;
};