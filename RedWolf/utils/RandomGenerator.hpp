#ifndef RW_UTILS_RANDOMGENERATOR_HPP
#define RW_UTILS_RANDOMGENERATOR_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/MVPair.hpp"

#include <chrono>
#include <random>
#include <type_traits>

namespace rw::utils
{
   /**
    * @brief Class for generating random data.
    */
   class RW_API RandomGenerator
   {
   public:
      /**
       * @brief Default constructor. Initialise the engines with the current timestamp as seed.
       */
      RandomGenerator();

      /**
       * @brief Set a new seed for 32-bit engines.
       * @param newSeed New seed to set.
       */
      void setSeed32(i32 newSeed);

      /**
       * @brief Set a new seed for 64-bit engines.
       * @param newSeed New seed to set.
       */
      void setSeed64(i64 newSeed);

      /**
       * @brief Generate a single value from a uniform probability distribution.
       * @tparam T Type of the required random number.
       * @param min Minimum value of the desired range.
       * @param max Maximum value of the desired range.
       * @return Random value in the range [min, max] with uniform probability distribution.
       */
      template<typename T>
      T uniform(T min, T max)
      {
         if constexpr (std::is_integral<T>::value)
         {
            auto distribution = std::uniform_int_distribution(min, max);

            if constexpr (sizeof(T) <= 4U)
            {
               std::scoped_lock lck{ mt32_.mtx };
               return distribution(mt32_);
            }
            else
            {
               std::scoped_lock lck{ mt64_.mtx };
               return distribution(mt64_);
            }
         }
         else if constexpr (std::is_floating_point<T>::value)
         {
            auto distribution = std::uniform_real_distribution(min, max);

            if constexpr (sizeof(T) <= 4U)
            {
               std::scoped_lock lck{ mt32_.mtx };
               return distribution(mt32_);
            }
            else
            {
               std::scoped_lock lck{ mt64_.mtx };
               return distribution(mt64_);
            }
         }
         else
         {
            static_assert(true, "T must be either integral or floating point.");
         }
      }

      /**
       * @brief Generate a vector of random values from a uniform probability distribution.
       * @tparam T Type of the required random number.
       * @param min Minimum value of the desired range.
       * @param max Maximum value of the desired range.
       * @param size Size of the vector.
       * @return Vector of random values in the range [min, max] from a uniform probability distribution.
       */
      template<typename T>
      std::vector<T> uniform(T min, T max, size_t size)
      {
         std::vector<T> result;
         result.resize(size);

         if constexpr (std::is_integral<T>::value)
         {
            auto distribution = std::uniform_int_distribution(min, max);

            if constexpr (sizeof(T) <= 4U)
            {
               std::scoped_lock lck{ mt32_.mtx };

               for (auto& i : result)
               {
                  i = distribution(mt32_);
               }
            }
            else
            {
               std::scoped_lock lck{ mt64_.mtx };

               for (auto& i : result)
               {
                  i = distribution(mt64_);
               }
            }
         }
         else if constexpr (std::is_floating_point<T>::value)
         {
            auto distribution = std::uniform_real_distribution(min, max);

            if constexpr (sizeof(T) <= 4U)
            {
               std::scoped_lock lck{ mt32_.mtx };

               for (auto& i : result)
               {
                  i = distribution(mt32_);
               }
            }
            else
            {
               std::scoped_lock lck{ mt64_.mtx };

               for (auto& i : result)
               {
                  i = distribution(mt64_);
               }
            }
         }
         else
         {
            static_assert(true, "T must be either integral or floating point.");
         }

         return result;
      }

   private:
      rw::dat::MVPair<std::mt19937>    mt32_; /**< 32-bit Mersenne-Twister random engine. */
      rw::dat::MVPair<std::mt19937_64> mt64_; /**< 64-bit Mersenne-Twister random engine. */
   };
} // namespace rw::utils

#endif
