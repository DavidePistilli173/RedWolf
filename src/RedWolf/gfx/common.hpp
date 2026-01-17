//
// Created by david on 17/01/2026.
//

#ifndef SRC_REDWOLF_GFX_COMMON_HPP
#define SRC_REDWOLF_GFX_COMMON_HPP

#include <cstdint>

namespace rw::gfx {
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

#endif // SRC_REDWOLF_COMMON_HPP
