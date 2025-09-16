//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_RENDERER_2_D_HPP
#define SRC_REDWOLF_RENDERER_2_D_HPP

#include "../core/asset_library.hpp"
#include "camera.hpp"
#include "gfx.hpp"

#include <vector>

namespace rw::gfx {
    /**
     * @brief 2D exclusive renderer.
     */
    class Renderer2D {
     public:
        // Reserved shader IDs.
        static constexpr uint64_t flat_colored_shader_id{ 0U }; /**< ID of the flat coloured shader. */
        static constexpr uint64_t textured_shader_id{ 1U };     /**< ID of the textured shader. */
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
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen();

        /**
         * @brief Draw a quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param transform Transformation matrix of the quad.
         * @param color Colour of the quad.
         */
        void draw_quad(Shader* shader, const rw::math::Mat4& transform, const rw::math::Vec4& color);

        /**
         * @brief Draw a textured quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param transform Transformation matrix of the quad.
         * @param texture Texture for the quad.
         */
        void draw_quad(Shader* shader, const rw::math::Mat4& transform, Texture2D* texture);

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene();

        /**
         * @brief Get a shader that was previously loaded by the renderer.
         * @param id ID of the loaded shader.
         * @return Pointer to the loaded shader, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] Shader* get_shader(const uint64_t id);

        /**
         * @brief Get a texture that was previously loaded by the renderer.
         * @param id ID of the loaded texture.
         * @return Pointer to the loaded texture, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] Texture2D* get_texture(const uint64_t id);

        /**
         * @brief Load a texture from file.
         * @param id ID of the texture to load. If the ID already exists, the texture is replaced.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] Texture2D* load_texture(const uint64_t id, const std::string& file_path);

     private:
        rw::core::AssetLibrary<Shader>    shader_library_;  /**< Collection of all loaded shaders. */
        rw::core::AssetLibrary<Texture2D> texture_library_; /**< Collection of all loaded textures. */

        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */

        std::shared_ptr<VertexArray> quad_vertex_array_;
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_2_D_HPP
