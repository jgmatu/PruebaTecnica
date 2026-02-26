#include <Actia/Random.hpp>

RandomGenerator::RandomGenerator()
{
    std::random_device rd;

    _engine.seed(rd());
}

// Generates a vector of N integers between min and max
std::vector<u_int8_t> RandomGenerator::generateVector(size_t size)
{
    std::uniform_int_distribution<u_int8_t> dist(0, std::numeric_limits<uint8_t>::max());
    std::vector<u_int8_t> vec(size);

    // Fill vector using a lambda and the distribution
    std::generate(vec.begin(), vec.end(), [&]() {
        return dist(_engine);
    });

    return vec;
}

RandomExecutor::RandomExecutor(ThreadSafeQueue<std::vector<u_int8_t>>& inQueue, const size_t size) :
    _inQueue(inQueue),
    _size(size)
{
    _shutdown = false;
}

// Takes a reference to a vector and fills it asynchronously
void RandomExecutor::run()
{
    // Anonymous function (lambda) capturing by reference [&]
    _workerThread = std::thread([this]() {
        while (!_shutdown)
        {
            _inQueue.push(generateVector(_size));

            // Simulate latency in data input.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        _inQueue.shutdown();
    });
}

// Ensures the thread finishes before we access data

void RandomExecutor::wait()
{
    if (_workerThread.joinable()) {
        _workerThread.join();
    }
}

void RandomExecutor::stop()
{
    _shutdown = true;
}

// Destructor: important to prevent crashes if you forget to join
RandomExecutor::~RandomExecutor()
{
    wait();
    std::cout << "[Random] Shutdown signal received. Closing random task." << std::endl;
}

std::unique_ptr<IModule> createRandomModule(ThreadSafeQueue<std::vector<uint8_t>>& inQueue, int size) {
    return std::make_unique<RandomExecutor>(inQueue, size);
}