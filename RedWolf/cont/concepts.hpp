#ifndef RW_CONT_CONCEPTS_HPP
#define RW_CONT_CONCEPTS_HPP

#include <concepts>
#include <iterator>
#include <type_traits>

namespace rw::cont
{
   /**
    * @brief Concept for a list of contiguous const char*.
    */
   template<typename T>
   concept IsConstCharStarList = requires(T list)
   {
      std::is_same_v<decltype(list[0]), const char*>;
      std::contiguous_iterator<decltype(std::begin(list))>;
   };
} // namespace rw::cont

#endif