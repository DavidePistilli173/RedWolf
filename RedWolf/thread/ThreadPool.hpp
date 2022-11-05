#ifndef RW_THREAD_THREADPOOL_HPP
#define RW_THREAD_THREADPOOL_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/utils/Logger.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <thread>
#include <vector>

namespace rw
{
   class RedWolfManager;
}

namespace rw::thread
{
   /**
    * @brief Class for handling a pool of threads that can execute custom tasks and be controlled from other threads.
    */
   class RW_API ThreadPool
   {
   public:
      /**
       * @brief Worker of the thread pool.
       */
      struct Worker
      {
         /**
          * @brief Default constructor.
          */
         Worker() = default;

         /**
          * @brief Constructor that accepts a thread.
          */
         template<typename Function>
         explicit Worker(Function f) : thread{ f }
         {
         }

         /**
          * @brief Copy constructor. Workers are non-copyable.
          */
         Worker(const Worker&) = delete;

         /**
          * @brief Move constructor.
          */
         Worker(Worker&& other) noexcept : thread{ std::move(other.thread) }, running{ other.running.load() }, busy{ other.busy.load() } {};

         /**
          * @brief Copy assignment operator. Workers are non-copyable.
          */
         Worker& operator=(const Worker&) noexcept = delete;

         /**
          * @brief Move assignment operator.
          */
         Worker& operator=(Worker& other) noexcept
         {
            if (&other != this)
            {
               thread = std::move(other.thread);
               running = other.running.load();
               busy = other.busy.load();
            }
            return *this;
         };

         std::thread       thread;          /**< Actual thread object. */
         std::atomic<bool> running{ true }; /**< If true, the thread is active. */
         std::atomic<bool> busy{ false };   /**< If true, the thread is actually performing a task. Otherwise it is waiting. */
      };

      /**
       * @brief Timeout for threads waiting for tasks. Introduced to allow the program to quit cleanly.
       */
      static constexpr std::chrono::seconds wait_timeout{ 1 };

      /**
       * @brief Constructor.
       * @param manager RedWolf library manager.
       * @param numThreads Number of starting threads of the pool. By default, matches the system's hardware concurrency, or at least 4.
       */
      explicit ThreadPool(RedWolfManager& manager, unsigned int numThreads = std::max(4U, std::thread::hardware_concurrency()));

      /**
       * @brief Custom destructor for joining all threads before destruction.
       */
      ~ThreadPool();

      /**
       * @brief Add a task for the pool to execute.
       * @param func Function object that will be executed by the pool. The return value must be void.
       */
      void addTask(const std::function<void()>& func);

      /**
       * @brief Add a task for the pool to execute.
       * @param func Function object that will be executed by the pool. The return value must be void.
       */
      void addTask(std::function<void()>&& func);

      /**
       * @brief Add a task with a return value for the pool to execute.
       * @tparam T Type of the return value.
       * @param func Function object that will be executed by the pool.
       * @return Future return value.
       */
      template<typename T>
      std::future<T> addTask(const std::function<T()>& func)
      {
         std::packaged_task task{ func };
         std::future<T>     retVal{ task.get_future() };

         addTaskInternal_([task = std::move(task)]() { task(); });
         workerConditionVariable_.notify_one();

         return retVal;
      }

      /**
       * @brief Add a task with a return value for the pool to execute.
       * @tparam T Type of the return value.
       * @param func Function object that will be executed by the pool.
       * @return Future return value.
       */
      template<typename T>
      std::future<T> addTask(std::function<T()>&& func)
      {
         std::packaged_task task{ func };
         std::future<T>     retVal{ task.get_future() };

         addTaskInternal_([task = std::move(task)]() { task(); });
         workerConditionVariable_.notify_one();

         return retVal;
      }

      /**
       * @brief Clear all pending tasks.
       */
      void clearTasks();

      /**
       * @brief Get the current number of threads of the pool.
       * @return size_t Current number of threads of the pool.
       */
      size_t numberOfThreads() const;

   private:
      /**
       * @brief Internal thread-safe function for adding a task to the queue.
       * @tparam T Type of function object to add.
       * @param func Function object to add.
       */
      template<typename T>
      void addTaskInternal_(T&& func)
      {
         std::scoped_lock lck{ queueMtx_ };
         tasks_.emplace(std::forward<T>(func));
      }

      /**
       * @brief Main loop of the workers.
       * @param thisWorker Reference to the worker object associated with the worker.
       */
      void workerMainLoop_(Worker& thisWorker);

      rw::utils::Logger& logger_; /**< Logger instance. */

      std::vector<Worker>     workers_;                 /**< Workers of the thread pool.  */
      std::condition_variable workerConditionVariable_; /**< Used for waking up workers when there are tasks to perform. */

      std::mutex                        queueMtx_; /**< Mutex for protecting the tasks queue. */
      std::queue<std::function<void()>> tasks_;    /**< Queue containing pending tasks for the pool. */
   };
} // namespace rw::thread

#endif