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
         */
        static void draw_indexed(const VertexArray* vertex_array);

        /**
         * @brief Set the colour to use to clear the screen.
         * @param color Colour to use when clearing the screen.
         */
        static void set_clear_color(const rw::math::Vec4& color);
    };
} // namespace rw::gfx::api::gl

#endif // SRC_REDWOLF_GL_RENDERER_API_HPP
