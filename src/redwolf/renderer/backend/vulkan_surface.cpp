#include "vulkan_surface.hpp"

#include "redwolf/platform/platform.hpp"
#include "vulkan_common.hpp"

#ifdef linux
    #include <vulkan/vulkan_wayland.h>
#endif

rw::vk::Surface::~Surface() {
    vkDestroySurfaceKHR(instance_->handle(), surface_, allocator_);
}

VkSurfaceKHR rw::vk::Surface::handle() const {
    return surface_;
}

bool rw::vk::Surface::init(Ptr<Instance> instance, VkAllocationCallbacks* allocator) {
    const VkWaylandSurfaceCreateInfoKHR create_info{ .sType   = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
                                                     .flags   = 0,
                                                     .display = Platform::display(),
                                                     .surface = Platform::surface() };

    instance_  = std::move(instance);
    allocator_ = allocator;

#ifdef linux
    RW_VK_CHECK(
        vkCreateWaylandSurfaceKHR(instance_->handle(), &create_info, allocator_, &surface_), "Failed to create Vulkan surface: '{}'", false)
#endif

    return true;
}