module;

#include "RedWolf/macros.hpp"
#include "msdf-atlas-gen/FontGeometry.h"

#include <array>
#include <format>
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
import redwolf.gfx.font;
import redwolf.gfx.font_manager;
import redwolf.gfx.framebuffer;
import redwolf.gfx.framebuffer_descriptor;
import redwolf.gfx.framebuffer_manager;
import redwolf.gfx.index_buffer;
import redwolf.gfx.quad;
import redwolf.gfx.renderer_api;
import redwolf.gfx.shader;
import redwolf.gfx.shader_data;
import redwolf.gfx.shader_manager;
import redwolf.gfx.text;
import redwolf.gfx.texture2d;
import redwolf.gfx.texture2d_manager;
import redwolf.gfx.vertex_array;
import redwolf.gfx.vertex_buffer;
import redwolf.math;
import redwolf.util.logger;

export namespace rw::gfx {
#pragma pack(push, 1)
    /**
     * @brief Vertex data for a quad on the GPU.
     */
    struct QuadVertex {
        rw::math::Vec3 position{ 0.0F };      /**< Position of the vertex. */
        rw::math::Vec4 color{ 0.0F };         /**< Colour of the vertex. */
        rw::math::Vec2 tex_coord{ 0.0F };     /**< Texture coordinate of the vertex. */
        float          tex_index{ 0.0F };     /**< Index of the texture to use for this vertex. */
        float          tiling_factor{ 1.0F }; /**< Tiling factor of the texture. */
    };

    /**
     * @brief Vertex data for a text glyph on the GPU.
     */
    struct TextVertex {
        rw::math::Vec3 position{ 0.0F };  /**< Position of the vertex. */
        rw::math::Vec4 color{ 0.0F };     /**< Colour of the vertex. */
        rw::math::Vec2 tex_coord{ 0.0F }; /**< Texture coordinate of the vertex. */
    };
#pragma pack(pop)

    /**
     * @brief Statistics for the renderer 2D.
     */
    struct Renderer2DStats {
        uint32_t draw_calls{ 0 };
        uint32_t quad_count{ 0 };
        uint32_t glyph_count{ 0 };
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

        /**
         * @brief List of default shaders.
         */
        enum class DefaultShader {
            quad_2d, /**< Basic shader for drawing 2D quads. */
            text_2d  /**< Basic shader for drawing text. */
        };

        /**
         * @brief List of default textures.
         */
        enum class DefaultTexture {
            white /**< White texture. */
        };

        /**
         * @brief List of default fonts.
         */
        enum class DefaultFont {
            cmu,           /**< CMU font. */
            courier_prime, /**< Courier Prime font. */
            zector         /**< Zector font. */
        };

        /**
         * @brief Constructor.
         */
        Renderer2D() {
            init_quad_data_();
            init_text_data_();
            load_basic_assets_();

            quad_texture_slots_.reserve(max_texture_slots);
            RendererApi::set_clear_color(clear_color);
        }

        /**
         * @brief Begin rendering a scene.
         * @param camera Active camera for the scene.
         */
        void begin_scene(const Camera& camera) {
            view_projection_matrix_ = camera.view_projection_matrix();
            quad_vertex_buffer_data_.clear();
            text_vertex_buffer_data_.clear();
            quad_texture_slots_.clear();
            quad_texture_slots_.emplace_back(white_texture_); // The white texture is always available.
        }

        /**
         * @begin Bind a framebuffer for rendering.
         * @param framebuffer_handle Handle of the framebuffer to bind.
         */
        void bind_framebuffer(const Handle<Framebuffer> framebuffer_handle) const {
            framebuffer_manager_.bind(framebuffer_handle);
        }

        /**
         * @brief Clone the data of a single framebuffer.
         * @param framebuffer_handle Handle of the framebuffer to clone.
         * @return Framebuffer data.
         */
        [[nodiscard]] Framebuffer clone_framebuffer(const Handle<Framebuffer> framebuffer_handle) {
            return framebuffer_manager_.clone_framebuffer(framebuffer_handle);
        }

        /**
         * @brief Compute a sub-region of the texture.
         * @param texture_handle Handle to the texture.
         * @param region Region of the texture to compute, in pixels.
         * @return Texture coordinates of the sub-region.
         */
        [[nodiscard]] rw::gfx::Texture2D::SubRegion
            compute_texture_subregion(const Handle<Texture2D> texture_handle, const rw::math::Rect<float>& region) const {
            return texture_manager_.compute_subregion(texture_handle, region);
        }

