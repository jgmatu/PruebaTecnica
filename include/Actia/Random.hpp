#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <thread>
#include <format>

#include <Actia/Queue.hpp>
#include <Actia/IModule.hpp>

class RandomGenerator {

public:
    // Constructor seeds the engine with a hardware random device
    RandomGenerator();

    // Generates a vector of N integers between min and max
    std::vector<u_int8_t> generateVector(size_t size);

protected:
    std::mt19937 _engine; // Mersenne Twister engine
    bool _shutdown;
};
    
class RandomExecutor final : public RandomGenerator, public IModule {
private:
    std::thread _workerThread;
    ThreadSafeQueue<std::vector<u_int8_t>>& _inQueue;
    const size_t _size;

public:
    RandomExecutor(ThreadSafeQueue<std::vector<u_int8_t>>& inQueue, const size_t size);

    // Takes a reference to a vector and fills it asynchronously
    void run() override;

    // Ensures the thread finishes before we access data
    void wait() override;

    void stop() override;

    // Destructor: important to prevent crashes if you forget to join
    ~RandomExecutor();
};

#endif