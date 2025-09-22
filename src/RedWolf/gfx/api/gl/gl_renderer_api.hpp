//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_GL_RENDERER_API_HPP
#define SRC_REDWOLF_GL_RENDERER_API_HPP

#include "RedWolf/math/math.hpp"
#include "gl_vertex_array.hpp"

#include <memory>

namespace rw::gfx::api::gl {
    /**
     * @brief Collection of raw OpenGL calls.
     */
    class RendererApi {
     public:
        /**
         * @brief Clear the screen with the currently set colour.
         */
        static void clear_screen();

        /**
         * @brief Draw an indexed set of vertices bound to a vertex array.
         * @param vertex_array Vertex array to draw.
         * @param index_count Number of indices to draw. If 0, the entire index buffer is drawn.
         */
        static void draw_indexed(const VertexArray* vertex_array, const uint32_t index_count);

        /**
         * @brief Initialize the rendering API.
         */
        static void init();

        /**
         * @brief Set the colour to use to clear the screen.
         * @param color Colour to use when clearing the screen.
         */
        static void set_clear_color(const rw::math::Vec4& color);

        /**
         * @brief Set a new viewport for rendering.
         * @param x X coordinate of the bottom-left corner of the viewport.
         * @param y Y coordinate of the bottom-left corner of the viewport.
         * @param width New width.
         * @param height New height.
         */
        static void set_viewport(const uint32_t x, uint32_t y, const uint32_t width, const uint32_t height);
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_GL_RENDERER_API_HPP
