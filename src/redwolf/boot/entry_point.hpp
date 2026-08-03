#pragma once

#include "redwolf/common.hpp"

/**
* @brief Entry point of the application, defined by the engine.
*/
i32 main(i32 argc, const char** argv);

namespace rw_user
{
    extern void init_modules();
}