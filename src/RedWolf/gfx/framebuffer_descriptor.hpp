//
// Created by david on 17/10/2025.
//

#ifndef SRC_REDWOLF_FRAMEBUFFER_DESCRIPTOR_HPP
#define SRC_REDWOLF_FRAMEBUFFER_DESCRIPTOR_HPP

#include <cstdint>

namespace rw::gfx {
    /**
     * @brief Options for creating a Framebuffer.
     */
    struct FramebufferDescriptor {
        uint32_t width{ 0U };               /**< Framebuffer width in pixels. */
        uint32_t height{ 0U };              /**< Framebuffer height in pixels. */
        bool     swapchain_target{ false }; /**< Is this framebuffer meant to be rendered to the screen? */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_FRAMEBUFFER_DESCRIPTOR_HPP
