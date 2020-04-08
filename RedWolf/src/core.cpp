#include <cstdio>

#include "RedWolf/core.hpp"

namespace rw
{
    void print(std::string_view msg)
    {
        std::printf("%s\n", msg.data());
    }
}