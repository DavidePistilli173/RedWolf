#include "vulkan_swapchain_impl.hpp"

#include "vulkan_common.hpp"
#include "vulkan_device_impl.hpp"

#include <ranges>
#include <vulkan/vulkan_core.h>

bool rw::VulkanSwapchainImpl::init(u32 width, u32 height) {
    return create_(width, height);
}

std::optional<u32>
    rw::VulkanSwapchainImpl::next_image_index(std::chrono::nanoseconds timeout, VkSemaphore image_available_semaphore, VkFence fence) {
    auto&          vk_ctx{ VulkanContext::ctx() };
    u32            out_image_index{ 0 };
    const VkResult res{ vkAcquireNextImageKHR(
        vk_ctx.device.logical, vk_ctx.swapchain.handle, timeout.count(), image_available_semaphore, fence, &out_image_index) };

    if (VK_ERROR_OUT_OF_DATE_KHR == res) {
        info("Recreating swapchain.");
        if (!recreate(vk_ctx.framebuffer_width, vk_ctx.framebuffer_height)) {
            error("Failed to recreate swapchain.");
        }
        return {};
    }

    if ((VK_SUCCESS != res) && (VK_SUBOPTIMAL_KHR != res)) {
        error("Failed to acquire swapchain image.");
        return {};
    }

    return out_image_index;
}

bool rw::VulkanSwapchainImpl::present(VkSemaphore render_complete_semaphore, u32 present_image_index) {
    auto& vk_ctx{ VulkanContext::ctx() };

    const VkPresentInfoKHR present_info{ .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                                         .pNext              = nullptr,
                                         .waitSemaphoreCount = 1,
                                         .pWaitSemaphores    = &render_complete_semaphore,
                                         .swapchainCount     = 1,
                                         .pSwapchains        = &vk_ctx.swapchain.handle,
                                         .pImageIndices      = &present_image_index,
                                         .pResults           = nullptr };

    const VkResult res{ vkQueuePresentKHR(vk_ctx.device.present_queue, &present_info) };
    if ((VK_ERROR_OUT_OF_DATE_KHR == res) || (VK_SUBOPTIMAL_KHR == res)) {
        info("Re-creating swapchain.");
        return recreate(vk_ctx.framebuffer_width, vk_ctx.framebuffer_height);
    }

    if (VK_SUCCESS != res) {
        error("Failed to present swapchain image.");
        return false;
    }

    return true;
}

bool rw::VulkanSwapchainImpl::recreate(u32 width, u32 height) {
    destroy_();
    return create_(width, height);
}

void rw::VulkanSwapchainImpl::shutdown() {
    destroy_();
}

