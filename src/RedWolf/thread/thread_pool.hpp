//
// Created by cjm036653 on 11/08/25.
//

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

namespace rw::thread {
    /**
     * @brief Class for handling a pool of threads that can execute custom tasks and be controlled from other threads.
     */
    class ThreadPool {
     public:
        /**
         * @brief Worker of the thread pool.
         */
        struct Worker {
            /**
             * @brief Default constructor.
             */
            Worker() = default;

            /**
             * @brief Constructor that accepts a function.
             * @param func Function that the worker will run.
             */
            template<typename Function>
            explicit Worker(Function func) : thread{ func } {}

            /**
             * @brief Destructor.
             */
            ~Worker() = default;

            /**
             * @brief Copy constructor. Workers are non-copyable.
             */
            Worker(const Worker&) = delete;

            /**
             * @brief Move constructor.
             */
            Worker(Worker&& other) noexcept;

            /**
             * @brief Copy assignment operator. Workers are non-copyable.
             */
            Worker& operator=(const Worker&) noexcept = delete;

            /**
             * @brief Move assignment operator.
             * @param other Worker that will be moved from.
             */
            Worker& operator=(Worker&& other) noexcept;

            std::thread       thread;          /**< Actual thread object. */
            std::atomic<bool> running{ true }; /**< If true, the thread is active. */
            std::atomic<bool> busy{ false };   /**< If true, the thread is actually performing a task. Otherwise, it is waiting. */
        };

        /**
         * @brief Timeout for threads waiting for tasks. Introduced to allow the program to quit cleanly.
         */
        static constexpr std::chrono::seconds wait_timeout{ 1 };

        /**
         * @brief Constructor.
         * @param num_threads Number of starting threads of the pool. By default, matches the system's hardware concurrency, or at least 4.
         */
        explicit ThreadPool(const uint32_t num_threads = std::max(4U, std::thread::hardware_concurrency()));

        /**
         * @brief Custom destructor for joining all threads before destruction.
         */
        ~ThreadPool();

        /**
         * @brief Copy constructor.
         */
        ThreadPool(const ThreadPool&) = delete;

        /**
         * @brief Move constructor.
         */
        ThreadPool(ThreadPool&&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        ThreadPool& operator=(const ThreadPool&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        ThreadPool& operator=(ThreadPool&&) = delete;

        /**
         * @brief Add a task for the pool to execute.
         * @param func Function object that will be executed by the pool. The return value must be void.
         */
        void add_task(std::function<void()>&& func);

        /**
         * @brief Add a task with a return value for the pool to execute.
         * @tparam T Type of the return value.
         * @param func Function object that will be executed by the pool.
         * @return Future return value.
         */
        template<typename T>
        [[nodiscard]] std::future<T> add_task(std::function<T()>&& func) {
            std::shared_ptr<std::packaged_task<T()>> task{ std::make_shared<std::packaged_task<T()>>(func) };
            std::future<T>                           retVal{ task->get_future() };

            add_task_internal_([task]() mutable { (*task)(); });
            worker_condition_variable_.notify_one();

            return retVal;
        }

        /**
         * @brief Get the current number of threads of the pool.
         * @return size_t Current number of threads of the pool.
         */
        [[nodiscard]] size_t number_of_threads() const;

        /**
         * @brief Stop all threads of the thread pool. This function is blocking.
         */
        void stop_all_threads();

     private:
        /**
         * @brief Internal thread-safe function for adding a task to the queue.
         * @tparam T Type of function object to add.
         * @param func Function object to add.
         */
        template<typename T>
        void add_task_internal_(T&& func) {
            std::scoped_lock lck{ queue_mtx_ };
            tasks_.emplace(std::forward<T>(func));
        }

        /**
         * @brief Main loop of the workers.
         * @param this_worker Reference to the worker object associated with the worker.
         */
        void worker_main_loop_(Worker& this_worker);

        std::vector<Worker>     workers_;                   /**< Workers of the thread pool.  */
        std::condition_variable worker_condition_variable_; /**< Used for waking up workers when there are tasks to perform. */

        std::mutex                        queue_mtx_; /**< Mutex for protecting the tasks queue. */
        std::queue<std::function<void()>> tasks_;     /**< Queue containing pending tasks for the pool. */
    };
} // namespace rw::thread

#endif // THREAD_POOL_HPP
