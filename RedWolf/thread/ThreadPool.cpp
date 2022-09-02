#include "ThreadPool.hpp"

namespace rw::thread
{
   std::unique_ptr<ThreadPool> ThreadPool::defaultPool_; /**< Default thread pool for RedWolf. */

   ThreadPool::ThreadPool(unsigned int numThreads) : logger_{ rw::utils::Logger::instance() }
   {
      workers_.reserve(numThreads);
      for (size_t i = 0; i < numThreads; ++i)
      {
         workers_.emplace_back([this, i]() { workerMainLoop_(workers_[i]); });
      }

      logger_->trace("Thread pool created with {} threads.", numThreads);
   }

   ThreadPool::~ThreadPool()
   {
      for (auto& worker : workers_)
      {
         worker.running = false;
      }

      workerConditionVariable_.notify_all();
      for (auto& worker : workers_)
      {
         if (worker.thread.joinable()) worker.thread.join();
      }
   }

   void ThreadPool::addTask(const std::function<void()>& func)
   {
      addTaskInternal_(func);
      workerConditionVariable_.notify_one();
   }

   void ThreadPool::addTask(std::function<void()>&& func)
   {
      addTaskInternal_(func);
      workerConditionVariable_.notify_one();
   }

   void ThreadPool::clearTasks()
   {
      std::scoped_lock lck{ queueMtx_ };
      tasks_ = std::queue<std::function<void()>>();
   }

   ThreadPool* ThreadPool::defaultPool()
   {
      if (defaultPool_ == nullptr)
      {
         defaultPool_.reset(new ThreadPool());
         defaultPool_->logger_->relInfo("Default thread pool created.");
      }

      return defaultPool_.get();
   }

   size_t ThreadPool::numberOfThreads() const
   {
      return workers_.size();
   }

   void ThreadPool::workerMainLoop_(Worker& thisWorker)
   {
      std::function<void()> task;

      auto getNewTask = [this, &task, &thisWorker]() -> bool
      {
         bool pop = false;

         std::unique_lock lck{ queueMtx_ };
         if (tasks_.empty())
         {
            workerConditionVariable_.wait(
               lck,
               [this, &pop, &task, &thisWorker]
               {
                  if (thisWorker.running && !tasks_.empty())
                  {
                     pop = true;
                     task = tasks_.front();
                     tasks_.pop();
                     return !thisWorker.running || pop;
                  }
                  return !thisWorker.running;
               });
         }
         else
         {
            pop = true;
            task = tasks_.front();
            tasks_.pop();
         }

         return pop;
      };

      while (thisWorker.running)
      {
         if (getNewTask())
         {
            thisWorker.busy = true;
            task();
            thisWorker.busy = false;
         }
      }
   }
} // namespace rw::thread