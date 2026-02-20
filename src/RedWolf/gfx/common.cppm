module;

#include <cstdint>

export module redwolf.gfx.common;

import redwolf.core.math;

export namespace rw::gfx {
    /**
     * @brief Available rendering APIs.
     */
    enum class GraphicsApi : uint8_t { opengl };

#ifdef RW_GFX_API_OPENGL
    constexpr auto active_api{ GraphicsApi::opengl }; /**< Currently selected rendering API. */
#else
    #error "No rendering API selected."
#endif

    /**
     * @brief Pure white.
     */
    constexpr rw::core::Vec4 color_white{ 1.0F, 1.0F, 1.0F, 1.0F };

    /**
     * @brief Data formats supported by textures.
     */
    enum class TextureFormat : uint8_t { r8, rgb8, rgba8, rgba32f };

    /**
     * @brief Descriptor for creating a texture.
     */
    struct TextureDescriptor {
        uint32_t      width{ 0U };                    /**< Width of the texture in pixels. */
        uint32_t      height{ 0U };                   /**< Height of the texture in pixels. */
        TextureFormat format{ TextureFormat::rgba8 }; /**< Format of the texture data. */
    };
} // namespace rw::gfx
