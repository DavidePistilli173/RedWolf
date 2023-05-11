#include "RandomGenerator.hpp"

#include <chrono>

using namespace rw::util;

RandomGenerator::RandomGenerator()
{
   i32 now32 = static_cast<i32>(
      std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
   i64 now64 = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

   auto mt32 = mt32_.lock();
   mt32.val.seed(now32);

   auto mt64 = mt64_.lock();
   mt64.val.seed(now64);
}

void RandomGenerator::setSeed32(i32 newSeed)
{
   auto mt32 = mt32_.lock();
   mt32.val.seed(newSeed);
}

void RandomGenerator::setSeed64(i64 newSeed)
{
   auto mt64 = mt64_.lock();
   mt64.val.seed(newSeed);
}