module;

#include <cstdint>

export module redwolf.gfx.framebuffer;

import redwolf.gfx.framebuffer_descriptor;

export namespace rw::gfx {
    /**
     * @brief OpenGL Framebuffer implementation.
     */
    struct Framebuffer {
        rw::gfx::FramebufferDescriptor descriptor; /**< Current framebuffer settings. */

        uint32_t gid{ 0U };              /**< OpenGL object ID. */
        uint32_t color_attachment{ 0U }; /**< ID of the colour attachment texture for the framebuffer. */
        uint32_t depth_attachment{ 0U }; /**< ID of the depth attachment texture for the framebuffer. */
    };
} // namespace rw::gfx
