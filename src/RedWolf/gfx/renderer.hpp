//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_RENDERER_HPP
#define SRC_REDWOLF_RENDERER_HPP

#include "camera.hpp"
#include "gfx.hpp"
#include "shader_library.hpp"

namespace rw::gfx {
    /**
     * @brief RedWolf renderer.
     */
    class Renderer {
     public:
        /**
         * @brief Constructor.
         */
        Renderer();

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
         * @return Newly created shader.
         */
        [[nodiscard]] std::shared_ptr<Shader> load_shader(const uint64_t id, const std::string& file_path);

        /**
         * @brief Set the colour to use to clear the screen.
         * @param color Colour to use when clearing the screen.
         */
        void set_clear_color(const rw::math::Vec4& color);
        /**
         * @brief Set a new viewport for rendering.
         * @param x X coordinate of the bottom-left corner of the viewport.
         * @param y Y coordinate of the bottom-left corner of the viewport.
         * @param width New width.
         * @param height New height.
         */
        void set_viewport(const uint32_t x, uint32_t y, const uint32_t width, const uint32_t height);

     private:
        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */
        ShaderLibrary  shader_library_;                 /**< Collection of all loaded shaders. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_HPP
