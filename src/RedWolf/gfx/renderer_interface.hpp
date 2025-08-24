//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_RENDER_COMMAND_HPP
#define SRC_REDWOLF_RENDER_COMMAND_HPP

#include "gfx.hpp"
#include "renderer.hpp"

namespace rw::gfx {
    /**
     * @brief Interface for the renderer.
     */
    class RendererInterface {
     public:
        /**
         * @brief Constructor.
         */
        explicit RendererInterface(Renderer& renderer);

        /**
         * @brief Begin rendering a scene.
         * @param Active camera for the scene.
         */
        void begin_scene(const Camera& camera);

        /**
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen();

        /**
         * @brief Drawa set of vertices bound to a vertex array.
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
        Renderer& renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDER_COMMAND_HPP
