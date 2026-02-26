#include <iostream>
#include <memory>

#include <Actia/Queue.hpp>
#include <Actia/IModule.hpp>

static const int SIZE = 50;

int main()
{
    ThreadSafeQueue<std::vector<u_int8_t>> raw_queue;
    ThreadSafeQueue<std::vector<u_int8_t>> filtered_queue;

    // Solo usamos IModule y sus fábricas
    std::vector<std::unique_ptr<IModule>> pipeline;
    
    pipeline.push_back(createRandomModule(raw_queue, SIZE));
    pipeline.push_back(createFilterModule(raw_queue, filtered_queue));
    pipeline.push_back(createLoggerModule(filtered_queue));

    // 2. Start the whole pipeline
    for (auto& module : pipeline) {
        module->run();
    }

    std::this_thread::sleep_for(std::chrono::seconds(100));

    // 3. Stop everithing.
    for (auto& module : pipeline) {
        module->stop();
    }

    // When you want to wait/delete them:
    pipeline.clear(); // This triggers the destructors (wait) automatically

    std::cout << "[Final] Pipeline has been stopped and cleaned up." << std::endl;
    return 0;
}