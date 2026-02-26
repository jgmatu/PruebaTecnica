#pragma once
#include <vector>
#include <cstdint>
#include <Actia/Queue.hpp>

class RandomFilter {
public:
    // Takes the input queue (with all data) and an output queue (for filtered data)
    RandomFilter();

    bool process(const std::vector<u_int8_t>& data);

private:
    ;
};

class FilterExecutor {
public:
    // Takes references to both input and output queues
    explicit FilterExecutor(ThreadSafeQueue<std::vector<uint8_t>>& inQueue, 
                            ThreadSafeQueue<std::vector<uint8_t>>& outQueue);

    // Destructor ensures the thread is joined safely
    ~FilterExecutor();

    // Starts the filtering process in a background thread
    void run();

    // Stops and joins the thread
    void wait();

private:
    ThreadSafeQueue<std::vector<uint8_t>>& _inQueue;
    ThreadSafeQueue<std::vector<uint8_t>>& _outQueue;

    RandomFilter _filter;
    std::thread _workerThread;
};