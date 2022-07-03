#ifndef RW_TIME_CONCEPTS_HPP
#define RW_TIME_CONCEPTS_HPP

#include <chrono>
#include <concepts>
#include <filesystem>
#include <type_traits>

namespace rw::time
{
   /**
    * @brief Concept for representing an std::chrono clock.
    */
   template<typename T>
   concept IsChronoClock = std::is_same<T, std::chrono::system_clock>::value || std::is_same<T, std::chrono::steady_clock>::value ||
      std::is_same<T, std::chrono::high_resolution_clock>::value || std::is_same<T, std::filesystem::file_time_type::clock>::value ||
      std::is_same<T, std::chrono::file_clock>::value;

   /**
    * @brief Concept to represent a chrono duration.
    */
   template<typename T>
   concept IsChronoDuration = requires(T t)
   {
      std::chrono::duration_cast<std::chrono::microseconds>(t);
   };

   /**
    * @brief Concept to represent a type convertible to a chrono duration.
    */
   template<typename T>
   concept IsConvertibleToChronoDuration = requires(T t)
   {
      std::chrono::microseconds(t);
   };
} // namespace rw::time

#endif