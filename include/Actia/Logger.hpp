#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <vector>
#include <cstdint>
#include <thread>

#include <Actia/Queue.hpp>
#include <Actia/IModule.hpp>


class Logger {
public:
    Logger() { };
    virtual ~Logger() = default;

protected:
    void print(const std::vector<uint8_t>& data);
};

class LoggerExecutor final : public Logger, public IModule {

public:
    explicit LoggerExecutor(ThreadSafeQueue<std::vector<uint8_t>>& queue);
    ~LoggerExecutor();

    // Starts the logging thread
    void run();
    
    // Explicitly wait for the logger to finish processing
    void wait();

    void stop();

private:
    ThreadSafeQueue<std::vector<uint8_t>>& _outQueue;
    std::thread _workerThread;
};

#endif