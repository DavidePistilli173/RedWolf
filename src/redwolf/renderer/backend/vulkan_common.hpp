#pragma once

#include "redwolf/logger.hpp"

#include <array>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

#define RW_VK_CHECK(res, message, ret_val)        \
    if (VK_SUCCESS != res) {                      \
        rw::error(message, string_VkResult(res)); \
        return ret_val;                           \
    }

namespace rw::vk {
    /**
     * @brief Extensions required for Vulkan rendering.
     */
    constexpr std::array required_extensions{ VK_KHR_SURFACE_EXTENSION_NAME
#ifdef RW_ENABLE_VULKAN_DEBUG
                                              ,
                                              VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
    };

    /**
     * @brief Check whether a Vulkan result is an error or not, according to the spec.
     * @param result Result to check.
     * @return true if the result is not an error, false if it is an error.
     */
    [[nodiscard]] bool result_not_error(VkResult result);
} // namespace rw::vk