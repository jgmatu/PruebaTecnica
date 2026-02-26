#include <Actia/Filter.hpp>

#define EVEN_VALUE 0x00
#define ODD_VALUE 0x01

RandomFilter::RandomFilter()
{
    ;
}

bool RandomFilter::process(const std::vector<u_int8_t>& data)
{
    return !data.empty() && (data.back() & 0x01) == EVEN_VALUE;
}

// Takes references to both input and output queues
FilterExecutor::FilterExecutor(ThreadSafeQueue<std::vector<uint8_t>>& inQueue,
                        ThreadSafeQueue<std::vector<uint8_t>>& outQueue) :
    _inQueue(inQueue),
    _outQueue(outQueue),
    _filter()
{
    ;
}

void FilterExecutor::run()
{
    _workerThread = std::thread([this]() {
        std::cout << "[Filter] Asynchronous processing started." << std::endl;
        
        for (;;)
        {
            auto item = _inQueue.pop();

            // 1. Safety check for shutdown signal
            if (!item.has_value()) {
                std::cout << "[Filter] Shutdown signal received. Closing output queue." << std::endl;
                _outQueue.shutdown(); // Propagate shutdown to the Logger
                break;
            }

            auto& vec = item.value();

            // 2. Filter logic: check if the LAST element is EVEN (par) using bitmask
            if (_filter.process(vec))
            {
                _outQueue.push(std::move(vec));
            }
        }
    });
}
// Ensures the thread finishes before we access data
void FilterExecutor::wait()
{
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

// Destructor: important to prevent crashes if you forget to join
FilterExecutor::~FilterExecutor()
{
    wait();
}