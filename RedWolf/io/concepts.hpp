#ifndef RW_IO_CONCEPTS_HPP
#define RW_IO_CONCEPTS_HPP

#include <concepts>
#include <format>
#include <type_traits>

namespace rw::io
{
   /**
    * @brief Concept for objects that can be used as std::format arguments.
    */
   template<typename T>
   concept IsFormattable = std::is_default_constructible_v<std::formatter<T>>;
} // namespace rw::io

#endif