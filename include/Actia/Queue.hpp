#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <iostream>

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cond;
    bool finished = false;

public:
    // Push data into the pipeline
    void push(T value)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(std::move(value));
        }
        cond.notify_one(); // Wake up the consumer
    }

    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait until there is data OR we are finished
        cond.wait(lock, [this] { return !queue.empty() || finished;  });

        // If the queue is empty AND finished is true, then stop.
        // If finished is true BUT the queue still has data, we should 
        // keep popping until it's actually empty.
        if (queue.empty() && finished) {
            return std::nullopt;
        }

        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    // Signal that no more data is coming
    void shutdown()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            finished = true;
        }
        cond.notify_all();
    }
};
#endif