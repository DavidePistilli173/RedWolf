#include "vulkan_swapchain_impl.hpp"

#include "vulkan_common.hpp"
#include "vulkan_device_impl.hpp"

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
}

void rw::VulkanSwapchainImpl::destroy_() {}