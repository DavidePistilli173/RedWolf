#pragma once

#include "common.hpp"
#include "redwolf/containers/str.hpp"

namespace rw {
    /**
     * @brief Manager for user-provided data needed by the engine.
     */
    class UserData {
     public:
        /**
         * @brief Get the application name.
         */
        [[nodiscard]] static View<char> app_name();

        /**
         * @brief Get the application version.
         */
        [[nodiscard]] static const VersionInfo& app_version();

        /**
         * @brief Initialise the user data manager.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool init();

        /**
         * @brief Shutdown the user data manager.
         */
        static void shutdown();

     private:
        UserData() = default;

        Str         app_name_{ MemoryCategory::user_data }; /**< Application name. */
        VersionInfo app_version_;                           /**< Application version. */
    };
} // namespace rw