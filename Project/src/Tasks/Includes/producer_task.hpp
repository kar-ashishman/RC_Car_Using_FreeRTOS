#pragma once

#include "src/Includes/Task.hpp"
#include "src/Includes/queue.hpp"
//this class will produce data and add it into queue
class Producer: public Task {
    public:
    Producer(Queue<int>& producer_queue);
    ~Producer();

    private:
    Queue<int>& data_queue;
    void updateRoutine() override;
};