#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <vector>
#include <cstdint>
#include <thread>
#include <Actia/Queue.hpp>

class Logger final {
public:
    void print(const std::vector<uint8_t>& data);
};

class LoggerExecutor final {
public:
    explicit LoggerExecutor(ThreadSafeQueue<std::vector<uint8_t>>& queue);
    ~LoggerExecutor();

    // Starts the logging thread
    void run();
    
    // Explicitly wait for the logger to finish processing
    void wait();

private:
    ThreadSafeQueue<std::vector<uint8_t>>& _queue;
    Logger _logger;
    std::thread _workerThread;
};

#endif