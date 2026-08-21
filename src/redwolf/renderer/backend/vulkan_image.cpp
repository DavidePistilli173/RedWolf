#include "vulkan_image.hpp"

#include "vulkan_common.hpp"

rw::vk::Image2D::~Image2D() {
    if (VK_NULL_HANDLE != view_) {
        vkDestroyImageView(device_->logical(), view_, allocator_);
    }

    if (VK_NULL_HANDLE != memory_) {
        vkFreeMemory(device_->logical(), memory_, allocator_);
    }

    if (VK_NULL_HANDLE != image_) {
        vkDestroyImage(device_->logical(), image_, allocator_);
    }
}

VkImage rw::vk::Image2D::handle() const {
    return image_;
}

bool rw::vk::Image2D::init(const Params& params) {
    device_    = params.device;
    allocator_ = params.allocator;

    width_  = params.width;
    height_ = params.height;

    const VkImageCreateInfo image_create_info{ .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                                               .pNext         = nullptr,
                                               .imageType     = VK_IMAGE_TYPE_2D,
                                               .format        = params.format,
                                               .extent        = { .width = width_, .height = height_, .depth = 1 },
                                               .mipLevels     = 1,
                                               .arrayLayers   = 1,
                                               .samples       = VK_SAMPLE_COUNT_1_BIT,
                                               .tiling        = params.tiling,
                                               .usage         = params.usage,
                                               .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
                                               .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED };

    RW_VK_CHECK(vkCreateImage(device_->logical(), &image_create_info, allocator_, &image_), "Failed to create image: '{}'", false)

    // Query memory requirements.
    VkMemoryRequirements memory_requirements{};
    vkGetImageMemoryRequirements(device_->logical(), image_, &memory_requirements);

    const auto mem_type_res{ device_->find_memory_index(memory_requirements.memoryTypeBits, params.memory_flags) };
    if (!mem_type_res.has_value()) {
        error("Memory type not found. Image not valid.");
        return false;
    }

    // Allocate memory.
    const VkMemoryAllocateInfo memory_allocate_info{ .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                                                     .allocationSize  = memory_requirements.size,
                                                     .memoryTypeIndex = mem_type_res.value() };
    RW_VK_CHECK(vkAllocateMemory(device_->logical(), &memory_allocate_info, allocator_, &memory_), "Failed to allocate memory: '{}'", false)

    // Bind the memory.
    RW_VK_CHECK(vkBindImageMemory(device_->logical(), image_, memory_, 0), "Failed to assign memory to image.", false)

    // Create the view.
    if (!create_view_(params.format, params.aspect_flags)) {
        error("Failed to create view.");
        return false;
    }

    return true;
}

VkImageView rw::vk::Image2D::view() const {
    return view_;
}

bool rw::vk::Image2D::create_view_(VkFormat format, VkImageAspectFlags aspect_flags) {
    const VkImageViewCreateInfo view_create_info{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image            = image_,
        .viewType         = VK_IMAGE_VIEW_TYPE_2D,
        .format           = format,
        .subresourceRange = { .aspectMask = aspect_flags, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
    };

    RW_VK_CHECK(vkCreateImageView(device_->logical(), &view_create_info, allocator_, &view_), "Failed to create image view: '{}'", false)

    return true;
}
