//
// Created by Beyond on 25-8-4.
//

#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads):isStop(false){
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([this]() {
            while (isStop==false) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    this->cv.wait(lock,[this]() {
                        return isStop || !this->tasks.empty();
                    });
                    if ( isStop && this->tasks.empty()) {
                        return;
                    }
                    task = std::move(this->tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}

