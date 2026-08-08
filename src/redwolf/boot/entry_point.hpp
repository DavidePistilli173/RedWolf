#pragma once

#include "redwolf/app.hpp"
#include "redwolf/common.hpp"

#include <string_view>

/**
 * @brief Entry point of the application, defined by the engine.
 */
i32 main(i32 argc, const char** argv);

namespace rw_user {
    /**
     * @brief Get the name of the application.
     * @details This must be implemented by the user of RedWolf.
     * @return Name of the application.
     */
    [[nodiscard]] extern std::string_view app_name();
} // namespace rw_user