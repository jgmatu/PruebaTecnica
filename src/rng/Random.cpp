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

RandomExecutor::RandomExecutor(ThreadSafeQueue<std::vector<u_int8_t>>& inQueue) :
    _inQueue(inQueue)
{
    ;
}

// Takes a reference to a vector and fills it asynchronously
void RandomExecutor::run(size_t iterations, size_t size)
{
    // Anonymous function (lambda) capturing by reference [&]
    _workerThread = std::thread([this, iterations, size]() {
        for (size_t i = 0; i < iterations; ++i)
        {
            _inQueue.push(_rng.generateVector(size));
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

// Destructor: important to prevent crashes if you forget to join
RandomExecutor::~RandomExecutor()
{
    wait();
}