        /**
         * @brief Clear the screen with the currently set colour.
         */
        void clear_screen() {
            RendererApi::clear_screen();
        }

        /**
         * @brief Create a framebuffer with the given descriptor.
         * @param descriptor Framebuffer creation options.
         * @return Non-owning pointer to the created framebuffer.
         */
        [[nodiscard]] Handle<Framebuffer> create_framebuffer(const FramebufferDescriptor& descriptor) {
            return framebuffer_manager_.new_from_descriptor(descriptor);
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

            for (size_t i{ 0U }; i < 4U; ++i) {
                quad_vertex_buffer_data_.emplace_back(
                    QuadVertex{ .position      = transform * quad_vertice_positions_[i],
                                .color         = quad.color,
                                .tex_coord     = tex_coords[i],
                                .tex_index     = texture_index,
                                .tiling_factor = quad.tiling_factor });
            }

            ++temp_stats_.quad_count;
        }

        /**
         * @brief Draw a string to the screen.
         * @param shader Shader to use for rendering.
         * @param text Text data to render.
         */
        void draw_text(rw::Handle<Shader> shader, Text text) {
            if (shader.invalid()) {
                RW_CORE_ERR("Null draw parameter: shader, while drawing string: {}", text.string);
                return;
            }

            if (text.font.invalid()) {
                RW_CORE_ERR("Null draw parameter: font, while drawing string: {}", text.string);
                return;
            }

            if (active_font_ != text.font || max_vertices_per_batch <= text_vertex_buffer_data_.size()) {
                flush_and_reset_();
                active_font_ = text.font;
            }

            auto font_raw_res{ font_manager_.unsafe_get_raw(text.font) };
            if (!font_raw_res.has_value()) {
                RW_CORE_WARN("Invalid font specified for string {}", text.string);
                return;
            }
            auto& font_raw{ font_raw_res.value().get() };

            const auto& metrics{ font_raw.font_geometry.getMetrics() };

            const rw::math::Mat4 transform{ rw::math::translate(rw::math::Mat4(1.0F), text.position) *
                                            rw::math::rotate(rw::math::Mat4(1.0F), text.rotation, { 0.0F, 0.0F, 1.0F }) *
                                            rw::math::scale(rw::math::Mat4(1.0F), { text.pixel_size, text.pixel_size, 1.0F }) };

            double       x{ 0.0 };
            double       fs_scale{ 1.0 / metrics.ascenderY - metrics.descenderY };
            double       y{ -fs_scale * metrics.ascenderY };
            const double line_height_offset{ 0.0F };

            for (const auto [i, character] : std::views::enumerate(text.string)) {
                if ('\n' == character) {
                    x = 0.0;
                    y -= fs_scale * metrics.lineHeight + line_height_offset;
                    continue;
                }

                if ('\r' == character) {
                    x = 0.0;
                    continue;
                }

                auto glyph{ font_raw.font_geometry.getGlyph(character) };
                if (nullptr == glyph) {
                    RW_CORE_WARN(
                        "Font does not contain glyph for character: {} (raw: {}), while drawing string: {}",
                        character,
                        static_cast<uint8_t>(character),
                        text.string);
                    glyph = font_raw.font_geometry.getGlyph('?');
                    if (nullptr == glyph) {
                        RW_CORE_ERR("Font does not contain glyph for fallback character '?', while drawing string: {}", text.string);
                        return;
                    }
                }

                double atlas_left{ 0.0 };
                double atlas_bottom{ 0.0 };
                double atlas_right{ 0.0 };
                double atlas_top{ 0.0 };
                glyph->getQuadAtlasBounds(atlas_left, atlas_bottom, atlas_right, atlas_top);
                rw::math::Vec2 tex_coord_min{ static_cast<float>(atlas_left), static_cast<float>(atlas_bottom) };
                rw::math::Vec2 tex_coord_max{ static_cast<float>(atlas_right), static_cast<float>(atlas_top) };

                double plane_left{ 0.0 };
                double plane_bottom{ 0.0 };
                double plane_right{ 0.0 };
                double plane_top{ 0.0 };
                glyph->getQuadPlaneBounds(plane_left, plane_bottom, plane_right, plane_top);
                rw::math::Vec2 quad_coord_min{ static_cast<float>(plane_left), static_cast<float>(plane_bottom) };
                rw::math::Vec2 quad_coord_max{ static_cast<float>(plane_right), static_cast<float>(plane_top) };

                quad_coord_min *= fs_scale;
                quad_coord_max *= fs_scale;

                quad_coord_min += rw::math::Vec2{ static_cast<float>(x), static_cast<float>(y) };
                quad_coord_max += rw::math::Vec2{ static_cast<float>(x), static_cast<float>(y) };

                const rw::math::Vec2 atlas_texture_size{ texture_manager_.texture_size(font_raw.atlas_texture) };
                if (0.0F == atlas_texture_size.x || 0.0F == atlas_texture_size.y) {
                    RW_CORE_WARN("Invalid font atlas size {}x{} when drawing text {}.", atlas_texture_size.x, atlas_texture_size.y);
                }
                float texel_width{ 1.0F / atlas_texture_size.x };
                float texel_height{ 1.0F / atlas_texture_size.y };

                tex_coord_min *= rw::math::Vec2{ texel_width, texel_height };
                tex_coord_max *= rw::math::Vec2{ texel_width, texel_height };

                text_vertex_buffer_data_.emplace_back(
                    TextVertex{ .position  = transform * rw::math::Vec4{ quad_coord_min.x, quad_coord_min.y, 0.0F, 1.0F },
                                .color     = text.foreground_color,
                                .tex_coord = { tex_coord_min.x, tex_coord_min.y } });

                text_vertex_buffer_data_.emplace_back(
                    TextVertex{ .position  = transform * rw::math::Vec4{ quad_coord_min.x, quad_coord_max.y, 0.0F, 1.0F },
                                .color     = text.foreground_color,
                                .tex_coord = { tex_coord_min.x, tex_coord_max.y } });

                text_vertex_buffer_data_.emplace_back(
                    TextVertex{ .position  = transform * rw::math::Vec4{ quad_coord_max.x, quad_coord_max.y, 0.0F, 1.0F },
                                .color     = text.foreground_color,
                                .tex_coord = { tex_coord_max.x, tex_coord_max.y } });

                text_vertex_buffer_data_.emplace_back(
                    TextVertex{ .position  = transform * rw::math::Vec4{ quad_coord_max.x, quad_coord_min.y, 0.0F, 1.0F },
                                .color     = text.foreground_color,
                                .tex_coord = { tex_coord_max.x, tex_coord_min.y } });

                if (static_cast<size_t>(i) < (text.string.size() - 1U)) {
                    double advance{ glyph->getAdvance() };
                    char   next_char{ text.string[i + 1U] };
                    (void) font_raw.font_geometry.getAdvance(advance, character, next_char);

                    float kerning_offset{ 0.0F };
                    x += fs_scale * advance + kerning_offset;
                }

                ++temp_stats_.glyph_count;
            }
        }

