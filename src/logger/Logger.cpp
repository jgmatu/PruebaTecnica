#include <Actia/Logger.hpp>
#include <iostream>
#include <format>

void Logger::print(const std::vector<uint8_t>& data)
{
    if (data.empty()) return;

    std::cout << std::format("[LOG] Even Vector Found! First: {:02X}, Last: {:02X}\n", 
                             data.front(), data.back());
    std::cout << "      Data: ";
    for (auto val : data) {
        std::cout << std::format("{:02X} ", val);
    }
    std::cout << "\n" << std::string(40, '-') << std::endl;
}

LoggerExecutor::LoggerExecutor(ThreadSafeQueue<std::vector<uint8_t>>& queue)
    : _queue(queue), _logger() {}

void LoggerExecutor::run() {
    _workerThread = std::thread([this]() {
        std::cout << "[Logger] Asynchronous logger started." << std::endl;
        
        for (;;) {
            auto item = _queue.pop();

            if (!item.has_value()) {
                std::cout << "[Logger] Pipeline closed. Exiting logger." << std::endl;
                break;
            }

            // Call the internal logger to handle the formatting/printing
            _logger.print(item.value());
        }
    });
}

void LoggerExecutor::wait() {
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

LoggerExecutor::~LoggerExecutor() {
    wait();
}