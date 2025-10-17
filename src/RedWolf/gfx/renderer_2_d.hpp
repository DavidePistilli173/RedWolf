//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_RENDERER_2_D_HPP
#define SRC_REDWOLF_RENDERER_2_D_HPP

#include "../core/asset_library.hpp"
#include "camera.hpp"
#include "gfx.hpp"
#include "quad.hpp"

#include <array>
#include <optional>

namespace rw::gfx {
    /**
     * @brief Statistics for the renderer 2D.
     */
    struct Renderer2DStats {
        uint32_t draw_calls{ 0 };
        uint32_t quad_count{ 0 };
    };

    /**
     * @brief 2D exclusive renderer.
     */
    class Renderer2D {
     public:
        // Reserved shader IDs.
        static constexpr uint64_t base_shader_id{ 0U }; /**< ID of the base 2D shader. */

        /**
         * @brief Maximum number of engine-reserved shader IDs for the 2D renderer.
         */
        static constexpr uint64_t max_reserved_shader_id{ 10000U };

        // Reserved texture IDs.
        static constexpr uint64_t white_texture_id{ 0U }; /**< ID of the white texture. */

        /**
         * @brief Maximum number of engine-reserved texture IDs for the 2D renderer.
         */
        static constexpr uint64_t max_reserved_texture_id{ 10000U };

        // Reserved framebuffer IDs.

        /**
         * @brief Maximum number of engine-reserved framebuffer IDs for the 2D renderer.
         */
        static constexpr uint64_t max_reserved_framebuffer_id{ 10000U };

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
         * @brief Create a framebuffer with the given descriptor.
         * @param id ID of the framebuffer to create.
         * @param descriptor Framebuffer creation options.
         * @return Non-owning pointer to the created framebuffer.
         */
        [[nodiscard]] Framebuffer* create_framebuffer(const uint64_t id, const FramebufferDescriptor& descriptor);

        /**
         * @brief Draw a quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param quad Quad data to render.
         */
        void draw_quad(Shader* shader, Quad quad);

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

        /**
         * @brief Reset the stored renderer statistics.
         */
        void reset_stats();

        /**
         * @brief Get the current renderer statistics.
         * @return Current renderer statistics.
         */
        [[nodiscard]] const Renderer2DStats& stats() const;

     private:
        /**
         * @brief Compute the appropriate shader texture index for the given texture.
         * @param texture Texture to compute the index for.
         * @return Shader texture index for the given texture.
         */
        [[nodiscard]] float compute_texture_index_(const Texture2D* texture);

        /**
         * @brief Initialise the data needed to draw quads.
         */
        void initQuadData_();

        /**
         * @brief Load basic assets that are always needed by the renderer.
         */
        void loadBasicAssets_();

        /**
         * @brief Flush the current batch without resetting the buffers.
         */
        void flush_();

        /**
         * @brief End the previous rendering batch and start a new one.
         */
        void flush_and_reset_();

        rw::core::AssetLibrary<Shader>      shader_library_;      /**< Collection of all loaded shaders. */
        rw::core::AssetLibrary<Texture2D>   texture_library_;     /**< Collection of all loaded textures. */
        rw::core::AssetLibrary<Framebuffer> framebuffer_library_; /**< Collection of all created framebuffers. */

        std::shared_ptr<VertexArray>  quad_vertex_array_;      /**< Vertex array used for drawing quads. */
        std::shared_ptr<VertexBuffer> quad_vertex_buffer_;     /**< Vertex buffer used for drawing quads. */
        Shader*                       base_shader_{ nullptr }; /**< Textured shader. */

        rw::gfx::Texture2D* white_texture_{ nullptr }; /**< Completely white texture. */

        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */
        /**
         * @brief Local space positions of the quad vertices.
         */
        rw::math::Vec4 quad_vertice_positions_[4]{
            { -0.5F, -0.5F, 0.0F, 1.0F }, { 0.5F, -0.5F, 0.0F, 1.0F }, { 0.5F, 0.5F, 0.0F, 1.0F }, { -0.5F, 0.5F, 0.0F, 1.0F }
        };
        std::vector<QuadVertex>       quad_vertex_buffer_data_; /**< Quad vertex buffer data. */
        std::vector<const Texture2D*> texture_slots_;           /**< Active textures for the current draw batch. */

        Renderer2DStats temp_stats_; /**< Temporary statistics for the current recording. */
        Renderer2DStats stats_;      /**< Statistics for the renderer's last recording. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_RENDERER_2_D_HPP
