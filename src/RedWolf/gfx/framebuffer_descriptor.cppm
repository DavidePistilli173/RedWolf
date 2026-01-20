module;

#include <cstdint>

export module redwolf.gfx.framebuffer_descriptor;

export namespace rw::gfx {
    /**
     * @brief Options for creating a Framebuffer.
     */
    struct FramebufferDescriptor {
        uint32_t width{ 0U };               /**< Framebuffer width in pixels. */
        uint32_t height{ 0U };              /**< Framebuffer height in pixels. */
        bool     swapchain_target{ false }; /**< Is this framebuffer meant to be rendered to the screen? */
    };
} // namespace rw::gfx
