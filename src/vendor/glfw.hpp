//
// Created by cjm036653 on 13/08/25.
//

#ifndef GLFW_HPP
#define GLFW_HPP

#include <GLFW/glfw3.h>
#include <format>

namespace rw::vendor {
    /**
     * @brief Enum representing GLFW error codes.
     */
    enum class GlfwError {
        None               = GLFW_NO_ERROR,
        NotInitialized     = GLFW_NOT_INITIALIZED,
        NoCurrentContext   = GLFW_NO_CURRENT_CONTEXT,
        InvalidEnum        = GLFW_INVALID_ENUM,
        InvalidValue       = GLFW_INVALID_VALUE,
        OutOfMemory        = GLFW_OUT_OF_MEMORY,
        ApiUnavailable     = GLFW_API_UNAVAILABLE,
        VersionUnavailable = GLFW_VERSION_UNAVAILABLE,
        PlatformError      = GLFW_PLATFORM_ERROR,
        FormatUnavailable  = GLFW_FORMAT_UNAVAILABLE,
        NoWindowContext    = GLFW_NO_WINDOW_CONTEXT
    };

    /**
     * @brief Get the last GLFW error.
     * @return Last GLFW error code in enum form.
     */
    [[nodiscard]] GlfwError glfw_get_error();
} // namespace rw::vendor

/**
 * @brief std::formatter specialization for rw::vendor::GlfwError
 */
template<>
struct std::formatter<rw::vendor::GlfwError> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::vendor::GlfwError error, std::format_context& ctx) const {
        switch (error) {
        case rw::vendor::GlfwError::None:
            return std::format_to(ctx.out(), "None");
            break;
        case rw::vendor::GlfwError::NotInitialized:
            return std::format_to(ctx.out(), "NotInitialized");
            break;
        case rw::vendor::GlfwError::NoCurrentContext:
            return std::format_to(ctx.out(), "NoCurrentContext");
            break;
        case rw::vendor::GlfwError::InvalidEnum:
            return std::format_to(ctx.out(), "InvalidEnum");
            break;
        case rw::vendor::GlfwError::InvalidValue:
            return std::format_to(ctx.out(), "InvalidValue");
            break;
        case rw::vendor::GlfwError::OutOfMemory:
            return std::format_to(ctx.out(), "OutOfMemory");
            break;
        case rw::vendor::GlfwError::ApiUnavailable:
            return std::format_to(ctx.out(), "ApiUnavailable");
            break;
        case rw::vendor::GlfwError::VersionUnavailable:
            return std::format_to(ctx.out(), "VersionUnavailable");
            break;
        case rw::vendor::GlfwError::PlatformError:
            return std::format_to(ctx.out(), "PlatformError");
            break;
        case rw::vendor::GlfwError::FormatUnavailable:
            return std::format_to(ctx.out(), "FormatUnavailable");
            break;
        case rw::vendor::GlfwError::NoWindowContext:
            return std::format_to(ctx.out(), "NoWindowContext");
            break;
        default:
            return std::format_to(ctx.out(), "Unknown");
            break;
        }
    }
};

#endif // GLFW_HPP
