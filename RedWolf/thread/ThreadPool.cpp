#include "ThreadPool.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::thread;

ThreadPool::ThreadPool(RedWolfManager& manager, u32 numThreads) : logger_{ manager.logger() }
{
   workers_.reserve(numThreads);
   for (size_t i = 0; i < numThreads; ++i)
   {
      workers_.emplace_back([this, i]() { workerMainLoop_(workers_[i]); });
   }

   logger_.trace("Thread pool created with {} threads.", numThreads);
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

size_t ThreadPool::numberOfThreads() const
{
   return workers_.size();
}

void ThreadPool::stopAllThreads()
{
   for (auto& worker : workers_)
   {
      worker.running = false;
      if (worker.thread.joinable())
      {
         worker.thread.join();
      }
   }
}

void ThreadPool::workerMainLoop_(Worker& thisWorker)
{
   std::function<void()> task;

   auto getNewTask = [this, &task, &thisWorker]() -> bool
   {
      bool pop = false;
      bool taskArrived{ true };

      std::unique_lock lck{ queueMtx_ };
      if (tasks_.empty())
      {
         taskArrived = workerConditionVariable_.wait_for(
            lck, wait_timeout, [this, &pop, &task, &thisWorker] { return !thisWorker.running || !tasks_.empty(); });
      }

      if (taskArrived && !tasks_.empty())
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