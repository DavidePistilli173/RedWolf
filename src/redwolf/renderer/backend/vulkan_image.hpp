#pragma once

#include "redwolf/common.hpp"
#include "vulkan_device.hpp"

#include <optional>
#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Vulkan image.
     */
    class Image2D {
     public:
        /**
         * @brief Initialisation parameters.
         */
        struct Params {
            VkAllocationCallbacks* allocator{ nullptr };                                /**< Custom vulkan allocator. */
            const Ptr<Device>&     device;                                              /**< Rendering device. */
            u32                    width{ 0U };                                         /**<  Width of the image. */
            u32                    height{ 0U };                                        /**< Height of the image. */
            VkFormat               format{ VK_FORMAT_UNDEFINED };                       /**< Data format. */
            VkImageTiling          tiling{ VK_IMAGE_TILING_OPTIMAL };                   /**< Memory layout. */
            VkImageUsageFlags      usage{ VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT };        /**< Intended usage of the image. */
            VkMemoryPropertyFlags  memory_flags{ VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }; /**< Memory allocation flags. */
            VkImageAspectFlags     aspect_flags{ VK_IMAGE_ASPECT_COLOR_BIT };           /**< Aspect flags. */
        };

        Image2D() = default;
        ~Image2D();

        Image2D(const Image2D&)            = delete;
        Image2D& operator=(const Image2D&) = delete;

        Image2D(Image2D&&) noexcept            = delete;
        Image2D& operator=(Image2D&&) noexcept = delete;

        /**
         * @brief Get the raw image handle.
         */
        [[nodiscard]] VkImage handle() const;

        /**
         * @brief Initialise the image.
         * @param params Initialisation parameters.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(const Params& params);

        /**
         * @brief Get the raw image view handle.
         */
        [[nodiscard]] VkImageView view() const;

     private:
        /**
         * @brief Create the image view.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool create_view_(VkFormat format, VkImageAspectFlags aspect_flags);

        VkAllocationCallbacks* allocator_{ nullptr }; /**< Custom vulkan allocator. */
        Ptr<Device>            device_;               /**< Rendering device. */

        VkImage        image_{ VK_NULL_HANDLE };  /**< Handle to the actual image. */
        VkDeviceMemory memory_{ VK_NULL_HANDLE }; /**< Handle to the image memory. */
        VkImageView    view_{ VK_NULL_HANDLE };   /**< Handle to the image view. */
        u32            width_{ 0U };              /**< Width of the image. */
        u32            height_{ 0U };             /**< Height of the image. */
    };
} // namespace rw::vk