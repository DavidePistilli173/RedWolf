#include <RedWolf/cont/ConstexprMap.hpp>
#include <array>
#include <gtest/gtest.h>

using namespace testing;

TEST(RwContConstexprmap, IteratorConstructors)
{
   static constexpr std::array ar{ std::pair(1, 2), std::pair(3, 4), std::pair(5, 6), std::pair(7, 8), std::pair(9, 10) };

   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it1(&ar[0]);
   EXPECT_EQ((*it1).first, 1);
   EXPECT_EQ(it1->first, 1);
}

TEST(RwContConstexprmap, IteratorOperators)
{
   static constexpr std::array ar{ std::pair(1, 2), std::pair(3, 4), std::pair(5, 6), std::pair(7, 8), std::pair(9, 10) };
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it1(&ar[0]);
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it2(&ar[0]);
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it3(&ar[1]);
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it4(&ar[2]);

   EXPECT_EQ(it1 == it1, true);
   EXPECT_EQ(it1 == it2, true);
   EXPECT_EQ(it1 == it3, false);
   EXPECT_EQ(it1 == it4, false);
   EXPECT_EQ(it2 == it1, true);
   EXPECT_EQ(it2 == it2, true);
   EXPECT_EQ(it2 == it3, false);
   EXPECT_EQ(it2 == it4, false);
   EXPECT_EQ(it3 == it1, false);
   EXPECT_EQ(it3 == it2, false);
   EXPECT_EQ(it3 == it3, true);
   EXPECT_EQ(it3 == it4, false);
   EXPECT_EQ(it4 == it1, false);
   EXPECT_EQ(it4 == it2, false);
   EXPECT_EQ(it4 == it3, false);
   EXPECT_EQ(it4 == it4, true);

   EXPECT_EQ(it1 != it1, false);
   EXPECT_EQ(it1 != it2, false);
   EXPECT_EQ(it1 != it3, true);
   EXPECT_EQ(it1 != it4, true);
   EXPECT_EQ(it2 != it1, false);
   EXPECT_EQ(it2 != it2, false);
   EXPECT_EQ(it2 != it3, true);
   EXPECT_EQ(it2 != it4, true);
   EXPECT_EQ(it3 != it1, true);
   EXPECT_EQ(it3 != it2, true);
   EXPECT_EQ(it3 != it3, false);
   EXPECT_EQ(it3 != it4, true);
   EXPECT_EQ(it4 != it1, true);
   EXPECT_EQ(it4 != it2, true);
   EXPECT_EQ(it4 != it3, true);
   EXPECT_EQ(it4 != it4, false);

   EXPECT_EQ(it1 <= it1, true);
   EXPECT_EQ(it1 <= it2, true);
   EXPECT_EQ(it1 <= it3, true);
   EXPECT_EQ(it1 <= it4, true);
   EXPECT_EQ(it2 <= it1, true);
   EXPECT_EQ(it2 <= it2, true);
   EXPECT_EQ(it2 <= it3, true);
   EXPECT_EQ(it2 <= it4, true);
   EXPECT_EQ(it3 <= it1, false);
   EXPECT_EQ(it3 <= it2, false);
   EXPECT_EQ(it3 <= it3, true);
   EXPECT_EQ(it3 <= it4, true);
   EXPECT_EQ(it4 <= it1, false);
   EXPECT_EQ(it4 <= it2, false);
   EXPECT_EQ(it4 <= it3, false);
   EXPECT_EQ(it4 <= it4, true);

   EXPECT_EQ(it1 < it1, false);
   EXPECT_EQ(it1 < it2, false);
   EXPECT_EQ(it1 < it3, true);
   EXPECT_EQ(it1 < it4, true);
   EXPECT_EQ(it2 < it1, false);
   EXPECT_EQ(it2 < it2, false);
   EXPECT_EQ(it2 < it3, true);
   EXPECT_EQ(it2 < it4, true);
   EXPECT_EQ(it3 < it1, false);
   EXPECT_EQ(it3 < it2, false);
   EXPECT_EQ(it3 < it3, false);
   EXPECT_EQ(it3 < it4, true);
   EXPECT_EQ(it4 < it1, false);
   EXPECT_EQ(it4 < it2, false);
   EXPECT_EQ(it4 < it3, false);
   EXPECT_EQ(it4 < it4, false);

   EXPECT_EQ(it1 > it1, false);
   EXPECT_EQ(it1 > it2, false);
   EXPECT_EQ(it1 > it3, false);
   EXPECT_EQ(it1 > it4, false);
   EXPECT_EQ(it2 > it1, false);
   EXPECT_EQ(it2 > it2, false);
   EXPECT_EQ(it2 > it3, false);
   EXPECT_EQ(it2 > it4, false);
   EXPECT_EQ(it3 > it1, true);
   EXPECT_EQ(it3 > it2, true);
   EXPECT_EQ(it3 > it3, false);
   EXPECT_EQ(it3 > it4, false);
   EXPECT_EQ(it4 > it1, true);
   EXPECT_EQ(it4 > it2, true);
   EXPECT_EQ(it4 > it3, true);
   EXPECT_EQ(it4 > it4, false);

   EXPECT_EQ(it1 >= it1, true);
   EXPECT_EQ(it1 >= it2, true);
   EXPECT_EQ(it1 >= it3, false);
   EXPECT_EQ(it1 >= it4, false);
   EXPECT_EQ(it2 >= it1, true);
   EXPECT_EQ(it2 >= it2, true);
   EXPECT_EQ(it2 >= it3, false);
   EXPECT_EQ(it2 >= it4, false);
   EXPECT_EQ(it3 >= it1, true);
   EXPECT_EQ(it3 >= it2, true);
   EXPECT_EQ(it3 >= it3, true);
   EXPECT_EQ(it3 >= it4, false);
   EXPECT_EQ(it4 >= it1, true);
   EXPECT_EQ(it4 >= it2, true);
   EXPECT_EQ(it4 >= it3, true);
   EXPECT_EQ(it4 >= it4, true);

   EXPECT_EQ(it1[1], ar[1]);
   EXPECT_EQ(*it1, std::pair(1, 2));
   EXPECT_EQ(it1->first, 1);
   EXPECT_EQ(it1->second, 2);
}

