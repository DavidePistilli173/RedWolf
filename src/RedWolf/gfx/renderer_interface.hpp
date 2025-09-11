//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_RENDER_COMMAND_HPP
#define SRC_REDWOLF_RENDER_COMMAND_HPP

#include "gfx.hpp"
#include "renderer.hpp"

#include <future>

namespace rw::gfx {
    /**
     * @brief Interface for the renderer.
     */
    class RendererInterface {
     public:
        /**
         * @brief Constructor.
         * @param renderer Renderer instance to connect to.
         */
        explicit RendererInterface(std::shared_ptr<Renderer> renderer);

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
         * @param shader_id ID of the shader to use for rendering.
         * @param vertex_array Vertex array to draw.
         * @param transform Transformation matrix to apply to the vertices.
         */
        void draw(const uint64_t shader_id, const VertexArray* vertex_array, const rw::math::Mat4& transform);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

        /**
         * @brief Load a shader from file.
         * @param id Unique identifier that will be given to the shader.
         * @param file_path Path to the shader source code.
         * @return Future that will contain the newly created shader as soon as it is created.
         */
        [[nodiscard]] std::future<std::shared_ptr<Shader>> load_shader(const uint64_t id, const std::string& file_path);

        /**
         * @brief Set the colour to use to clear the screen.
         * @param color Colour to use when clearing the screen.
         */
        void set_clear_color(const rw::math::Vec4& color);

     private:
        std::shared_ptr<Renderer> renderer_; /**< Renderer instance this interface is connected to. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDER_COMMAND_HPP
