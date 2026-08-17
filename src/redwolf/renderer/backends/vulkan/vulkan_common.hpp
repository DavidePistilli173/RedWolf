#pragma once

#include "redwolf/logger.hpp"

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan.h>

#define RW_VK_CHECK(res, message, ret_val)        \
    if (VK_SUCCESS != res) {                      \
        rw::error(message, string_VkResult(res)); \
        return ret_val;                           \
    }