TEST(RwContConstexprmap, IteratorMathOperators)
{
   static constexpr std::array ar{ std::pair(1, 2), std::pair(3, 4), std::pair(5, 6), std::pair(7, 8), std::pair(9, 10) };
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it1(&ar[1]);
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it2(&ar[1]);
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it3(&ar[2]);
   rw::cont::ConstexprMap<int, int, ar.size()>::Iterator it4(&ar[3]);

   EXPECT_EQ(*(it1 - 1), ar[0]);
   EXPECT_EQ(*(it2 - 1), ar[0]);
   EXPECT_EQ(*(it3 - 1), ar[1]);
   EXPECT_EQ(*(it4 - 1), ar[2]);

   EXPECT_EQ(it1 - it1, 0);
   EXPECT_EQ(it2 - it2, 0);
   EXPECT_EQ(it3 - it3, 0);
   EXPECT_EQ(it4 - it4, 0);

   EXPECT_EQ(*(it1 + 1), ar[2]);
   EXPECT_EQ(*(it2 + 1), ar[2]);
   EXPECT_EQ(*(it3 + 1), ar[3]);
   EXPECT_EQ(*(it4 + 1), ar[4]);

   EXPECT_EQ(*(1 + it1), ar[2]);
   EXPECT_EQ(*(1 + it2), ar[2]);
   EXPECT_EQ(*(1 + it3), ar[3]);
   EXPECT_EQ(*(1 + it4), ar[4]);

   EXPECT_EQ(*(it1 += 1), ar[2]);
   EXPECT_EQ(*(it2 += 1), ar[2]);
   EXPECT_EQ(*(it3 += 1), ar[3]);
   EXPECT_EQ(*(it4 += 1), ar[4]);

   EXPECT_EQ(*(it1 -= 1), ar[1]);
   EXPECT_EQ(*(it2 -= 1), ar[1]);
   EXPECT_EQ(*(it3 -= 1), ar[2]);
   EXPECT_EQ(*(it4 -= 1), ar[3]);

   EXPECT_EQ(*(it1++), ar[1]);
   EXPECT_EQ(*it1, ar[2]);
   EXPECT_EQ(*(it2++), ar[1]);
   EXPECT_EQ(*it2, ar[2]);
   EXPECT_EQ(*(it3++), ar[2]);
   EXPECT_EQ(*it3, ar[3]);
   EXPECT_EQ(*(it4++), ar[3]);
   EXPECT_EQ(*it4, ar[4]);

   EXPECT_EQ(*(it1--), ar[2]);
   EXPECT_EQ(*it1, ar[1]);
   EXPECT_EQ(*(it2--), ar[2]);
   EXPECT_EQ(*it2, ar[1]);
   EXPECT_EQ(*(it3--), ar[3]);
   EXPECT_EQ(*it3, ar[2]);
   EXPECT_EQ(*(it4--), ar[4]);
   EXPECT_EQ(*it4, ar[3]);

   EXPECT_EQ(*(++it1), ar[2]);
   EXPECT_EQ(*(++it2), ar[2]);
   EXPECT_EQ(*(++it3), ar[3]);
   EXPECT_EQ(*(++it4), ar[4]);

   EXPECT_EQ(*(--it1), ar[1]);
   EXPECT_EQ(*(--it2), ar[1]);
   EXPECT_EQ(*(--it3), ar[2]);
   EXPECT_EQ(*(--it4), ar[3]);
}

