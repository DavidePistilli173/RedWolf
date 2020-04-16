#ifndef RW_CORE_HPP
#define RW_CORE_HPP

#ifdef RW_EXPORTS
    #define RW_API __declspec(dllexport)
#else
    #define RW_API __declspec(dllimport)
#endif

namespace rw
{
    #ifdef RW_DEBUG
        constexpr bool debug{ true };
    #else
        constexpr bool debug{ false };
    #endif
}

#endif