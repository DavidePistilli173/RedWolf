#ifndef RW_CORE_HPP
#define RW_CORE_HPP

#include <string_view>

#ifdef RW_EXPORTS
    #define RW_API __declspec(dllexport)
#else
    #define RW_API __declspec(dllimport)
#endif

namespace rw
{
    extern RW_API void print(std::string_view msg);
}

#endif