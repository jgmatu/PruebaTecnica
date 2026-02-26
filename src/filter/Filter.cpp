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
    _outQueue(outQueue)
{
    ;
}

void FilterExecutor::run()
{
    _workerThread = std::thread([this]() {
        std::cout << "[Filter] Asynchronous processing started." << std::endl;

        while (auto item = _inQueue.pop())
        {
            auto& vec = item.value();

            // Filter logic: check if the LAST element is EVEN (par) using bitmask
            if (process(vec))
            {
                _outQueue.push(std::move(vec));
            }
        }
        std::cout << "[Filter] Shutdown signal received. Closing output queue." << std::endl;

        // Propagate shutdown to the Logger and RNG
        _outQueue.shutdown();
        _inQueue.shutdown();
    });
}
// Ensures the thread finishes before we access data
void FilterExecutor::wait()
{
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

void FilterExecutor::stop()
{
    // Propagate shutdown to the Logger and RNG
    _outQueue.shutdown();
    _inQueue.shutdown();
}

// Destructor: important to prevent crashes if you forget to join
FilterExecutor::~FilterExecutor()
{
    wait();
}

// Definición de la función declarada en IModule.hpp
std::unique_ptr<IModule> createFilterModule(ThreadSafeQueue<std::vector<uint8_t>>& in, 
                                            ThreadSafeQueue<std::vector<uint8_t>>& out) {
    // Retornamos la clase concreta como un puntero a la interfaz
    return std::make_unique<FilterExecutor>(in, out);
}