        /**
         * @brief Finish rendering a scene.
         */
        void end_scene() {
            // Flush the renderer.
            flush_();
        }

        /**
         * @brief Get one of the default fonts.
         * @param font Font to get.
         * @return Handle to the specified font.
         */
        [[nodiscard]] Handle<Font> get_default_font(const DefaultFont font) {
            switch (font) {
            case DefaultFont::cmu:
                return cmu_font_;
                break;
            case DefaultFont::courier_prime:
                return courier_prime_font_;
                break;
            case DefaultFont::zector:
                return zector_font_;
                break;
            }

            return {};
        }

        /**
         * @brief Get one of the default shaders.
         * @param shader Shader to get.
         * @return Handle to the requested shader.
         */
        [[nodiscard]] Handle<Shader> get_default_shader(const DefaultShader shader) {
            switch (shader) {
            case DefaultShader::quad_2d:
                return quad_shader_;
                break;
            case DefaultShader::text_2d:
                return text_shader_;
                break;
            }

            return {};
        }

        /**
         * @brief Get one of the default textures.
         * @param texture Texture to get.
         * @return Handle to the requested texture.
         */
        [[nodiscard]] Handle<Texture2D> get_default_texture(const DefaultTexture texture) {
            switch (texture) {
            case DefaultTexture::white:
                return white_texture_;
                break;
            }

            return {};
        }

