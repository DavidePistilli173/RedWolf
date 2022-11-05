#include "ThreadPool.hpp"

#include "RedWolf/RedWolfManager.hpp"

using namespace rw::thread;

ThreadPool::ThreadPool(RedWolfManager& manager, unsigned int numThreads) : logger_{ manager.logger() }
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

void ThreadPool::clearTasks()
{
   std::scoped_lock lck{ queueMtx_ };
   tasks_ = std::queue<std::function<void()>>();
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
         pop = workerConditionVariable_.wait_for(
            lck, wait_timeout, [this, &pop, &task, &thisWorker] { return !thisWorker.running || !tasks_.empty(); });
      }

      if (pop && !tasks_.empty())
      {
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