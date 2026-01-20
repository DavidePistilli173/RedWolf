module;

#include "RedWolf/macros.hpp"

#include <array>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <vector>

export module redwolf.gfx.renderer_2_d;

import redwolf.common;
import redwolf.core.asset_library;
import redwolf.gfx.buffer_layout;
import redwolf.gfx.common;
import redwolf.gfx.camera;
import redwolf.gfx.framebuffer;
import redwolf.gfx.framebuffer_descriptor;
import redwolf.gfx.index_buffer;
import redwolf.gfx.quad;
import redwolf.gfx.renderer_api;
import redwolf.gfx.shader;
import redwolf.gfx.shader_data;
import redwolf.gfx.texture_2d;
import redwolf.gfx.vertex_array;
import redwolf.gfx.vertex_buffer;
import redwolf.math;
import redwolf.util.logger;

export namespace rw::gfx {
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
        static constexpr std::array<uint8_t, 4> white_texture_data{ 0xFF, 0xFF, 0xFF, 0xFF };
        static constexpr rw::math::Vec4         clear_color{ 0.1F, 0.1F, 0.1F, 0.0F };
        static constexpr uint32_t               max_quads_per_batch{ 10000 };
        static constexpr uint32_t               max_vertices_per_batch{ max_quads_per_batch * 4 };
        static constexpr uint32_t               max_indices_per_batch{ max_quads_per_batch * 6 };
        static constexpr uint32_t max_texture_slots{ 32U }; /**< Maximum texture slots for a single batch. TODO(PID): Query the driver. */

