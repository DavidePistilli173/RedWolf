module;

#include <string>

export module redwolf.gfx.text;

import redwolf.common;
import redwolf.gfx.common;
import redwolf.gfx.font;
import redwolf.core.math;

export namespace rw::gfx {
    /**
     * @brief Text data that can be submitted to the renderer.
     */
    struct Text {
        std::string    string;                          /**< Actual string to render. */
        rw::core::Vec3 position{ 0.0F };                /**< Position of the quad in world coordinates. */
        float          rotation{ 0.0F };                /**< Rotation of the quad around the z axis. [rad] */
        float          pixel_size{ 0.0F };              /**< Pixel size of the text. */
        rw::core::Vec4 foreground_color{ color_white }; /**< Colour of the text. */
        Handle<Font>   font;                            /**< Font to use for rendering the text. */
    };
} // namespace rw::gfx