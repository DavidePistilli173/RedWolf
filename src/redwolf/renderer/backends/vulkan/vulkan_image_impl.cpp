#include "vulkan_image_impl.hpp"

#include "redwolf/renderer/backends/vulkan/vulkan_common.hpp"
#include "vulkan_device_impl.hpp"

#include <vulkan/vulkan_core.h>

std::optional<rw::VulkanImage> rw::VulkanImageImpl::create(
    u32                   width,
    u32                   height,
    VkFormat              format,
    VkImageTiling         tiling,
    VkImageUsageFlags     usage,
    VkMemoryPropertyFlags memory_flags,
    bool                  create_view,
    VkImageAspectFlags    aspect_flags) {
    auto& vk_ctx{ VulkanContext::ctx() };

    VulkanImage image;
    image.width  = width;
    image.height = height;

    const VkImageCreateInfo image_create_info{ .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                                               .pNext         = nullptr,
                                               .imageType     = VK_IMAGE_TYPE_2D,
                                               .format        = format,
                                               .extent        = { .width = width, .height = height, .depth = 1 },
                                               .mipLevels     = 1,
                                               .arrayLayers   = 1,
                                               .samples       = VK_SAMPLE_COUNT_1_BIT,
                                               .tiling        = tiling,
                                               .usage         = usage,
                                               .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
                                               .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED };

    RW_VK_CHECK(
        vkCreateImage(vk_ctx.device.logical, &image_create_info, vk_ctx.allocator.get(), &image.handle), "Failed to create image: '{}'", {})

    // Query memory requirements.
    VkMemoryRequirements memory_requirements{};
    vkGetImageMemoryRequirements(vk_ctx.device.logical, image.handle, &memory_requirements);

    const auto mem_type_res{ VulkanDeviceImpl::find_memory_index(memory_requirements.memoryTypeBits, memory_flags) };
    if (!mem_type_res.has_value()) {
        error("Memory type not found. Image not valid.");
        return {};
    }

    // Allocate memory.
    const VkMemoryAllocateInfo memory_allocate_info{ .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                                     .allocationSize  = memory_requirements.size,
                                                     .memoryTypeIndex = mem_type_res.value() };
    RW_VK_CHECK(
        vkAllocateMemory(vk_ctx.device.logical, &memory_allocate_info, vk_ctx.allocator.get(), &image.memory),
        "Failed to allocate memory: '{}'",
        {})

    // Bind the memory.
    RW_VK_CHECK(vkBindImageMemory(vk_ctx.device.logical, image.handle, image.memory, 0), "Failed to assign memory to image.", {})

    if (create_view) {
        if (!VulkanImageImpl::create_view(format, aspect_flags, image)) {
            error("Failed to create view.");
            return {};
        }
    }

    return image;
}

bool rw::VulkanImageImpl::create_view(VkFormat format, VkImageAspectFlags aspect_flags, VulkanImage& image) {
    auto& vk_ctx{ VulkanContext::ctx() };

    const VkImageViewCreateInfo view_create_info{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image            = image.handle,
        .viewType         = VK_IMAGE_VIEW_TYPE_2D,
        .format           = format,
        .subresourceRange = { .aspectMask = aspect_flags, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
    };

    RW_VK_CHECK(
        vkCreateImageView(vk_ctx.device.logical, &view_create_info, vk_ctx.allocator.get(), &image.view),
        "Failed to create image view: '{}'",
        false)

    return true;
}

void rw::VulkanImageImpl::destroy(VulkanImage& image) {
    auto& vk_ctx{ VulkanContext::ctx() };

    if (VK_NULL_HANDLE != image.view) {
        vkDestroyImageView(vk_ctx.device.logical, image.view, vk_ctx.allocator.get());
    }

    if (VK_NULL_HANDLE != image.memory) {
        vkFreeMemory(vk_ctx.device.logical, image.memory, vk_ctx.allocator.get());
    }

    if (VK_NULL_HANDLE != image.handle) {
        vkDestroyImage(vk_ctx.device.logical, image.handle, vk_ctx.allocator.get());
    }
}