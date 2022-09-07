/** @file common.hpp Common macros and definitions. */

#ifndef RW_COMMON_HPP
#define RW_COMMON_HPP

// Defines
#define GLFW_INCLUDE_VULKAN /**< Used so that GLFW automatically includes Vulkan headers. */

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

   #ifdef RW_DLL
      #define RW_API __declspec(dllexport)
   #else
      #define RW_API __declspec(dllimport)
   #endif
#else
   constexpr bool windows{ false };

   #define RW_API
#endif
} // namespace rw

#endif