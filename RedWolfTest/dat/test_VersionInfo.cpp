#include <RedWolf/dat/VersionInfo.hpp>
#include <gtest/gtest.h>

TEST(RwDatVersionInfo, Constructor)
{
   rw::dat::VersionInfo v1;
   EXPECT_EQ(v1.major, 0U);
   EXPECT_EQ(v1.minor, 0U);
   EXPECT_EQ(v1.patch, 0U);

   rw::dat::VersionInfo v2{ 2U, 3U, 4U };
   EXPECT_EQ(v2.major, 2U);
   EXPECT_EQ(v2.minor, 3U);
   EXPECT_EQ(v2.patch, 4U);
}