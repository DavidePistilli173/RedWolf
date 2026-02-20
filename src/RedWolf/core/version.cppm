module;

#include <cstdint>
#include <format>

export module redwolf.core.version;

export namespace rw {
    /**
     * @brief Structure representing version information.
     */
    struct VersionInfo {
        uint32_t major{ 0U }; /**< Major version number. */
        uint32_t minor{ 0U }; /**< Minor version number. */
        uint32_t patch{ 0U }; /**< Patch version number. */
    };

    constexpr VersionInfo current_version{ .major = 0U, .minor = 7U, .patch = 0U }; /**< Current version of the RedWolf engine. */
} // namespace rw

/**
 * @brief std::formatter specialization for rw::VersionInfo
 */
template<>
struct std::formatter<rw::VersionInfo> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::VersionInfo& version_info, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "v{}.{}.{}", version_info.major, version_info.minor, version_info.patch);
    }
};