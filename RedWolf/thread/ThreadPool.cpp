#include "ThreadPool.hpp"

using namespace rw::thread;

ThreadPool::Worker::Worker(Worker&& other) noexcept :
   thread{ std::move(other.thread) }, running{ other.running.load() }, busy{ other.busy.load() } {};

ThreadPool::Worker& ThreadPool::Worker::operator=(Worker&& other) noexcept
{
   if (&other != this)
   {
      thread = std::move(other.thread);
      running = other.running.load();
      busy = other.busy.load();
   }
   return *this;
};

ThreadPool::ThreadPool(u32 numThreads)
{
   workers_.reserve(numThreads);
   for (size_t i = 0; i < numThreads; ++i)
   {
      workers_.emplace_back([this, i]() { workerMainLoop_(workers_[i]); });
   }
}

ThreadPool::~ThreadPool()
{
   stopAllThreads();
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
   }

   workerConditionVariable_.notify_all();
   for (auto& worker : workers_)
   {
      if (worker.thread.joinable()) worker.thread.join();
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