        /**
         * @brief Load a texture from file.
         * @param file_path Path where the texture image is located.
         * @return Non-owning pointer to the newly created texture.
         */
        [[nodiscard]] Handle<Texture2D> load_texture(const std::string& file_path) {
            return texture_manager_.new_from_path(file_path);
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

        /**
         * @brief Unbind any currently bound framebuffer.
         */
        void unbind_framebuffer() const {
            framebuffer_manager_.unbind();
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

            if (const auto it{
                    std::ranges::find_if(quad_texture_slots_, [texture](const auto& item) { return item.index == texture.index; }) };
                quad_texture_slots_.end() != it) {
                return static_cast<float>(std::distance(quad_texture_slots_.begin(), it));
            }

            if (quad_texture_slots_.size() >= max_texture_slots) {
                flush_();

                quad_vertex_buffer_data_.clear();
                quad_texture_slots_.clear();
                quad_texture_slots_.emplace_back(white_texture_);
            }

            quad_texture_slots_.emplace_back(texture);
            return static_cast<float>(quad_texture_slots_.size() - 1U);
        }

        /**
         * @brief Initialise the data needed to draw quads.
         */
        void init_quad_data_() {
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
         * @brief Initialise the data needed for text rendering.
         */
        void init_text_data_() {
            // Vertex array.
            text_vertex_array_ = std::make_shared<rw::gfx::VertexArray>();

            // Vertex buffer.
            text_vertex_buffer_ = std::make_shared<rw::gfx::VertexBuffer>();
            text_vertex_buffer_->set_size(max_vertices_per_batch * sizeof(TextVertex));
            text_vertex_buffer_->set_layout(
                rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" },
                                       { ShaderDataType::f32_4, "in_color" },
                                       { ShaderDataType::f32_2, "in_tex_coord" } });
            text_vertex_array_->add_vertex_buffer(text_vertex_buffer_);

            text_vertex_buffer_data_.reserve(max_vertices_per_batch);

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
            text_vertex_array_->set_index_buffer(quad_ib);
        }

        /**
         * @brief Load basic assets that are always needed by the renderer.
         */
        void load_basic_assets_() {
            // Shaders
            quad_shader_ = shader_manager_.new_from_path("engine_assets/shaders/quad_2d.glsl");
            shader_manager_.bind(quad_shader_);
            shader_manager_.set_i32_array(quad_shader_, "u_textures", texture_samplers);

            text_shader_ = shader_manager_.new_from_path("engine_assets/shaders/text_2d.glsl");
            shader_manager_.bind(text_shader_);
            shader_manager_.set_i32(text_shader_, "u_font_atlas", 0);

            // Textures
            white_texture_ = texture_manager_.new_empty();
            texture_manager_.set_data(
                white_texture_, white_texture_data, TextureDescriptor{ .width = 1, .height = 1, .format = TextureFormat::rgba8 });

            // Fonts
            cmu_font_           = font_manager_.new_from_path("engine_assets/fonts/cmu/cmunbmr.ttf", texture_manager_);
            courier_prime_font_ = font_manager_.new_from_path("engine_assets/fonts/courier_prime/courier_prime.ttf", texture_manager_);
            zector_font_        = font_manager_.new_from_path("engine_assets/fonts/zector/Zector.ttf", texture_manager_);
        }

        /**
         * @brief Flush the current batch without resetting the buffers.
         */
        void flush_() {
            // Draw quads if necessary.
            if (!quad_vertex_buffer_data_.empty()) {
                shader_manager_.bind(quad_shader_);
                shader_manager_.set_mat_f32_4(quad_shader_, "u_view_projection", view_projection_matrix_);
                quad_vertex_buffer_->set_data(std::span<const QuadVertex>(quad_vertex_buffer_data_));
                quad_vertex_array_->bind();

                // Bind all active textures.
                for (const auto [i, texture] : std::views::enumerate(quad_texture_slots_)) {
                    texture_manager_.bind(texture, static_cast<uint32_t>(i));
                }

                RendererApi::draw_indexed(quad_vertex_array_.get(), static_cast<uint32_t>((quad_vertex_buffer_data_.size() / 4) * 6));
                ++temp_stats_.draw_calls;
            }

            // Draw text if necessary.
            if (!text_vertex_buffer_data_.empty()) {
                shader_manager_.bind(text_shader_);
                shader_manager_.set_mat_f32_4(text_shader_, "u_view_projection", view_projection_matrix_);
                text_vertex_buffer_->set_data(std::span<const TextVertex>(text_vertex_buffer_data_));
                text_vertex_array_->bind();

                // Bind the active font atlas.
                font_manager_.bind_atlas(active_font_, texture_manager_);

                RendererApi::draw_indexed(text_vertex_array_.get(), static_cast<uint32_t>((text_vertex_buffer_data_.size() / 4) * 6));
                ++temp_stats_.draw_calls;
            }
        }

        /**
         * @brief End the previous rendering batch and start a new one.
         */
        void flush_and_reset_() {
            end_scene();
            quad_vertex_buffer_data_.clear();
            text_vertex_buffer_data_.clear();
            quad_texture_slots_.clear();
            quad_texture_slots_.emplace_back(white_texture_); // The white texture is always available.
        }

        ShaderManager      shader_manager_;      /**< Collection of all loaded shaders. */
        Texture2DManager   texture_manager_;     /**< Collection of all loaded textures. */
        FramebufferManager framebuffer_manager_; /**< Collection of all created framebuffers. */
        FontManager        font_manager_;        /**< Collection of all loaded fonts. */

        std::shared_ptr<VertexArray>  quad_vertex_array_;  /**< Vertex array used for drawing quads. */
        std::shared_ptr<VertexBuffer> quad_vertex_buffer_; /**< Vertex buffer used for drawing quads. */
        Handle<Shader>                quad_shader_;        /**< Shader for quads. */

        std::shared_ptr<VertexArray>  text_vertex_array_;  /**< Vertex array used for drawing text. */
        std::shared_ptr<VertexBuffer> text_vertex_buffer_; /**< Vertex buffer used for drawing text. */
        Handle<Shader>                text_shader_;        /**< Shader for text. */

        Handle<Font> cmu_font_;           /**< CMU font. */
        Handle<Font> courier_prime_font_; /**< Courier Prime font. */
        Handle<Font> zector_font_;        /**< Zector font. */

        rw::Handle<Texture2D> white_texture_; /**< Completely white texture. */

        rw::math::Mat4 view_projection_matrix_{ 1.0F }; /**< Combined view and projection matrix of the current scene. */
        /**
         * @brief Local space positions of the quad vertices.
         */
        rw::math::Vec4 quad_vertice_positions_[4]{
            { -0.5F, -0.5F, 0.0F, 1.0F }, { 0.5F, -0.5F, 0.0F, 1.0F }, { 0.5F, 0.5F, 0.0F, 1.0F }, { -0.5F, 0.5F, 0.0F, 1.0F }
        };
        std::vector<QuadVertex>        quad_vertex_buffer_data_; /**< Quad vertex buffer data. */
        std::vector<Handle<Texture2D>> quad_texture_slots_;      /**< Active textures for the current quad draw batch. */

        std::vector<TextVertex> text_vertex_buffer_data_; /**< Text vertex buffer data. */
        Handle<Font>            active_font_;             /**< Active font for the current text draw batch. */

        Renderer2DStats temp_stats_; /**< Temporary statistics for the current recording. */
        Renderer2DStats stats_;      /**< Statistics for the renderer's last recording. */
    };
} // namespace rw::gfx

/**
 * @brief std::formatter specialization for rw::gfx::Renderer2D::DefaultFont
 */
template<>
struct std::formatter<rw::gfx::Renderer2D::DefaultFont> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::gfx::Renderer2D::DefaultFont default_font, std::format_context& ctx) const {
        switch (default_font) {
        case rw::gfx::Renderer2D::DefaultFont::cmu:
            return std::format_to(ctx.out(), "cmu");
            break;
        case rw::gfx::Renderer2D::DefaultFont::courier_prime:
            return std::format_to(ctx.out(), "courier_prime");
            break;
        case rw::gfx::Renderer2D::DefaultFont::zector:
            return std::format_to(ctx.out(), "zector");
            break;
        }

