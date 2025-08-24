//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_RENDERER_HPP
#define SRC_REDWOLF_RENDERER_HPP

#include "camera.hpp"
#include "gfx.hpp"

namespace rw::gfx {
    /**
     * @brief RedWolf renderer.
     */
    class Renderer {
     public:
        /**
         * @brief Constructor.
         */
        Renderer() = default;

        /**
         * @brief Begin rendering a scene.
         * @param camera Active camera for the scene.
         */
        void begin_scene(const Camera& camera);

        /**
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen();

        /**
         * @brief Draw a set of vertices bound to a vertex array.
         * @param shader Shader to use for rendering.
         * @param vertex_array Vertex array to draw.
         */
        void draw(const Shader* shader, const VertexArray* vertex_array);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

        /**
         * @brief Set the colour to use to clear the screen.
         * @param color Colour to use when clearing the screen.
         */
        void set_clear_color(const rw::math::Vec4& color);

     private:
        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_HPP
