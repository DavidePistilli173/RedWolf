/** @file common.hpp Common macros and definitions. DO NOT EDIT MANUALLY. */

#ifndef RW_COMMON_HPP
#define RW_COMMON_HPP

// Defines
#ifndef GLFW_INCLUDE_VULKAN
    #define GLFW_INCLUDE_VULKAN /**< Used so that GLFW automatically includes Vulkan headers. */
#endif

namespace rw
{
   /**
      @var debug
      @brief true for debug configurations, false otherwise.
   */
#ifdef RW_DEBUG
   constexpr bool debug{ true };
#else
   constexpr bool debug{ false };
#endif

   /**
    * @var windows
    * @brief true in a Windows environment, false otherwise.
    */
#ifdef _WIN32
   constexpr bool windows{ true };

   #ifndef RW_API
       #ifdef RW_DLL
          #define RW_API __declspec(dllexport)
       #else
          #define RW_API __declspec(dllimport)
       #endif
    #endif
#else
   constexpr bool windows{ false };

   #ifndef RW_API
      #define RW_API
   #endif
#endif

    constexpr int version_major{ 0 };
    constexpr int version_minor{ 5 };
    constexpr int version_patch{ 0 };
} // namespace rw

// Basic aliases.
#include <stdint.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

#endif
