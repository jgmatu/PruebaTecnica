#pragma once
#include <memory>
#include <vector>
#include <cstdint>

// Forward declaration de la cola para no incluir todo el header aquí
template <typename T> class ThreadSafeQueue;

class IModule {
public:
    virtual ~IModule() = default;

    virtual void run() = 0;
    virtual void wait() = 0;
    virtual void stop() = 0;
};

// --- FUNCIONES FÁBRICA ---
// Estas funciones permiten crear los módulos sin que el main conozca las clases hijas.
std::unique_ptr<IModule> createRandomModule(ThreadSafeQueue<std::vector<uint8_t>>& inQueue, int size);
std::unique_ptr<IModule> createFilterModule(ThreadSafeQueue<std::vector<uint8_t>>& inQueue,
        ThreadSafeQueue<std::vector<uint8_t>>& outQueue);
std::unique_ptr<IModule> createLoggerModule(ThreadSafeQueue<std::vector<uint8_t>>& outQueue);
