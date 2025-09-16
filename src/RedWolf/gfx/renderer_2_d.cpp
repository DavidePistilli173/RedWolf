//
// Created by david on 12/09/2025.
//

#include "renderer_2_d.hpp"

#include "RedWolf/util/logger.hpp"

static constexpr rw::math::Vec4 clear_color{ 1.0F, 1.0F, 0.0F, 0.0F };

rw::gfx::Renderer2D::Renderer2D() {
    quad_vertex_array_ = std::make_shared<rw::gfx::VertexArray>();

    float square_vertices[] = { -0.5F, -0.5F, 0.0F, 0.0F, 0.0F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.0F,
                                0.5F,  0.5F,  0.0F, 1.0F, 1.0F, -0.5F, 0.5F,  0.0F, 0.0F, 1.0F };
    auto  quad_vb{ std::make_shared<rw::gfx::VertexBuffer>() };
    quad_vb->set_data(square_vertices);
    quad_vb->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" }, { ShaderDataType::f32_2, "in_tex_coord" } });
    uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
    auto     quad_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
    quad_ib->set_data(square_indices);

    quad_vertex_array_->add_vertex_buffer(quad_vb);
    quad_vertex_array_->set_index_buffer(quad_ib);

    (void) shader_library_.load(flat_colored_shader_id, "../src/RedWolfSandbox/assets/shaders/flat_colored.glsl");

    auto* texture_shader{ shader_library_.load(textured_shader_id, "../src/RedWolfSandbox/assets/shaders/texture.glsl") };
    texture_shader->bind();
    texture_shader->set_i32("u_texture", 0);

    RendererApi::set_clear_color(clear_color);
}

void rw::gfx::Renderer2D::begin_scene(const Camera& camera) {
    view_projection_matrix_ = camera.view_projection_matrix();
}

void rw::gfx::Renderer2D::clear_screen() {
    RendererApi::clear_screen();
}

void rw::gfx::Renderer2D::draw_quad(Shader* shader, const rw::math::Mat4& transform, const rw::math::Vec4& color) {
    if (nullptr == shader) {
        RW_CORE_ERR("Null draw parameter: shader");
        return;
    }

    shader->bind();
    shader->set_mat_f32_4("u_view_projection", view_projection_matrix_);
    shader->set_mat_f32_4("u_transform", transform);
    shader->set_f32_4("u_color", color);

    quad_vertex_array_->bind();
    RendererApi::draw_indexed(quad_vertex_array_.get());
}

void rw::gfx::Renderer2D::draw_quad(Shader* shader, const rw::math::Mat4& transform, Texture2D* texture) {
    if (nullptr == shader || nullptr == texture) {
        RW_CORE_ERR("Null draw parameter: shader {} texture {}", (nullptr == shader), (nullptr == texture));
        return;
    }

    shader->bind();
    shader->set_mat_f32_4("u_view_projection", view_projection_matrix_);
    shader->set_mat_f32_4("u_transform", transform);
    texture->bind(0);

    quad_vertex_array_->bind();
    RendererApi::draw_indexed(quad_vertex_array_.get());
}

void rw::gfx::Renderer2D::end_scene() {}

rw::gfx::Shader* rw::gfx::Renderer2D::get_shader(const uint64_t id) {
    return shader_library_.get(id);
}

rw::gfx::Texture2D* rw::gfx::Renderer2D::get_texture(const uint64_t id) {
    return texture_library_.get(id);
}

rw::gfx::Texture2D* rw::gfx::Renderer2D::load_texture(const uint64_t id, const std::string& file_path) {
    return texture_library_.load(id, file_path);
}
