//
// Created by cjm036653 on 11/08/25.
//

#include "thread_pool.hpp"

rw::thread::ThreadPool::Worker::Worker(Worker&& other) noexcept :
    thread{ std::move(other.thread) }, running{ other.running.load() }, busy{ other.busy.load() } {};

rw::thread::ThreadPool::Worker& rw::thread::ThreadPool::Worker::operator=(Worker&& other) noexcept {
    if (&other != this) {
        thread = std::move(other.thread);
        running = other.running.load();
        busy = other.busy.load();
    }
    return *this;
};

rw::thread::ThreadPool::ThreadPool(const uint32_t num_threads) {
    workers_.reserve(num_threads);
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this, i]() { worker_main_loop_(workers_[i]); });
    }
}

rw::thread::ThreadPool::~ThreadPool() {
    stop_all_threads();
}

void rw::thread::ThreadPool::add_task(std::function<void()>&& func) {
    add_task_internal_(func);
    worker_condition_variable_.notify_one();
}

size_t rw::thread::ThreadPool::number_of_threads() const {
    return workers_.size();
}

void rw::thread::ThreadPool::stop_all_threads() {
    for (auto& worker : workers_) {
        worker.running = false;
    }

    worker_condition_variable_.notify_all();
    for (auto& worker : workers_) {
        if (worker.thread.joinable()) worker.thread.join();
    }
}

void rw::thread::ThreadPool::worker_main_loop_(Worker& this_worker) {
    std::function<void()> task;

    auto get_new_task = [this, &task, &this_worker]() -> bool {
        bool pop = false;
        bool task_arrived{ true };

        std::unique_lock lck{ queue_mtx_ };
        if (tasks_.empty()) {
            task_arrived = worker_condition_variable_.wait_for(
                lck, wait_timeout, [this, &pop, &task, &this_worker] { return !this_worker.running || !tasks_.empty(); });
        }

        if (task_arrived && !tasks_.empty()) {
            pop = true;
            task = tasks_.front();
            tasks_.pop();
        }

        return pop;
    };

    while (this_worker.running) {
        if (get_new_task()) {
            this_worker.busy = true;
            task();
            this_worker.busy = false;
        }
    }
}