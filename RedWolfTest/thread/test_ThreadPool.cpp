#include <RedWolf/thread/ThreadPool.hpp>
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

TEST(RwThreadThreadPool, Constructor)
{
   rw::thread::ThreadPool p1{ 4U };
   EXPECT_EQ(p1.numberOfThreads(), 4U);
}

TEST(RwThreadThreadPool, AddVoidTask)
{
   rw::thread::ThreadPool p1{ 4U };

   std::atomic<int> i1{ 0 };
   std::atomic<int> i2{ 0 };
   std::atomic<int> i3{ 0 };
   std::atomic<int> i4{ 0 };

   p1.addTask(
      [&i1]()
      {
         while (i1 < 10)
         {
            ++i1;
         }
      });

   p1.addTask(
      [&i2]()
      {
         while (i2 < 10)
         {
            ++i2;
         }
      });

   p1.addTask(
      [&i3]()
      {
         while (i3 < 10)
         {
            ++i3;
         }
      });

   p1.addTask(
      [&i4]()
      {
         while (i4 < 10)
         {
            ++i4;
         }
      });

   std::this_thread::sleep_for(std::chrono::seconds(1));
   EXPECT_EQ(i1.load(), 10);
   EXPECT_EQ(i2.load(), 10);
   EXPECT_EQ(i3.load(), 10);
   EXPECT_EQ(i4.load(), 10);
}

TEST(RwThreadThreadPool, AddReturningTask)
{
   rw::thread::ThreadPool p1{ 4U };

   auto res1 = p1.addTask<int>(
      []()
      {
         int i{ 0 };
         while (i < 10)
         {
            ++i;
         }
         return i;
      });

   auto res2 = p1.addTask<int>(
      []()
      {
         int i{ 0 };
         while (i < 10)
         {
            ++i;
         }
         return i;
      });

   auto res3 = p1.addTask<int>(
      []()
      {
         int i{ 0 };
         while (i < 10)
         {
            ++i;
         }
         return i;
      });

   auto res4 = p1.addTask<int>(
      []()
      {
         int i{ 0 };
         while (i < 10)
         {
            ++i;
         }
         return i;
      });

   EXPECT_EQ(res1.get(), 10);
   EXPECT_EQ(res2.get(), 10);
   EXPECT_EQ(res3.get(), 10);
   EXPECT_EQ(res4.get(), 10);
}
