//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_RENDERER_2_D_HPP
#define SRC_REDWOLF_RENDERER_2_D_HPP

#include "camera.hpp"
#include "gfx.hpp"
#include "shader_library.hpp"

#include <vector>

namespace rw::gfx {
    /**
     * @brief 2D exclusive renderer.
     */
    class Renderer2D {
     public:
        // Reserved shader IDs.
        static constexpr uint64_t flat_colored_shader_id{ 0U }; /**< ID of the flat coloured shader. */
        static constexpr uint64_t actual_shader_num{ 1U };      /**< Actual amount of engine shaders. */

        /**
         * @brief Maximum number of engine-reserved shader IDs for the 2D renderer.
         */
        static constexpr uint64_t max_reserved_shader_id{ 10000U };

        /**
         * @brief Constructor.
         */
        Renderer2D();

        /**
         * @brief Begin rendering a scene.
         * @param camera Active camera for the scene.
         */
        void begin_scene(const Camera& camera);

        /**
         * @brief Draw a quad to the screen.
         * @param shader_id ID of the shader to use for rendering.
         * @param position Position of the quad.
         * @param size Size of the quad.
         * @param color Colour of the quad.
         */
        void draw_quad(const uint64_t shader_id, const rw::math::Vec2& position, const rw::math::Vec2& size, const rw::math::Vec4& color);

        /**
         * @brief Draw a quad to the screen, using the Z coordinate to sort the draw order..
         * @param shader_id ID of the shader to use for rendering.
         * @param position Position of the quad.
         * @param size Size of the quad.
         * @param color Colour of the quad.
         */
        void draw_quad(const uint64_t shader_id, const rw::math::Vec3& position, const rw::math::Vec2& size, const rw::math::Vec4& color);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

     private:
        ShaderLibrary  shader_library_;                 /**< Collection of all loaded shaders. */
        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */

        std::shared_ptr<VertexArray> quad_vertex_array_;
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_2_D_HPP
