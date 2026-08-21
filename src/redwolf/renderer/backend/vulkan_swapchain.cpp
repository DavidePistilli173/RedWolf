#include "vulkan_swapchain.hpp"

#include "vulkan_common.hpp"

#include <ranges>
#include <vulkan/vulkan_core.h>

rw::vk::Swapchain::~Swapchain() {
    destroy_();
}

bool rw::vk::Swapchain::init(const Params& params) {
    allocator_ = params.allocator;
    surface_   = params.surface;
    device_    = params.device;
    width_     = params.width;
    height_    = params.height;

    return create_();
}

std::optional<u32>
    rw::vk::Swapchain::next_image_index(std::chrono::nanoseconds timeout, VkSemaphore image_available_semaphore, VkFence fence) {
    u32            out_image_index{ 0 };
    const VkResult res{ vkAcquireNextImageKHR(
        device_->logical(), swapchain_, timeout.count(), image_available_semaphore, fence, &out_image_index) };

    if (VK_ERROR_OUT_OF_DATE_KHR == res) {
        info("Recreating swapchain.");
        if (!recreate(width_, height_)) {
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

bool rw::vk::Swapchain::present(VkSemaphore render_complete_semaphore, u32 present_image_index) {
    const VkPresentInfoKHR present_info{ .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                                         .pNext              = nullptr,
                                         .waitSemaphoreCount = 1,
                                         .pWaitSemaphores    = &render_complete_semaphore,
                                         .swapchainCount     = 1,
                                         .pSwapchains        = &swapchain_,
                                         .pImageIndices      = &present_image_index,
                                         .pResults           = nullptr };

    const VkResult res{ vkQueuePresentKHR(device_->present_queue(), &present_info) };
    if ((VK_ERROR_OUT_OF_DATE_KHR == res) || (VK_SUBOPTIMAL_KHR == res)) {
        info("Re-creating swapchain.");
        return recreate(width_, height_);
    }

    if (VK_SUCCESS != res) {
        error("Failed to present swapchain image.");
        return false;
    }

    return true;
}

bool rw::vk::Swapchain::recreate(u32 width, u32 height) {
    destroy_();

    width_  = width;
    height_ = height;
    return create_();
}

bool rw::vk::Swapchain::create_() {
    VkExtent2D extent{ .width = width_, .height = height_ };

    // Choose a swap surface format.
    image_format_ = device_->swapchain_support().formats[0]; // Default format.
    for (const auto& format : device_->swapchain_support().formats) {
        // Preferred format.
        if ((VK_FORMAT_B8G8R8A8_UNORM == format.format) && (VK_COLOR_SPACE_SRGB_NONLINEAR_KHR == format.colorSpace)) {
            image_format_ = format;
            break;
        }
    }

    // Choose the present mode.
    VkPresentModeKHR present_mode{ VK_PRESENT_MODE_FIFO_KHR }; // Default, guaranteed to exist.
    for (const auto& mode : device_->swapchain_support().present_modes) {
        // Preferred mode.
        if (VK_PRESENT_MODE_MAILBOX_KHR == mode) {
            present_mode = mode;
        }
    }

    // Re-query swapchain support.
    if (!device_->query_swapchain_support(device_->physical())) {
        error("Failed to query swapchain support.");
    }

    // Swapchain extent.
    if (UINT32_MAX != device_->swapchain_support().capabilities.currentExtent.width) {
        extent = device_->swapchain_support().capabilities.currentExtent;
    }

    // Clamp to the values alowed by the GPU.
    extent.width = std::clamp(
        extent.width,
        device_->swapchain_support().capabilities.minImageExtent.width,
        device_->swapchain_support().capabilities.maxImageExtent.width);
    extent.height = std::clamp(
        extent.height,
        device_->swapchain_support().capabilities.minImageExtent.height,
        device_->swapchain_support().capabilities.maxImageExtent.height);

    u32 image_count{ 0U };
    if (0 < device_->swapchain_support().capabilities.maxImageCount) {
        image_count =
            std::min(device_->swapchain_support().capabilities.minImageCount + 1, device_->swapchain_support().capabilities.maxImageCount);
    } else {
        image_count = device_->swapchain_support().capabilities.minImageCount + 1;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info{ .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                                                    .pNext            = nullptr,
                                                    .surface          = surface_->handle(),
                                                    .minImageCount    = image_count,
                                                    .imageFormat      = image_format_.format,
                                                    .imageColorSpace  = image_format_.colorSpace,
                                                    .imageExtent      = extent,
                                                    .imageArrayLayers = 1,
                                                    .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                                    .preTransform     = device_->swapchain_support().capabilities.currentTransform,
                                                    .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                                                    .presentMode      = present_mode,
                                                    .clipped          = VK_TRUE,
                                                    .oldSwapchain     = VK_NULL_HANDLE };

    if (device_->queue_families().graphics_family_index != device_->queue_families().present_family_index) {
        const std::array<u32, 2> queue_familty_indices{ device_->queue_families().graphics_family_index,
                                                        device_->queue_families().present_family_index };
        swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = queue_familty_indices.size();
        swapchain_create_info.pQueueFamilyIndices   = queue_familty_indices.data();
    } else {
        swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices   = nullptr;
    }

    RW_VK_CHECK(
        vkCreateSwapchainKHR(device_->logical(), &swapchain_create_info, allocator_, &swapchain_),
        "Failed to create swapchain: '{}'",
        false)

    image_count = 0;
    RW_VK_CHECK(
        vkGetSwapchainImagesKHR(device_->logical(), swapchain_, &image_count, nullptr), "Failed to enumerate swapchain images: '{}'", false)
    if (0 == image_count) {
        error("No images in swapchain.");
        return false;
    }
    images_.resize(image_count);
    views_.resize(image_count);

    RW_VK_CHECK(
        vkGetSwapchainImagesKHR(device_->logical(), swapchain_, &image_count, images_.data()),
        "Failed to retrieve swapchain images: '{}'",
        false)

    // Create the image view.
    for (auto [i, view] : std::views::enumerate(views_)) {
        VkImageViewCreateInfo view_create_info{ .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                                                .pNext            = nullptr,
                                                .image            = images_[i],
                                                .viewType         = VK_IMAGE_VIEW_TYPE_2D,
                                                .format           = image_format_.format,
                                                .subresourceRange = { .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                                                                      .baseMipLevel   = 0,
                                                                      .levelCount     = 1,
                                                                      .baseArrayLayer = 0,
                                                                      .layerCount     = 1 } };

        RW_VK_CHECK(vkCreateImageView(device_->logical(), &view_create_info, allocator_, &view), "Failed to create image view: '{}'", false)
    }

    if (!device_->detect_depth_format()) {
        error("Failed to detect depth buffer format.");
        return false;
    }

    // Create the depth image.
    depth_buffer_ = Memory::new_object<Image2D>(MemoryType::renderer);
    if (!depth_buffer_->init(
            Image2D::Params{ .allocator    = allocator_,
                             .device       = device_,
                             .width        = extent.width,
                             .height       = extent.height,
                             .format       = device_->depth_format(),
                             .tiling       = VK_IMAGE_TILING_OPTIMAL,
                             .usage        = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                             .memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                             .aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT

            })) {
        error("Failed to create depth buffer.");
        return false;
    }

    info("Swapchain created successfully.");
    return true;
}

void rw::vk::Swapchain::destroy_() {
    depth_buffer_.reset();

    // Only destroy the views.
    for (const auto& view : views_) {
        vkDestroyImageView(device_->logical(), view, allocator_);
    }

    vkDestroySwapchainKHR(device_->logical(), swapchain_, allocator_);
}