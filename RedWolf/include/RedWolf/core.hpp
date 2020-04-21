/** \file core.hpp Core macros and definitions. */

#ifndef RW_CORE_HPP
#define RW_CORE_HPP

/** 
    \def RW_API 
    \brief Sets dll exporting or importing. 
*/
#ifdef RW_EXPORTS
    #define RW_API __declspec(dllexport)
#else
    #define RW_API __declspec(dllimport)
#endif

namespace rw
{
    /** 
        \var debug 
        \brief true for debug configurations, false otherwise.
    */
    #ifdef RW_DEBUG
        constexpr bool debug{ true };
    #else
        constexpr bool debug{ false };
    #endif
}

#endif