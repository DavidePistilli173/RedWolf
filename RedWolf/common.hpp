/** @file common.hpp Common macros and definitions. */

#ifndef RW_COMMON_HPP
#define RW_COMMON_HPP

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
} // namespace rw

#endif