        return std::format_to(ctx.out(), "UNKNOWN VALUE");
    }
};

/**
 * @brief std::formatter specialization for rw::gfx::Renderer2D::DefaultShader
 */
template<>
struct std::formatter<rw::gfx::Renderer2D::DefaultShader> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::gfx::Renderer2D::DefaultShader default_shader, std::format_context& ctx) const {
        switch (default_shader) {
        case rw::gfx::Renderer2D::DefaultShader::quad_2d:
            return std::format_to(ctx.out(), "quad_2d");
            break;
        case rw::gfx::Renderer2D::DefaultShader::text_2d:
            return std::format_to(ctx.out(), "text_2d");
            break;
        }

        return std::format_to(ctx.out(), "UNKNOWN VALUE");
    }
};

/**
 * @brief std::formatter specialization for rw::gfx::Renderer2D::DefaultTexture
 */
template<>
struct std::formatter<rw::gfx::Renderer2D::DefaultTexture> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::gfx::Renderer2D::DefaultTexture default_texture, std::format_context& ctx) const {
        switch (default_texture) {
        case rw::gfx::Renderer2D::DefaultTexture::white:
            return std::format_to(ctx.out(), "white");
            break;
        }

        return std::format_to(ctx.out(), "UNKNOWN VALUE");
    }
};
