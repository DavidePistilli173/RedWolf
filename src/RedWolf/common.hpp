//
// Created by cjm036653 on 10/08/25.
//

#ifndef COMMON_HPP
#define COMMON_HPP

#include <chrono>
#include <filesystem>
#include <format>

namespace rw {
    static constexpr float seconds_to_milliseconds{ 1e3F };      /**< Conversion factor between seconds and milliseconds. */
    static constexpr float nanoseconds_to_milliseconds{ 1e-6F }; /**< Conversion factor between nanoseconds and milliseconds. */
    static constexpr float nanoseconds_to_seconds{ 1e-9F };      /**< Conversion factor between nanoseconds and seconds. */

    /**
     * @brief ID for ReaWolf resources.
     */
    using Id = uint64_t;

    static constexpr Id invalid_id{ 0U }; /**< Constant representing an invalid resource ID. */

    /**
     * @brief Generic handle for RedWolf resources, cheap enough to be copied around.
     * @details This handle does not own the resource it points to, and is only valid as long as the resource exists.
     * @tparam T Type of resource contained in the handle.
     */
    template<typename T>
    struct Handle {
        /**
         * @brief Arrow operator to access the underlying resource.
         * @return Pointer to the resource.
         */
        [[nodiscard]] T* operator->() const noexcept {
            return ptr;
        }

        /**
         * @brief Check if the handle is valid (i.e., points to a resource).
         * @return True if the handle is valid, false otherwise.
         */
        [[nodiscard]] bool valid() const noexcept {
            return nullptr != ptr;
        }

        Id id{ invalid_id }; /**< Unique identifier for the resource. */
        T* ptr{ nullptr };   /**< Pointer to the resource. */
    };

    /**
     * @brief Structure representing version information.
     */
    struct VersionInfo {
        uint32_t major{ 0U }; /**< Major version number. */
        uint32_t minor{ 0U }; /**< Minor version number. */
        uint32_t patch{ 0U }; /**< Patch version number. */
    };

    static constexpr VersionInfo current_version{ .major = 0U, .minor = 6U, .patch = 0U }; /**< Current version of the RedWolf engine. */

    namespace core {
        /**
         * @brief Concept for objects that can be used as std::format arguments.
         */
        template<typename T>
        concept IsFormattable = std::is_default_constructible_v<std::formatter<T>>;
    } // namespace core

    namespace time {
        /**
         * @brief Concept for representing an std::chrono clock.
         */
        template<typename T>
        concept IsChronoClock = std::is_same_v<T, std::chrono::system_clock> || std::is_same_v<T, std::chrono::steady_clock> ||
                                std::is_same_v<T, std::chrono::high_resolution_clock> ||
                                std::is_same_v<T, std::filesystem::file_time_type::clock> || std::is_same_v<T, std::chrono::file_clock>;
    } // namespace time
} // namespace rw

/**
 * @brief std::formatter specialization for rw::VersionInfo
 */
template<>
struct std::formatter<rw::VersionInfo> : std::formatter<std::string> {
    auto format(const rw::VersionInfo& version_info, std::format_context& ctx) const {
        return std::formatter<std::string>::format(
            std::format("v{}.{}.{}", version_info.major, version_info.minor, version_info.patch), ctx);
    }
};

#endif // COMMON_HPP