TEST(RwContConstexprmap, Constructors)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1;
   EXPECT_EQ(map1.size(), 0U);

   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map2{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };
   EXPECT_EQ(map2["A"], 1);
   EXPECT_EQ(map2["B"], -1);
   EXPECT_EQ(map2["C"], 24);
   EXPECT_EQ(map2["a"], 10);
   EXPECT_EQ(map2["D"], 12);

   auto cmpLambda = [](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) { return lhs.first < rhs.first; };
   rw::cont::ConstexprMap<std::pair<int, int>, int, 64, decltype(cmpLambda)> map3(
      { { { 1, 2 }, 1 }, { { 2, 1 }, -1 }, { { 3, 4 }, 24 }, { { -1, 27 }, 10 }, { { 6, -6 }, 12 } }, cmpLambda);
   EXPECT_EQ(map3[std::pair(1, 2)], 1);
   EXPECT_EQ(map3[std::pair(2, 1)], -1);
   EXPECT_EQ(map3[std::pair(3, 4)], 24);
   EXPECT_EQ(map3[std::pair(-1, 27)], 10);
   EXPECT_EQ(map3[std::pair(6, -6)], 12);
}

TEST(RwContConstexprmap, At)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };
   EXPECT_EQ(map1.at("A"), 1);
   EXPECT_EQ(map1.at("B"), -1);
   EXPECT_EQ(map1.at("C"), 24);
   EXPECT_EQ(map1.at("a"), 10);
   EXPECT_EQ(map1.at("D"), 12);
   EXPECT_THROW(std::ignore = map1.at("FAIL"), std::out_of_range);

   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map2{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };
   EXPECT_EQ(map2.at("A"), 1);
   EXPECT_EQ(map2.at("B"), -1);
   EXPECT_EQ(map2.at("C"), 24);
   EXPECT_EQ(map2.at("a"), 10);
   EXPECT_EQ(map2.at("D"), 12);
   EXPECT_THROW(std::ignore = map2.at("FAIL"), std::out_of_range);
}

TEST(RwContConstexprmap, Begin)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto b1 = map1.begin();
   EXPECT_EQ(b1->key, "A");
   EXPECT_EQ(b1->value, 1);
}

TEST(RwContConstexprmap, CBegin)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto b1 = map1.cbegin();
   EXPECT_EQ(b1->key, "A");
   EXPECT_EQ(b1->value, 1);
}

TEST(RwContConstexprmap, CEnd)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.cend() - 1;
   EXPECT_EQ(it1->key, "a");
   EXPECT_EQ(it1->value, 10);
}

TEST(RwContConstexprmap, Contains)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   EXPECT_EQ(map1.contains("A"), true);
   EXPECT_EQ(map1.contains("B"), true);
   EXPECT_EQ(map1.contains("C"), true);
   EXPECT_EQ(map1.contains("D"), true);
   EXPECT_EQ(map1.contains("a"), true);
   EXPECT_EQ(map1.contains("FAIL"), false);
}

TEST(RwContConstexprmap, Clear)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };
   map1.clear();

   EXPECT_EQ(map1.contains("A"), false);
   EXPECT_EQ(map1.contains("B"), false);
   EXPECT_EQ(map1.contains("C"), false);
   EXPECT_EQ(map1.contains("D"), false);
   EXPECT_EQ(map1.contains("a"), false);
}

TEST(RwContConstexprmap, CRBegin)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto b1 = map1.crbegin();
   EXPECT_EQ((*b1).key, "a");
   EXPECT_EQ((*b1).value, 10);
}

TEST(RwContConstexprmap, CREnd)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.crend() - 1;
   EXPECT_EQ((*it1).key, "A");
   EXPECT_EQ((*it1).value, 1);
}

TEST(RwContConstexprmap, Emplace)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto retval = map1.emplace("N", 22);
   EXPECT_GE(retval.first, map1.begin());
   EXPECT_LT(retval.first, map1.end());
   EXPECT_EQ(retval.second, true);
   EXPECT_EQ(map1["N"], 22);

   retval = map1.emplace("2", 2);
   EXPECT_GE(retval.first, map1.begin());
   EXPECT_LT(retval.first, map1.end());
   EXPECT_EQ(retval.second, true);
   EXPECT_EQ(map1["2"], 2);

   retval = map1.emplace("A", 99);
   EXPECT_EQ(retval.first, map1.begin() + 1);
   EXPECT_EQ(retval.second, false);
   EXPECT_EQ(map1["A"], 1);
}

