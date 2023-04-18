#include <RedWolf/time/DateTime.hpp>
#include <gtest/gtest.h>
#include <iomanip>
#include <sstream>

TEST(RwTimeDateTime, Constructor)
{
   auto   now{ std::chrono::system_clock::now() };
   time_t tt = std::chrono::system_clock::to_time_t(now);
   tm     local_tm = *gmtime(&tt);

   rw::time::DateTime d1{ now };

   EXPECT_EQ(d1.year(), std::chrono::year(local_tm.tm_year + 1900));
   EXPECT_EQ(d1.month(), std::chrono::month(local_tm.tm_mon + 1));
   EXPECT_EQ(d1.day(), std::chrono::day(local_tm.tm_mday));
   EXPECT_EQ(d1.hour(), std::chrono::hours(local_tm.tm_hour));
   EXPECT_EQ(d1.minute(), std::chrono::minutes(local_tm.tm_min));
   EXPECT_EQ(d1.second(), std::chrono::seconds(local_tm.tm_sec));
}

TEST(RwTimeDateTime, Formatter)
{
   auto   now{ std::chrono::system_clock::now() };
   time_t tt = std::chrono::system_clock::to_time_t(now);
   tm     local_tm = *gmtime(&tt);

   rw::time::DateTime d1{ now };

   std::string       s1a{ std::format("{}", d1) };
   std::stringstream s1b;
   s1b << local_tm.tm_year + 1900 << '-' << std::setfill('0') << std::setw(2) << local_tm.tm_mon + 1 << '-' << std::setfill('0')
       << std::setw(2) << local_tm.tm_mday << '|' << std::setfill('0') << std::setw(2) << local_tm.tm_hour << ':' << std::setfill('0')
       << std::setw(2) << local_tm.tm_min << ':' << std::setfill('0') << std::setw(2) << local_tm.tm_sec;

   EXPECT_EQ(s1a, s1b.str());
}