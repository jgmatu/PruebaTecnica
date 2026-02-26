#ifndef IMODULE_H
#define IMODULE_H

/**
 * @brief Interface for asynchronous modules.
 * Ensures all child modules implement basic lifecycle and thread control.
 */
class IModule {
public:
    // Virtual destructor is mandatory to prevent memory leaks in derived classes
    virtual ~IModule() = default;

    // Starts the internal logic/thread
    virtual void run() = 0;
    
    // Blocks the calling thread until the module completes its work
    virtual void wait() = 0;

    // Signals the module to cease operations
    virtual void stop() = 0;
};

#endif // IMODULE_H