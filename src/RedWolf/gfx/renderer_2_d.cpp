//
// Created by david on 12/09/2025.
//

#include "renderer_2_d.hpp"

#include "RedWolf/util/logger.hpp"

static constexpr std::array<uint8_t, 4> white_texture_data{ 0xFF, 0xFF, 0xFF, 0xFF };
static constexpr rw::math::Vec4         clear_color{ 1.0F, 1.0F, 0.0F, 0.0F };
static constexpr uint32_t               max_quads_per_batch{ 10000 };
static constexpr uint32_t               max_vertices_per_batch{ max_quads_per_batch * 4 };
static constexpr uint32_t               max_indices_per_batch{ max_quads_per_batch * 6 };

rw::gfx::Renderer2D::Renderer2D() {
    quad_vertex_array_ = std::make_shared<rw::gfx::VertexArray>();

    quad_vertex_buffer_ = std::make_shared<rw::gfx::VertexBuffer>();
    quad_vertex_buffer_->set_size(max_vertices_per_batch * sizeof(QuadVertex));
    quad_vertex_buffer_->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" },
                               { ShaderDataType::f32_4, "in_color" },
                               { ShaderDataType::f32_2, "in_tex_coord" } });
    quad_vertex_array_->add_vertex_buffer(quad_vertex_buffer_);

    quad_vertex_buffer_data_.reserve(max_vertices_per_batch);

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

    texture_shader_ = shader_library_.create(textured_shader_id, "../src/RedWolfSandbox/assets/shaders/texture.glsl");
    texture_shader_->bind();
    texture_shader_->set_i32("u_texture", 0);

    white_texture_ = texture_library_.create(white_texture_id, 1, 1);
    white_texture_->set_data(white_texture_data);

    RendererApi::set_clear_color(clear_color);
}

void rw::gfx::Renderer2D::begin_scene(const Camera& camera) {
    view_projection_matrix_ = camera.view_projection_matrix();
    quad_vertex_buffer_data_.clear();
}

void rw::gfx::Renderer2D::clear_screen() {
    RendererApi::clear_screen();
}

void rw::gfx::Renderer2D::draw_quad(Shader* shader, Quad quad) {
    if (nullptr == shader) {
        RW_CORE_ERR("Null draw parameter: shader");
        return;
    }

    quad_vertex_buffer_data_.emplace_back(QuadVertex{ .position = { quad.position }, .color = quad.color, .tex_coord = { 0.0F, 1.0F } });
    quad_vertex_buffer_data_.emplace_back(
        QuadVertex{ .position  = { quad.position.x, quad.position.y - quad.size.y, quad.position.z },
                    .color     = quad.color,
                    .tex_coord = { 0.0F, 0.0F } });
    quad_vertex_buffer_data_.emplace_back(
        QuadVertex{ .position  = { quad.position.x + quad.size.x, quad.position.y - quad.size.y, quad.position.z },
                    .color     = quad.color,
                    .tex_coord = { 1.0F, 0.0F } });
    quad_vertex_buffer_data_.emplace_back(
        QuadVertex{ .position  = { quad.position.x + quad.size.x, quad.position.y, quad.position.z },
                    .color     = quad.color,
                    .tex_coord = { 1.0F, 1.0F } });

    /*shader->bind();
    shader->set_mat_f32_4("u_view_projection", view_projection_matrix_);
    shader->set_mat_f32_4("u_transform", quad.transform);
    shader->set_f32("u_tiling_factor", quad.tiling_factor);

    if (nullptr != quad.texture) {
        quad.texture->bind(0);
    } else {
        white_texture_->bind(0);
    }

    quad_vertex_array_->bind();
    RendererApi::draw_indexed(quad_vertex_array_.get());*/
}

void rw::gfx::Renderer2D::end_scene() {
    // Flush the renderer.
    texture_shader_->bind();
    texture_shader_->set_mat_f32_4("u_view_projection", view_projection_matrix_);
    quad_vertex_buffer_->set_data(std::span<const QuadVertex>(quad_vertex_buffer_data_));
    quad_vertex_array_->bind();
    RendererApi::draw_indexed(quad_vertex_array_.get(), static_cast<uint32_t>((quad_vertex_buffer_data_.size() / 4) * 6));
}

rw::gfx::Shader* rw::gfx::Renderer2D::get_shader(const uint64_t id) {
    return shader_library_.get(id);
}

rw::gfx::Texture2D* rw::gfx::Renderer2D::get_texture(const uint64_t id) {
    return texture_library_.get(id);
}

rw::gfx::Texture2D* rw::gfx::Renderer2D::load_texture(const uint64_t id, const std::string& file_path) {
    return texture_library_.create(id, file_path);
}
