module;

#include <optional>

export module redwolf.gfx.quad;

import redwolf.common;
import redwolf.gfx.common;
import redwolf.gfx.texture2d;
import redwolf.core.math;

export namespace rw::gfx {
    /**
     * @brief 2D quad data that can be submitted to the renderer.
     */
    struct Quad {
        rw::core::Vec4                               color{ color_white };  /**< Colour. */
        float                                        tiling_factor{ 1.0F }; /**< Texture tiling multiplier. */
        Handle<Texture2D>                            texture;               /**< Optional texture to fill the quad. */
        std::optional<rw::gfx::Texture2D::SubRegion> texture_sub_region{};  /**< Optional sub-region of the texture. */
    };
} // namespace rw::gfx
