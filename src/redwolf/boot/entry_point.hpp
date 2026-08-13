#pragma once

#include "redwolf/common.hpp"
#include "redwolf/containers/vec.hpp"
#include "redwolf/memory/memory.hpp"
#include "redwolf/module.hpp"

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

    /**
     * @brief Create all modules required for the application.
     * @details This must be implemented by the user of RedWolf.
     * @return Vector with all modules that will be executed, costructed but not initialised.
     */
    [[nodiscard]] extern rw::Vec<rw::Ptr<rw::Module>> create_modules();
} // namespace rw_user