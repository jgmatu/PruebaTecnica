#include <Actia/Logger.hpp>
#include <iostream>
#include <format>

void Logger::print(const std::vector<uint8_t>& data)
{
    if (data.empty()) return;

    std::cout << std::format("[LOG] Even Vector Found! Last: {:02X}, Last: {:02d}\n", 
                             data.back(), data.back());
    std::cout << "      Data: ";
    for (auto val : data) {
        std::cout << std::format("{:02X} ", val);
    }
    std::cout << "\n" << std::string(40, '-') << std::endl;
}

LoggerExecutor::LoggerExecutor(ThreadSafeQueue<std::vector<uint8_t>>& queue)
    : _outQueue(queue) {}

void LoggerExecutor::run()
{
    _workerThread = std::thread([this]() {
        std::cout << "[Logger] Asynchronous logger started." << std::endl;

        while (auto item = _outQueue.pop()) {
            print(item.value());
        }
        _outQueue.shutdown();
    });
}

void LoggerExecutor::wait()
{
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

void LoggerExecutor::stop()
{
    _outQueue.shutdown();
}

LoggerExecutor::~LoggerExecutor() {
    wait();
    std::cout << "[Logger] Shutdown signal received. Closing logger task." << std::endl;
}

std::unique_ptr<IModule> createLoggerModule(ThreadSafeQueue<std::vector<uint8_t>>& outQueue) {
    return std::make_unique<LoggerExecutor>(outQueue);
}
