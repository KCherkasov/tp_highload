#include <cstring>

#include "ThreadPool.hpp"

ThreadPool::ThreadPool(const size_t& threads_count): _stop_flag(false) {
    on_start(std::max((unsigned int)threads_count, std::thread::hardware_concurrency()));
}

ThreadPool::~ThreadPool() {
    _stop_flag = true;
    for (auto& thread : _thread_pool) {
        thread.join();
    }
}

void ThreadPool::offer_task(const thread_task_ref_t task) {
    thread_lock_t offer_lock(_task_lock);
    _scheduled_tasks.push(task);
}

void ThreadPool::thread_executor() {
    while(!_stop_flag) {
        thread_task_t scheduled_task;
        if (poll_task(scheduled_task)) {
            scheduled_task();
        } else {
            std::this_thread::yield();
        }
    }
}

bool ThreadPool::poll_task(thread_task_ref_t task) {
    thread_lock_t poll_lock(_task_lock);
    
    if (_scheduled_tasks.empty()) {
        return false;
    }
    
    task = std::move(_scheduled_tasks.front());
    _scheduled_tasks.pop();
    return true;
}

void ThreadPool::on_start(const size_t& threads_count) {
    try {
        for (size_t i = 0; i < threads_count; ++i) {
            _thread_pool.push_back(std::thread(
                &ThreadPool::thread_executor, this));
        }
    } catch (...) {
        std::runtime_error(THREAD_POOL_INIT_ERROR + std::string(strerror(errno)));
    }
}
