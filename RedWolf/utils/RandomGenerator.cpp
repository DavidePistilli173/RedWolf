#include "RandomGenerator.hpp"

using namespace rw::utils;

RandomGenerator::RandomGenerator()
{
   int32_t now32 = static_cast<i32>(
      std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
   int64_t now64 =
      std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

   mt32_.value.seed(now32);
   mt64_.value.seed(now64);
}

void RandomGenerator::setSeed32(i32 newSeed)
{
   std::scoped_lock lck{ mt32_.mtx };
   mt32_.value.seed(newSeed);
}

void RandomGenerator::setSeed64(i64 newSeed)
{
   std::scoped_lock lck{ mt64_.mtx };
   mt64_.value.seed(newSeed);
}