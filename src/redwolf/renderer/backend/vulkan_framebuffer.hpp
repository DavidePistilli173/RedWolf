#pragma once

#include "redwolf/containers/vec.hpp"
#include "vulkan_renderpass.hpp"

#include <vulkan/vulkan.h>

namespace rw::vk {
    /**
     * @brief Vulkan framebuffer wrapper.
     */
    class Framebuffer {
     public:
        /**
         * @brief Initialisation parameters of the framebuffer.
         */
        struct Params {
            VkAllocationCallbacks*  allocator{ nullptr }; /**< Custom vulkan allocator. */
            const Ptr<Device>&      device;               /**< Rendering device. */
            const Vec<VkImageView>& attachments;          /**< Framebuffer attachments. */
            const Ptr<RenderPass>&  render_pass;          /**< Render pass associated with the framebuffer. */
            u32                     width{ 0U };          /**< Width of the framebuffer. */
            u32                     height{ 0U };         /**< Height of the framebuffer. */
        };

        Framebuffer() = default;
        ~Framebuffer();

        Framebuffer(const Framebuffer&)            = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        Framebuffer(Framebuffer&&)            = delete;
        Framebuffer& operator=(Framebuffer&&) = delete;

        /**
         * @brief Get the raw framebuffer handle.
         */
        [[nodiscard]] VkFramebuffer handle() const;

        /**
         * @brief Initialise the framebuffer.
         * @param params Initialisation parameters.
         * @return true on success, false otherwise.
         */
        [[nodiscard]] bool init(const Params& params);

     private:
        VkAllocationCallbacks* allocator_{ nullptr };                /**< Custom vulkan allocator. */
        Ptr<Device>            device_;                              /**< Rendering device. */
        VkFramebuffer          framebuffer_{ VK_NULL_HANDLE };       /**< Raw framebuffer handle. */
        Vec<VkImageView>       attachments_{ MemoryType::renderer }; /**< Attachments of the framebuffer. */
        Ptr<RenderPass>        render_pass_;                         /**< Render pass the framebuffer is associated with. */
    };
} // namespace rw::vk