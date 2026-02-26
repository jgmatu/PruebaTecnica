#include <iostream>
#include <memory>

#include <Actia/Random.hpp>
#include <Actia/Queue.hpp>
#include <Actia/Filter.hpp>
#include <Actia/Logger.hpp>
#include <Actia/IModule.hpp>

static const int SIZE = 20;

int main()
{
    ThreadSafeQueue<std::vector<u_int8_t>> raw_queue;
    ThreadSafeQueue<std::vector<u_int8_t>> filtered_queue;

    // 1. Create the concrete objects
    auto rng = std::make_unique<RandomExecutor>(raw_queue, SIZE);
    auto filter = std::make_unique<FilterExecutor>(raw_queue, filtered_queue);
    auto logger = std::make_unique<LoggerExecutor>(filtered_queue);

    std::vector<std::unique_ptr<IModule>> pipeline;

    pipeline.push_back(std::move(rng));
    pipeline.push_back(std::move(filter));
    pipeline.push_back(std::move(logger));

    // 2. Start the whole pipeline
    for (auto& module : pipeline) {
        module->run();
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 3. Stop everithing.
    for (auto& module : pipeline) {
        module->stop();
    }

    // When you want to wait/delete them:
    pipeline.clear(); // This triggers the destructors (wait) automatically

    std::cout << "Pipeline has been stopped and cleaned up." << std::endl;
    return 0;
}