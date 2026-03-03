#include <iostream>
#include <memory>
#include <thread>

#include <Actia/Queue.hpp>
#include <Actia/IModule.hpp>

int main()
{
    ThreadSafeQueue<std::vector<u_int8_t>> raw_queue;
    ThreadSafeQueue<std::vector<u_int8_t>> filtered_queue;

    // Solo usamos IModule y sus fábricas
    std::vector<std::unique_ptr<IModule>> pipeline;

    pipeline.push_back(createRandomModule(raw_queue));
    pipeline.push_back(createFilterModule(raw_queue, filtered_queue));
    pipeline.push_back(createLoggerModule(filtered_queue));

    // 2. Start the whole pipeline
    for (auto& module : pipeline) {
        module->run();
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 3. Stop everithing.
    for (auto& module : pipeline) {
        module->stop();
    }

    for (auto& module : pipeline) {
        module.reset(); // Safely deletes the object and sets pointer to nullptr
    }

    auto status = [](bool condition) { return condition ? "OK" : "FAIL"; };

    std::cout << "Raw queue Empty: " << status(raw_queue.size() == 0) << "\n";
    std::cout << "Filter queue Empty: " << status(filtered_queue.size() == 0) << "\n";
    std::cout << "[Final] Pipeline has been stopped and cleaned up." << std::endl;
    return 0;
}