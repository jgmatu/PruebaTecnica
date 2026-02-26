#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <thread>
#include <format>

#include <Actia/Queue.hpp>

class RandomGenerator {

public:
    // Constructor seeds the engine with a hardware random device
    RandomGenerator();

    // Generates a vector of N integers between min and max
    std::vector<u_int8_t> generateVector(size_t size);

private:
    std::mt19937 _engine; // Mersenne Twister engine
};

class RandomExecutor {
private:
    std::thread _workerThread;
    ThreadSafeQueue<std::vector<u_int8_t>>& _inQueue;
    RandomGenerator _rng;

public:
    RandomExecutor(ThreadSafeQueue<std::vector<u_int8_t>>& inQueue);

    // Takes a reference to a vector and fills it asynchronously
    void run(size_t iterations, size_t size);

    // Ensures the thread finishes before we access data
    void wait();

    // Destructor: important to prevent crashes if you forget to join
    ~RandomExecutor();
};

#endif