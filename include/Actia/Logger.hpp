#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <vector>
#include <cstdint>
#include <thread>
#include <fstream>
#include <iostream>
#include <format>
#include <chrono>

#include <Actia/Queue.hpp>
#include <Actia/IModule.hpp>

#define LOG_FILE "log/log.txt"

class Logger {
public:
    Logger();
    virtual ~Logger();

protected:
    void print(const std::vector<uint8_t>& data);
    void writeToStream(std::ostream& os, const std::string& timeStr, const std::vector<uint8_t>& data);

    std::ofstream _outputFile;
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