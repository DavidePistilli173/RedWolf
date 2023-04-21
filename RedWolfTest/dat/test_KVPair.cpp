#include <RedWolf/dat/KVPair.hpp>
#include <gtest/gtest.h>

using namespace testing;

TEST(RwDatKvpair, Constructors)
{
   constexpr rw::dat::KVPair<std::string_view, int> p1{};
   EXPECT_EQ(p1.key, "");
   EXPECT_EQ(p1.value, 0);

   constexpr rw::dat::KVPair<std::string_view, int> p2{ "Test", 5 };
   EXPECT_EQ(p2.key, "Test");
   EXPECT_EQ(p2.value, 5);
}

TEST(RwDatKvpair, Operators)
{
   constexpr rw::dat::KVPair<std::string_view, int> p1{ "A", 1 };
   constexpr rw::dat::KVPair<std::string_view, int> p2{ "B", -1 };
   constexpr rw::dat::KVPair<std::string_view, int> p3{ "A", 3 };
   constexpr rw::dat::KVPair<std::string_view, int> p4{ "C", 1 };

   EXPECT_EQ(p1 == p1, true);
   EXPECT_EQ(p1 == p2, false);
   EXPECT_EQ(p1 == p3, true);
   EXPECT_EQ(p1 == p4, false);
   EXPECT_EQ(p2 == p1, false);
   EXPECT_EQ(p2 == p2, true);
   EXPECT_EQ(p2 == p3, false);
   EXPECT_EQ(p2 == p4, false);
   EXPECT_EQ(p3 == p1, true);
   EXPECT_EQ(p3 == p2, false);
   EXPECT_EQ(p3 == p3, true);
   EXPECT_EQ(p3 == p4, false);
   EXPECT_EQ(p4 == p1, false);
   EXPECT_EQ(p4 == p2, false);
   EXPECT_EQ(p4 == p3, false);
   EXPECT_EQ(p4 == p4, true);

   EXPECT_EQ(p1 != p1, false);
   EXPECT_EQ(p1 != p2, true);
   EXPECT_EQ(p1 != p3, false);
   EXPECT_EQ(p1 != p4, true);
   EXPECT_EQ(p2 != p1, true);
   EXPECT_EQ(p2 != p2, false);
   EXPECT_EQ(p2 != p3, true);
   EXPECT_EQ(p2 != p4, true);
   EXPECT_EQ(p3 != p1, false);
   EXPECT_EQ(p3 != p2, true);
   EXPECT_EQ(p3 != p3, false);
   EXPECT_EQ(p3 != p4, true);
   EXPECT_EQ(p4 != p1, true);
   EXPECT_EQ(p4 != p2, true);
   EXPECT_EQ(p4 != p3, true);
   EXPECT_EQ(p4 != p4, false);

   EXPECT_EQ(p1 <= p1, true);
   EXPECT_EQ(p1 <= p2, true);
   EXPECT_EQ(p1 <= p3, true);
   EXPECT_EQ(p1 <= p4, true);
   EXPECT_EQ(p2 <= p1, false);
   EXPECT_EQ(p2 <= p2, true);
   EXPECT_EQ(p2 <= p3, false);
   EXPECT_EQ(p2 <= p4, true);
   EXPECT_EQ(p3 <= p1, true);
   EXPECT_EQ(p3 <= p2, true);
   EXPECT_EQ(p3 <= p3, true);
   EXPECT_EQ(p3 <= p4, true);
   EXPECT_EQ(p4 <= p1, false);
   EXPECT_EQ(p4 <= p2, false);
   EXPECT_EQ(p4 <= p3, false);
   EXPECT_EQ(p4 <= p4, true);

   EXPECT_EQ(p1 < p1, false);
   EXPECT_EQ(p1 < p2, true);
   EXPECT_EQ(p1 < p3, false);
   EXPECT_EQ(p1 < p4, true);
   EXPECT_EQ(p2 < p1, false);
   EXPECT_EQ(p2 < p2, false);
   EXPECT_EQ(p2 < p3, false);
   EXPECT_EQ(p2 < p4, true);
   EXPECT_EQ(p3 < p1, false);
   EXPECT_EQ(p3 < p2, true);
   EXPECT_EQ(p3 < p3, false);
   EXPECT_EQ(p3 < p4, true);
   EXPECT_EQ(p4 < p1, false);
   EXPECT_EQ(p4 < p2, false);
   EXPECT_EQ(p4 < p3, false);
   EXPECT_EQ(p4 < p4, false);

   EXPECT_EQ(p1 > p1, false);
   EXPECT_EQ(p1 > p2, false);
   EXPECT_EQ(p1 > p3, false);
   EXPECT_EQ(p1 > p4, false);
   EXPECT_EQ(p2 > p1, true);
   EXPECT_EQ(p2 > p2, false);
   EXPECT_EQ(p2 > p3, true);
   EXPECT_EQ(p2 > p4, false);
   EXPECT_EQ(p3 > p1, false);
   EXPECT_EQ(p3 > p2, false);
   EXPECT_EQ(p3 > p3, false);
   EXPECT_EQ(p3 > p4, false);
   EXPECT_EQ(p4 > p1, true);
   EXPECT_EQ(p4 > p2, true);
   EXPECT_EQ(p4 > p3, true);
   EXPECT_EQ(p4 > p4, false);

   EXPECT_EQ(p1 >= p1, true);
   EXPECT_EQ(p1 >= p2, false);
   EXPECT_EQ(p1 >= p3, true);
   EXPECT_EQ(p1 >= p4, false);
   EXPECT_EQ(p2 >= p1, true);
   EXPECT_EQ(p2 >= p2, true);
   EXPECT_EQ(p2 >= p3, true);
   EXPECT_EQ(p2 >= p4, false);
   EXPECT_EQ(p3 >= p1, true);
   EXPECT_EQ(p3 >= p2, false);
   EXPECT_EQ(p3 >= p3, true);
   EXPECT_EQ(p3 >= p4, false);
   EXPECT_EQ(p4 >= p1, true);
   EXPECT_EQ(p4 >= p2, true);
   EXPECT_EQ(p4 >= p3, true);
   EXPECT_EQ(p4 >= p4, true);
}