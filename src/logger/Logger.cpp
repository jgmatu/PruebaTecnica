#include <Actia/Logger.hpp>

Logger::Logger()
{
    _outputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try
    {
         _outputFile.open(LOG_FILE, std::ios::out | std::ios::trunc);
    }
    catch (const std::ofstream::failure& e)
    {
        std::cerr << "File I/O Error: " << LOG_FILE << " - " << e.what() << std::endl;
        throw; // Re-raise the exception to signal failure
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to open log file: " << e.what() << std::endl;
        throw; // Rethrow to prevent silent failure
    }
    catch(...)
    {
        std::cerr << "An unknown error occurred while opening the log file." << std::endl;
        throw; // Rethrow to prevent silent failure
    }
}

Logger::~Logger() {
    try
    {
        // MANUALLY CLOSE inside 'try' to catch errors during flushing
        _outputFile.close(); 
    } 
    catch (const std::ios_base::failure& e)
    {
        std::cerr << "File Error: " << e.what() << std::endl;
        
        // CLEANUP: If close() failed or was never reached, ensure 
        // the file handle is released. clear() resets error flags.
        _outputFile.clear(); 

        if (_outputFile.is_open())
        {
            // We call close() here again without exceptions enabled 
            // to avoid an infinite loop of exceptions.
            _outputFile.exceptions(std::ofstream::goodbit); 
            _outputFile.close();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred during Logger destruction." << std::endl;
    }
}

void Logger::print(const std::vector<uint8_t>& data)
{
    if (data.empty()) return;
    auto now = std::chrono::system_clock::now();

    writeToStream(std::cout, std::format("{:%F %T %Z}", now), data);
    writeToStream(_outputFile, std::format("{:%F %T %Z}", now), data);
    _outputFile.flush(); // Ensure data is written to disk
}

// HELPER: Handles the actual formatting logic for any stream
void Logger::writeToStream(std::ostream& os, const std::string& timeStr, const std::vector<uint8_t>& data)
{
    // Header with Hex and Dec (using static_cast to be safe with uint8_t)
    os << std::format("[LOG] [{}] Last: {:02X}, Last: {:02d}\n", 
                        timeStr, 
                        static_cast<int>(data.back()), 
                        static_cast<int>(data.back()));
    
    os << "Data: ";
    for (auto val : data) {
        os << std::format("{:02X} ", static_cast<int>(val));
    }
    
    os << "\n" << std::string(40, '-') << std::endl;
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
