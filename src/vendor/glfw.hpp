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
struct std::formatter<rw::vendor::GlfwError> : std::formatter<std::string> {
    auto format(const rw::vendor::GlfwError& error, std::format_context& ctx) const {
        switch (error) {
        case rw::vendor::GlfwError::None:
            return std::formatter<std::string>::format("None", ctx);
            break;
        case rw::vendor::GlfwError::NotInitialized:
            return std::formatter<std::string>::format("NotInitialized", ctx);
            break;
        case rw::vendor::GlfwError::NoCurrentContext:
            return std::formatter<std::string>::format("NoCurrentContext", ctx);
            break;
        case rw::vendor::GlfwError::InvalidEnum:
            return std::formatter<std::string>::format("InvalidEnum", ctx);
            break;
        case rw::vendor::GlfwError::InvalidValue:
            return std::formatter<std::string>::format("InvalidValue", ctx);
            break;
        case rw::vendor::GlfwError::OutOfMemory:
            return std::formatter<std::string>::format("OutOfMemory", ctx);
            break;
        case rw::vendor::GlfwError::ApiUnavailable:
            return std::formatter<std::string>::format("ApiUnavailable", ctx);
            break;
        case rw::vendor::GlfwError::VersionUnavailable:
            return std::formatter<std::string>::format("VersionUnavailable", ctx);
            break;
        case rw::vendor::GlfwError::PlatformError:
            return std::formatter<std::string>::format("PlatformError", ctx);
            break;
        case rw::vendor::GlfwError::FormatUnavailable:
            return std::formatter<std::string>::format("FormatUnavailable", ctx);
            break;
        case rw::vendor::GlfwError::NoWindowContext:
            return std::formatter<std::string>::format("NoWindowContext", ctx);
            break;
        default:
            return std::formatter<std::string>::format("Unknown", ctx);
            break;
        }
    }
};

#endif // GLFW_HPP
