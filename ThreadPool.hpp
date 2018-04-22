#ifndef HTTP_SERVER_THREADPOOL_HPP
#define HTTP_SERVER_THREADPOOL_HPP

#include <atomic>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Config.hpp"

typedef std::function<void()> thread_task_t;
typedef std::function<void()>& thread_task_ref_t;
typedef std::unique_lock<std::mutex> thread_lock_t;

const std::string THREAD_POOL_INIT_ERROR = "ThreadPool.on_start() error initializing a thread: ";

class ThreadPool {
    private:
        std::atomic_bool _stop_flag;
        std::mutex _task_lock;

        std::vector<std::thread> _thread_pool;
        std::vector<thread_task_t> _scheduled_tasks;

        void on_start(const size_t& threads_count);
        void on_dispose();

        bool poll_task(thread_task_ref_t task);

        void thread_executor();

    public:
        ThreadPool(const size_t& threads_count = config::MINIMAL_THREADS_COUNT);
        ~ThreadPool();

        void offer_task(const thread_task_ref_t task);
};

#endif
