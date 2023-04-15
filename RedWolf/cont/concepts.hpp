#ifndef RW_CONT_CONCEPTS_HPP
#define RW_CONT_CONCEPTS_HPP

#include <concepts>
#include <iterator>
#include <type_traits>

namespace rw::cont
{
   /**
    * @brief Concept for a contiguous container.
    * @tparam T Container type.
    */
   template<typename T>
   concept IsContiguousContainer = requires(T container) { std::contiguous_iterator<decltype(std::begin(container))>; };

   /**
    * @brief Concept for a contiguous container of a specific type.
    * @tparam T Container type.
    * @tparam U Stored type.
    */
   template<typename T, typename U>
   concept IsContiguousContainerOf = IsContiguousContainer<T> && requires(T list) { std::is_same_v<decltype(list[0]), U>; };
} // namespace rw::cont

#endif