#pragma once

#include "redwolf/memory/ptr.hpp"
#include "vulkan_instance.hpp"

#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Surface to draw on.
     */
    class Surface {
     public:
        Surface() = default;
        ~Surface();

        Surface(const Surface&)            = delete;
        Surface& operator=(const Surface&) = delete;

        Surface(Surface&&)            = delete;
        Surface& operator=(Surface&&) = delete;

        /**
         * @brief Get the raw surface handle.
         */
        [[nodiscard]] VkSurfaceKHR handle() const;

        /**
         * @brief Initialise the surface.
         * @param instance Vulkan instance.
         * @param allocator Custom vulkan allocator.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(Ptr<Instance> instance, VkAllocationCallbacks* allocator);

     private:
        Ptr<Instance>          instance_;                  /**< Vulkan instance. */
        VkAllocationCallbacks* allocator_{ nullptr };      /**< Custom vulkan allocator. */
        VkSurfaceKHR           surface_{ VK_NULL_HANDLE }; /**< Raw surface handle. */
    };
} // namespace rw::vk