//
// Created by Beyond on 25-8-4.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <iostream>
#include <functional>
#include <vector>


class ThreadPool {
public:
    ThreadPool(size_t numThreads=std::thread::hardware_concurrency());
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    template<typename F,typename... Args>
    void enqueue(F&& f,Args&&... args) {
        auto task=std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.emplace(task);
        }
        cv.notify_one();
    }
private:
    std::mutex mutex;
    std::atomic_bool isStop;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
};



#endif //THREADPOOL_H