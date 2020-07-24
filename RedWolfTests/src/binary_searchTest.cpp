#pragma push_macro("max")
#undef max

#include <RedWolf/algorithms.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(BinarySearch, Empty)
{
   constexpr size_t    val{ 5 };
   std::vector<size_t> v;

   size_t idx{ rw::binary_search(v, val) };

   ASSERT_EQ(idx, std::numeric_limits<size_t>::max());
}

TEST(BinarySearch, OneVal)
{
   constexpr size_t    val{ 5 };
   std::vector<size_t> v{ 0 };

   size_t idx{ rw::binary_search(v, val) };
   ASSERT_EQ(idx, 0);

   v[0] = val;
   idx = rw::binary_search(v, val);
   ASSERT_EQ(idx, 0);
}

TEST(BinarySearch, TwoVals)
{
   constexpr size_t    val{ 5 };
   std::vector<size_t> v{ 0, 1 };

   size_t idx{ rw::binary_search(v, val) };
   ASSERT_EQ(idx, 1);

   v[1] = val;
   idx = rw::binary_search(v, val);
   ASSERT_EQ(idx, 1);

   v[0] = val;
   v[1] = val + 4;
   idx = rw::binary_search(v, val);
   ASSERT_EQ(idx, 0);
}

TEST(BinarySearch, Large)
{
   constexpr size_t    val{ 95 };
   std::vector<size_t> v{
      0, 2, 65, 77, 78, 79, 92, 95, 96, 99, 102, 1905
   };

   size_t idx{ rw::binary_search(v, val) };
   ASSERT_EQ(idx, 7);

   v[7] = 94;
   idx = rw::binary_search(v, val);
   ASSERT_EQ(idx, 7);
}

#pragma pop_macro("max")