module;

#include <array>
#include <cstdint>
#include <glad/glad.h>
#include <string>

export module redwolf.gfx.texture2d;

import redwolf.core.math;

export namespace rw::gfx {
    /**
     * @brief 2D texture class for OpenGL.
     */
    struct Texture2D {
        using SubRegion = std::array<rw::core::Vec2, 4>; /**< Texture coordinates of a texture sub-region. */

        uint32_t gid{ 0U }; /**< OpenGL texture ID. */

        std::string path;                       /**< Path to the texture file. */
        uint32_t    width{ 0U };                /**< Width of the texture. [px] */
        uint32_t    height{ 0U };               /**< Height of the texture. [px] */
        GLenum      internal_format{ GL_RGB8 }; /**< Internal texture format. */
        GLenum      data_format{ GL_RGB };      /**< Texture data format. */
    };
} // namespace rw::gfx
