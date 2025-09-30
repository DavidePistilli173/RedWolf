//
// Created by cjm036653 on 21/09/2025.
//

#ifndef SRC_REDWOLF_QUAD_HPP
#define SRC_REDWOLF_QUAD_HPP

#include "RedWolf/math/math.hpp"
#include "gfx.hpp"

namespace rw::gfx {
    /**
     * @brief 2D quad data that can be submitted to the renderer.
     */
    struct Quad {
        rw::math::Vec3 position{ 0.0F };      /**< Position of the quad in world coordinates. */
        float          rotation{ 0.0F };      /**< Rotation of the quad around the z axis. [deg] */
        rw::math::Vec2 size{ 1.0F };          /**< Size of the quad. */
        Texture2D*     texture{ nullptr };    /**< Texture. */
        rw::math::Vec4 color{ color_white };  /**< Colour. */
        float          tiling_factor{ 1.0F }; /**< Texture tiling multiplier. */
    };

#pragma pack(push, 1)
    /**
     * @brief Vertex data for a quad on the GPU.
     */
    struct QuadVertex {
        rw::math::Vec3 position{ 0.0F };      /**< Position of the vertex. */
        rw::math::Vec4 color{ 0.0F };         /**< Colour of the vertex. */
        rw::math::Vec2 tex_coord{ 0.0F };     /**< Texture coordinate of the vertex. */
        float          tex_index{ 0.0F };     /**< Index of the texture to use for this vertex. */
        float          tiling_factor{ 1.0F }; /**< Tiling factor of the texture. */
    };
#pragma pack(pop)
} // namespace rw::gfx

#endif // SRC_REDWOLF_QUAD_HPP