        static constexpr std::array<int32_t, max_texture_slots> texture_samplers{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                                                                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                                                                                  22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

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
        Renderer2D() {
            initQuadData_();
            loadBasicAssets_();

            texture_slots_.reserve(max_texture_slots);
            RendererApi::set_clear_color(clear_color);
        }

        /**
         * @brief Begin rendering a scene.
         * @param camera Active camera for the scene.
         */
        void begin_scene(const Camera& camera) {
            view_projection_matrix_ = camera.view_projection_matrix();
            quad_vertex_buffer_data_.clear();
            texture_slots_.clear();
            texture_slots_.emplace_back(white_texture_); // The white texture is always available.
        }

        /**
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen() {
            RendererApi::clear_screen();
        }

        /**
         * @brief Create a framebuffer with the given descriptor.
         * @param id ID of the framebuffer to create.
         * @param descriptor Framebuffer creation options.
         * @return Non-owning pointer to the created framebuffer.
         */
        [[nodiscard]] Handle<Framebuffer> create_framebuffer(const uint64_t id, const FramebufferDescriptor& descriptor) {
            return framebuffer_library_.create(id, descriptor);
        }

        /**
         * @brief Draw a quad to the screen, using the Z coordinate to sort the draw order.
         * @param shader Shader to use for rendering.
         * @param quad Quad data to render.
         */
        void draw_quad(rw::Handle<Shader> shader, Quad quad) {
            if (!shader.valid()) {
                RW_CORE_ERR("Null draw parameter: shader");
                return;
            }

            if (max_vertices_per_batch <= quad_vertex_buffer_data_.size()) {
                flush_and_reset_();
            }

            // Compute the texture index.
            const float texture_index{ compute_texture_index_(quad.texture) };

            // Compute the vertex attributes.
            const rw::math::Mat4 transform{ rw::math::translate(rw::math::Mat4(1.0F), quad.position) *
                                            rw::math::rotate(rw::math::Mat4(1.0F), quad.rotation, { 0.0F, 0.0F, 1.0F }) *
                                            rw::math::scale(rw::math::Mat4(1.0F), { quad.size.x, quad.size.y, 1.0F }) };

            Texture2D::SubRegion tex_coords{
                rw::math::Vec2{ 0.0F, 0.0F }, rw::math::Vec2{ 1.0F, 0.0F }, rw::math::Vec2{ 1.0F, 1.0F }, rw::math::Vec2{ 0.0F, 1.0F }
            };
            if (quad.texture_sub_region.has_value()) {
                tex_coords = quad.texture_sub_region.value();
            }

            quad_vertex_buffer_data_.emplace_back(
                QuadVertex{ .position      = transform * quad_vertice_positions_[0],
                            .color         = quad.color,
                            .tex_coord     = tex_coords[0],
                            .tex_index     = texture_index,
                            .tiling_factor = quad.tiling_factor });
            quad_vertex_buffer_data_.emplace_back(
                QuadVertex{ .position      = transform * quad_vertice_positions_[1],
                            .color         = quad.color,
                            .tex_coord     = tex_coords[1],
                            .tex_index     = texture_index,
                            .tiling_factor = quad.tiling_factor });
            quad_vertex_buffer_data_.emplace_back(
                QuadVertex{ .position      = transform * quad_vertice_positions_[2],
                            .color         = quad.color,
                            .tex_coord     = tex_coords[2],
                            .tex_index     = texture_index,
                            .tiling_factor = quad.tiling_factor });
            quad_vertex_buffer_data_.emplace_back(
                QuadVertex{ .position      = transform * quad_vertice_positions_[3],
                            .color         = quad.color,
                            .tex_coord     = tex_coords[3],
                            .tex_index     = texture_index,
                            .tiling_factor = quad.tiling_factor });

            ++temp_stats_.quad_count;
        }

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene() {
            // Flush the renderer.
            flush_();
        }

        /**
         * @brief Get a shader that was previously loaded by the renderer.
         * @param id ID of the loaded shader.
         * @return Pointer to the loaded shader, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] Handle<Shader> get_shader(const uint64_t id) {
            return shader_library_.get(id);
        }

        /**
         * @brief Get a texture that was previously loaded by the renderer.
         * @param id ID of the loaded texture.
         * @return Pointer to the loaded texture, or nullptr if it wasn't loaded.
         */
        [[nodiscard]] Handle<Texture2D> get_texture(const uint64_t id) {
            return texture_library_.get(id);
        }

        /**
         * @brief Load a texture from file.
         * @param id ID of the texture to load. If the ID already exists, the texture is replaced.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] Handle<Texture2D> load_texture(const uint64_t id, const std::string& file_path) {
            return texture_library_.create(id, file_path);
        }

        /**
         * @brief Reset the stored renderer statistics.
         */
        void reset_stats() {
            stats_                 = temp_stats_;
            temp_stats_.draw_calls = 0;
            temp_stats_.quad_count = 0;
        }

        /**
         * @brief Set a new viewport for rendering.
         * @param x X coordinate of the bottom-left corner of the viewport.
         * @param y Y coordinate of the bottom-left corner of the viewport.
         * @param width New width.
         * @param height New height.
         */
        void set_viewport(const uint32_t x, uint32_t y, const uint32_t width, const uint32_t height) {
            RendererApi::set_viewport(x, y, width, height);
        }

        /**
         * @brief Get the current renderer statistics.
         * @return Current renderer statistics.
         */
        [[nodiscard]] const Renderer2DStats& stats() const {
            return stats_;
        }

     private:
        /**
         * @brief Compute the appropriate shader texture index for the given texture.
         * @param texture Texture to compute the index for.
         * @return Shader texture index for the given texture.
         */
        [[nodiscard]] float compute_texture_index_(const Handle<Texture2D> texture) {
            if (!texture.valid()) {
                return 0.0F;
            }

            if (const auto it{ std::ranges::find_if(texture_slots_, [texture](const auto& item) { return item.id == texture.id; }) };
                texture_slots_.end() != it) {
                return static_cast<float>(std::distance(texture_slots_.begin(), it));
            }

            if (texture_slots_.size() >= max_texture_slots) {
                flush_();

                quad_vertex_buffer_data_.clear();
                texture_slots_.clear();
                texture_slots_.emplace_back(white_texture_);
            }

            texture_slots_.emplace_back(texture);
            return static_cast<float>(texture_slots_.size() - 1U);
        }

        /**
         * @brief Initialise the data needed to draw quads.
         */
        void initQuadData_() {
            // Vertex array.
            quad_vertex_array_ = std::make_shared<rw::gfx::VertexArray>();

            // Vertex buffer.
            quad_vertex_buffer_ = std::make_shared<rw::gfx::VertexBuffer>();
            quad_vertex_buffer_->set_size(max_vertices_per_batch * sizeof(QuadVertex));
            quad_vertex_buffer_->set_layout(
                rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" },
                                       { ShaderDataType::f32_4, "in_color" },
                                       { ShaderDataType::f32_2, "in_tex_coord" },
                                       { ShaderDataType::f32, "in_tex_index" },
                                       { ShaderDataType::f32, "in_tiling_factor" } });
            quad_vertex_array_->add_vertex_buffer(quad_vertex_buffer_);

            quad_vertex_buffer_data_.reserve(max_vertices_per_batch);

            // Index buffer.
            std::vector<uint32_t> quad_indices;
            quad_indices.resize(max_indices_per_batch);
            for (uint32_t i{ 0U }; i < quad_indices.size(); i += 6U) {
                const uint32_t offset{ (i / 6U) * 4U };
                quad_indices[i + 0U] = offset + 0U;
                quad_indices[i + 1U] = offset + 1U;
                quad_indices[i + 2U] = offset + 2U;

                quad_indices[i + 3U] = offset + 2U;
                quad_indices[i + 4U] = offset + 3U;
                quad_indices[i + 5U] = offset + 0U;
            }

            auto quad_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
            quad_ib->set_data(quad_indices);
            quad_vertex_array_->set_index_buffer(quad_ib);
        }

