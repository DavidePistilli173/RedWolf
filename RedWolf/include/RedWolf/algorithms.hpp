#ifndef RW_ALGORITHMS_HPP
#define RW_ALGORITHMS_HPP

#include "RedWolf/core.hpp"

#include <limits>
#include <vector>

namespace rw
{
   /**
      \brief Perform a binary search for a value in a given container.
      Only works with containers of size at most std::numeric_limits<size_t>::max()/2;
      \param container Container to be searched.
      \param val Value to look for.
      \return Index of the required value, if present. Otherwise, the function returns 
      the index of the last element inferior to val.
   */
   template<typename Container, typename Value>
   constexpr size_t RW_API binary_search(
      const Container& container,
      const Value&     val) noexcept
   {
      if (container.empty()) return std::numeric_limits<size_t>::max();

      size_t start{ 0 };
      size_t end{ container.size() - 1 };

      size_t midpoint{ 0 };
      while (start <= end)
      {
         midpoint = (start + end) / 2;
         if (container[midpoint] == val) return midpoint;
         if (container[midpoint] < val)
            start = midpoint + 1;
         else
            end = midpoint - 1;
      }
      return midpoint;
   }

   // Template instantiations.
   template size_t RW_API binary_search<std::vector<size_t>, size_t>(
      const std::vector<size_t>& container,
      const size_t&              val) noexcept;
} // namespace rw

#endif
