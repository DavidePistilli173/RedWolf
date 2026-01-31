module;

#include <optional>

export module redwolf.gfx.quad;

import redwolf.common;
import redwolf.gfx.common;
import redwolf.gfx.texture_2d;
import redwolf.math;

export namespace rw::gfx {
    /**
     * @brief 2D quad data that can be submitted to the renderer.
     */
    struct Quad {
        rw::math::Vec3                               position{ 0.0F };      /**< Position of the quad in world coordinates. */
        float                                        rotation{ 0.0F };      /**< Rotation of the quad around the z axis. [rad] */
        rw::math::Vec2                               size{ 1.0F };          /**< Size of the quad. */
        rw::math::Vec4                               color{ color_white };  /**< Colour. */
        float                                        tiling_factor{ 1.0F }; /**< Texture tiling multiplier. */
        Handle<Texture2D>                            texture;               /**< Optional texture to fill the quad. */
        std::optional<rw::gfx::Texture2D::SubRegion> texture_sub_region{};  /**< Optional sub-region of the texture. */
    };
} // namespace rw::gfx
