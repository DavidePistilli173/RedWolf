#pragma once

#include "redwolf/common.hpp"

#include <string_view>

/**
 * @brief Entry point of the application, defined by the engine.
 */
i32 main(i32 argc, const char** argv);

namespace rw_user {
    [[nodiscard]] extern std::string_view app_name();
}