        /**
         * @brief Load basic assets that are always needed by the renderer.
         */
        void loadBasicAssets_() {
            // Shaders
            base_shader_ = shader_library_.create(base_shader_id, "shaders/base_2d.glsl");
            base_shader_->bind();
            base_shader_->set_i32_array("u_textures", texture_samplers);

            // Textures
            white_texture_ = texture_library_.create(white_texture_id);
            white_texture_->set_data(white_texture_data, TextureDescriptor{ .width = 1, .height = 1, .format = TextureFormat::rgba8 });
        }

        /**
         * @brief Flush the current batch without resetting the buffers.
         */
        void flush_() {
            base_shader_->bind();
            base_shader_->set_mat_f32_4("u_view_projection", view_projection_matrix_);
            quad_vertex_buffer_->set_data(std::span<const QuadVertex>(quad_vertex_buffer_data_));
            quad_vertex_array_->bind();

            // Bind all active textures.
            for (const auto [i, texture] : std::views::enumerate(texture_slots_)) {
                texture->bind(static_cast<uint32_t>(i));
            }

            RendererApi::draw_indexed(quad_vertex_array_.get(), static_cast<uint32_t>((quad_vertex_buffer_data_.size() / 4) * 6));
            ++temp_stats_.draw_calls;
        }

        /**
         * @brief End the previous rendering batch and start a new one.
         */
        void flush_and_reset_() {
            end_scene();
            quad_vertex_buffer_data_.clear();
            texture_slots_.clear();
            texture_slots_.emplace_back(white_texture_); // The white texture is always available.
        }

        rw::core::AssetLibrary<Shader>      shader_library_;      /**< Collection of all loaded shaders. */
        rw::core::AssetLibrary<Texture2D>   texture_library_;     /**< Collection of all loaded textures. */
        rw::core::AssetLibrary<Framebuffer> framebuffer_library_; /**< Collection of all created framebuffers. */

        std::shared_ptr<VertexArray>  quad_vertex_array_;  /**< Vertex array used for drawing quads. */
        std::shared_ptr<VertexBuffer> quad_vertex_buffer_; /**< Vertex buffer used for drawing quads. */
        rw::Handle<Shader>            base_shader_;        /**< Textured shader. */

        rw::Handle<Texture2D> white_texture_; /**< Completely white texture. */

        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */
        /**
         * @brief Local space positions of the quad vertices.
         */
        rw::math::Vec4 quad_vertice_positions_[4]{
            { -0.5F, -0.5F, 0.0F, 1.0F }, { 0.5F, -0.5F, 0.0F, 1.0F }, { 0.5F, 0.5F, 0.0F, 1.0F }, { -0.5F, 0.5F, 0.0F, 1.0F }
        };
        std::vector<QuadVertex>        quad_vertex_buffer_data_; /**< Quad vertex buffer data. */
        std::vector<Handle<Texture2D>> texture_slots_;           /**< Active textures for the current draw batch. */

        Renderer2DStats temp_stats_; /**< Temporary statistics for the current recording. */
        Renderer2DStats stats_;      /**< Statistics for the renderer's last recording. */
    };
} // namespace rw::gfx
