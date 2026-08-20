#pragma once

#include "redwolf/common.hpp"
#include "vulkan_common.hpp"

#include <optional>
#include <vulkan/vulkan_core.h>

namespace rw {
    /**
     * @brief Vulkan image.
     */
    class VulkanImageImpl {
     public:
        /**
         * @brief Create a Vulkan image.
         * @param width Width of the image.
         * @param height Height of the image.
         * @param format Data format.
         * @param tiling Tiling configuration.
         * @param usage Intended usage of the image.
         * @param memory_flags Memory allocation flags.
         * @param create_view If true, also creates the view.
         * @param aspect_flags Aspect flags.
         * @return Newly created image, if no errors occured.
         */
        [[nodiscard]] static std::optional<VulkanImage> create(
            u32                   width,
            u32                   height,
            VkFormat              format,
            VkImageTiling         tiling,
            VkImageUsageFlags     usage,
            VkMemoryPropertyFlags memory_flags,
            bool                  create_view,
            VkImageAspectFlags    aspect_flags);

        /**
         * @brief Create a view over an already created image.
         * @param format Image format.
         * @param aspect_flags Aspect flags.
         * @param image Data of the already created image.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] static bool create_view(VkFormat format, VkImageAspectFlags aspect_flags, VulkanImage& image);

        /**
         * @brief Destroy a given image.
         * @param image Image to destroy.
         */
        static void destroy(VulkanImage& image);

     private:
    };
} // namespace rw