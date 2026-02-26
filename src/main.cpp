#include <iostream>

#include <Actia/Random.hpp>
#include <Actia/Queue.hpp>
#include <Actia/Filter.hpp>
#include <Actia/Logger.hpp>

static const int ITERATIONS = 1000;
static const int SIZE = 20;

int main()
{
    ThreadSafeQueue<std::vector<u_int8_t>> raw_queue;
    ThreadSafeQueue<std::vector<u_int8_t>> filtered_queue;

    RandomExecutor execRng(raw_queue);
    FilterExecutor execFilter(raw_queue, filtered_queue);
    LoggerExecutor logger(filtered_queue);

    execRng.run(ITERATIONS, SIZE);
    execFilter.run();
    logger.run();

    return 0;
}