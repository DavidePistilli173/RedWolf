#include <RedWolf/dat/LogMsg.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(RwDatLogMsg, Constructor)
{
   rw::dat::LogMsg m1{ rw::dat::LogMsg::Level::warning, std::source_location::current(), "TESTMSG" };

   EXPECT_EQ(m1.level, rw::dat::LogMsg::Level::warning);
   EXPECT_EQ(m1.message, "TESTMSG");

   std::filesystem::path p1{ m1.sourceLocation.file_name() };
   std::string           s1FileName{ p1.filename().string() };
   std::string           s1FileNameExp{ "test_LogMsg.cpp" };
   EXPECT_EQ(s1FileName, s1FileNameExp);
   EXPECT_EQ(m1.sourceLocation.line(), 7);
}