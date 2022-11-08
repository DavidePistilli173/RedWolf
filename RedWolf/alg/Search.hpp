#ifndef RW_ALG_SEARCH_HPP
#define RW_ALG_SEARCH_HPP

#include <algorithm>

namespace rw::alg
{
   /**
    * @brief Perform a binary search on a container.
    * @tparam Container Type of container.
    * @param container Sorted container.
    * @param value Value to look for.
    * @return Iterator to the found element, or end iterator if not found.
    */
   template<typename Container>
   Container::iterator binarySearch(const Container& container, typename Container::const_reference value)
   {
      auto element = std::lower_bound(
         container.begin(),
         container.end(),
         value,
         [](Container::const_reference firstVal, Container::const_reference secondVal) { return firstVal < secondVal; });
      if (!(element == container.end()) && (value == *element)) return element;
      return container.end();
   }

   /**
    * @brief Perform a binary search on a const container.
    * @tparam Container Type of container.
    * @param container Sorted container.
    * @param value Value to look for.
    * @return Iterator to the found element, or end iterator if not found.
    */
   template<typename Container>
   Container::const_iterator binarySearch(const Container& container, typename Container::const_reference value)
   {
      auto element = std::lower_bound(
         container.cbegin(),
         container.cend(),
         value,
         [](Container::const_reference firstVal, Container::const_reference secondVal) { return firstVal < secondVal; });
      if (!(element == container.cend()) && (value == *element)) return element;
      return container.cend();
   }
} // namespace rw::alg

#endif