TEST(RwContConstexprmap, Empty)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1;
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map2{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   EXPECT_EQ(map1.empty(), true);
   EXPECT_EQ(map2.empty(), false);
}

TEST(RwContConstexprmap, End)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.end() - 1;
   EXPECT_EQ(it1->key, "a");
   EXPECT_EQ(it1->value, 10);
}

TEST(RwContConstexprmap, Erase)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto elem = map1.erase("A");
   EXPECT_EQ(elem, 1);
   EXPECT_EQ(map1.size(), 4U);
   EXPECT_THROW(std::ignore = map1.at("A"), std::out_of_range);

   elem = map1.erase("A");
   EXPECT_EQ(elem, 0);
   EXPECT_EQ(map1.size(), 4U);
   EXPECT_THROW(std::ignore = map1.at("A"), std::out_of_range);
}

TEST(RwContConstexprmap, Find)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   EXPECT_EQ(map1.find("A"), map1.begin());
   EXPECT_EQ(map1.find("B"), map1.begin() + 1);
   EXPECT_EQ(map1.find("FAIL"), map1.end());

   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map2{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   EXPECT_EQ(map2.find("A"), map2.cbegin());
   EXPECT_EQ(map2.find("B"), map2.cbegin() + 1);
   EXPECT_EQ(map2.find("FAIL"), map2.cend());
}

TEST(RwContConstexprmap, Insert)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto retval = map1.insert(rw::dat::KVPair<std::string_view, int>("TEST", 2));
   EXPECT_NE(retval.first, map1.end());
   EXPECT_EQ(retval.second, true);
   EXPECT_EQ(map1.at("TEST"), 2);
   EXPECT_EQ(map1.size(), 6U);

   retval = map1.insert(rw::dat::KVPair<std::string_view, int>("TEST", 75));
   EXPECT_NE(retval.first, map1.end());
   EXPECT_EQ(retval.second, false);
   EXPECT_EQ(map1.at("TEST"), 2);
   EXPECT_EQ(map1.size(), 6U);
}

TEST(RwContConstexprmap, InsertOrAssign)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto retval = map1.insert_or_assign(rw::dat::KVPair<std::string_view, int>("TEST", 2));
   EXPECT_NE(retval.first, map1.end());
   EXPECT_EQ(retval.second, true);
   EXPECT_EQ(map1.at("TEST"), 2);
   EXPECT_EQ(map1.size(), 6U);

   retval = map1.insert_or_assign(rw::dat::KVPair<std::string_view, int>("TEST", 75));
   EXPECT_NE(retval.first, map1.end());
   EXPECT_EQ(retval.second, true);
   EXPECT_EQ(map1.at("TEST"), 75);
   EXPECT_EQ(map1.size(), 6U);
}

TEST(RwContConstexprmap, LowerBound)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.lower_bound("B");
   EXPECT_EQ(it1, map1.find("B"));

   auto it2 = map1.lower_bound("N");
   EXPECT_EQ(it2, map1.find("a"));

   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map2{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it3 = map2.lower_bound("B");
   EXPECT_EQ(it3, map2.find("B"));

   auto it4 = map2.lower_bound("N");
   EXPECT_EQ(it4, map2.find("a"));
}

TEST(RwContConstexprmap, MaxSize)
{
   constexpr rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   EXPECT_EQ(map1.max_size(), 66U);
}

TEST(RwContConstexprmap, RBegin)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.rbegin();
   EXPECT_EQ((*it1).key, "a");
   EXPECT_EQ((*it1).value, 10);
}

TEST(RwContConstexprmap, REnd)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.rend() - 1;
   EXPECT_EQ((*it1).key, "A");
   EXPECT_EQ((*it1).value, 1);
}

TEST(RwContConstexprmap, Size)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1;
   rw::cont::ConstexprMap<std::string_view, int, 64> map2{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };
   rw::cont::ConstexprMap<std::string_view, int, 2>  map3{ { "A", 1 }, { "B", -1 } };

   EXPECT_EQ(map1.size(), 0U);
   EXPECT_EQ(map2.size(), 5U);
   EXPECT_EQ(map3.size(), 2U);
}

TEST(RwContConstexprmap, UpperBound)
{
   rw::cont::ConstexprMap<std::string_view, int, 64> map1{ { "A", 1 }, { "B", -1 }, { "C", 24 }, { "a", 10 }, { "D", 12 } };

   auto it1 = map1.upper_bound("B");
   EXPECT_EQ(it1, map1.find("C"));

   auto it2 = map1.upper_bound("1");
   EXPECT_EQ(it2, map1.find("A"));
}