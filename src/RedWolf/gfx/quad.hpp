//
// Created by cjm036653 on 21/09/2025.
//

#ifndef SRC_REDWOLF_QUAD_HPP
#define SRC_REDWOLF_QUAD_HPP

#include "RedWolf/math/math.hpp"
#include "gfx.hpp"

namespace rw::gfx {
    /**
     * @brief 2D quad.
     */
    struct Quad {
        rw::math::Vec3 position{ 0.0F };      /**< Position of the quad in world coordinates. */
        float          rotation{ 0.0F };      /**< Rotation of the quad around the z axis. [deg] */
        rw::math::Vec2 size{ 1.0F };          /**< Size of the quad. */
        Texture2D*     texture{ nullptr };    /**< Texture. */
        rw::math::Vec4 color{ color_white };  /**< Colour. */
        float          tiling_factor{ 1.0F }; /**< Texture tiling multiplier. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_QUAD_HPP