bool rw::VulkanSwapchainImpl::create_(u32 width, u32 height) {
    auto& vk_ctx{ VulkanContext::ctx() };

    VkExtent2D extent{ .width = width, .height = height };
    vk_ctx.swapchain.max_frames_in_flight = 2; // Triple-buffering (two rendered frames and one presented frame).

    // Choose a swap surface format.
    vk_ctx.swapchain.image_format = vk_ctx.device.swapchain_support.formats[0]; // Default format.
    for (const auto& format : vk_ctx.device.swapchain_support.formats) {
        // Preferred format.
        if ((VK_FORMAT_B8G8R8A8_UNORM == format.format) && (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == format.colorSpace)) {
            vk_ctx.swapchain.image_format = format;
            break;
        }
    }

    // Choose the present mode.
    VkPresentModeKHR present_mode{ VK_PRESENT_MODE_FIFO_KHR }; // Default, guaranteed to exist.
    for (const auto& mode : vk_ctx.device.swapchain_support.present_modes) {
        // Preferred mode.
        if (VK_PRESENT_MODE_MAILBOX_KHR == mode) {
            present_mode = mode;
        }
    }

    // Re-query swapchain support.
    if (!VulkanDeviceImpl::query_swapchain_support(vk_ctx.device.physical)) {
        error("Failed to query swapchain support.");
    }

    // Swapchain extent.
    if (UINT32_MAX != vk_ctx.device.swapchain_support.capabilities.currentExtent.width) {
        extent = vk_ctx.device.swapchain_support.capabilities.currentExtent;
    }

    // Clamp to the values alowed by the GPU.
    extent.width = std::clamp(
        extent.width,
        vk_ctx.device.swapchain_support.capabilities.minImageExtent.width,
        vk_ctx.device.swapchain_support.capabilities.maxImageExtent.width);
    extent.height = std::clamp(
        extent.height,
        vk_ctx.device.swapchain_support.capabilities.minImageExtent.height,
        vk_ctx.device.swapchain_support.capabilities.maxImageExtent.height);

    u32 image_count{ std::min(
        vk_ctx.device.swapchain_support.capabilities.minImageCount + 1, vk_ctx.device.swapchain_support.capabilities.maxImageCount) };

    VkSwapchainCreateInfoKHR swapchain_create_info{ .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                                    .pNext            = nullptr,
                                                    .surface          = vk_ctx.surface,
                                                    .minImageCount    = image_count,
                                                    .imageFormat      = vk_ctx.swapchain.image_format.format,
                                                    .imageColorSpace  = vk_ctx.swapchain.image_format.colorSpace,
                                                    .imageExtent      = extent,
                                                    .imageArrayLayers = 1,
                                                    .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                                    .preTransform     = vk_ctx.device.swapchain_support.capabilities.currentTransform,
                                                    .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                                    .presentMode      = present_mode,
                                                    .clipped          = VK_TRUE,
                                                    .oldSwapchain     = VK_NULL_HANDLE };

    if (vk_ctx.device.queue_families.graphics_family_index != vk_ctx.device.queue_families.present_family_index) {
        const std::array<u32, 2> queue_familty_indices{ vk_ctx.device.queue_families.graphics_family_index,
                                                        vk_ctx.device.queue_families.present_family_index };
        swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = queue_familty_indices.size();
        swapchain_create_info.pQueueFamilyIndices   = queue_familty_indices.data();
    } else {
        swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices   = nullptr;
    }

    RW_VK_CHECK(
        vkCreateSwapchainKHR(vk_ctx.device.logical, &swapchain_create_info, vk_ctx.allocator.get(), &vk_ctx.swapchain.handle),
        "Failed to create swapchain: '{}'",
        false)

    image_count = 0;
    RW_VK_CHECK(
        vkGetSwapchainImagesKHR(vk_ctx.device.logical, vk_ctx.swapchain.handle, &image_count, nullptr),
        "Failed to enumerate swapchain images: '{}'",
        false)
    if (0 == image_count) {
        error("No images in swapchain.");
        return false;
    }
    vk_ctx.swapchain.images.resize(image_count);
    vk_ctx.swapchain.views.resize(image_count);

    RW_VK_CHECK(
        vkGetSwapchainImagesKHR(vk_ctx.device.logical, vk_ctx.swapchain.handle, &image_count, vk_ctx.swapchain.images.data()),
        "Failed to retrieve swapchain images: '{}'",
        false)

    // Create the image view.
    for (auto [i, view] : std::views::enumerate(vk_ctx.swapchain.views)) {
        VkImageViewCreateInfo view_create_info{ .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                                .pNext            = nullptr,
                                                .image            = vk_ctx.swapchain.images[i],
                                                .viewType         = VK_IMAGE_VIEW_TYPE_2D,
                                                .format           = vk_ctx.swapchain.image_format.format,
                                                .subresourceRange = { .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                      .baseMipLevel   = 0,
                                                                      .levelCount     = 1,
                                                                      .baseArrayLayer = 0,
                                                                      .layerCount     = 1 } };

        RW_VK_CHECK(
            vkCreateImageView(vk_ctx.device.logical, &view_create_info, vk_ctx.allocator.get(), &view),
            "Failed to create image view: '{}'",
            false)
    }

    if (!VulkanDeviceImpl::detect_depth_format()) {
        error("Failed to detect depth buffer format.");
        return false;
    }

    // Create the depth image.
}

void rw::VulkanSwapchainImpl::destroy_() {}