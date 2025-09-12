//
// Created by david on 12/09/2025.
//

#include "renderer_2_d.hpp"

#include "RedWolf/util/logger.hpp"

static constexpr rw::math::Vec4 clear_color{ 1.0F, 1.0F, 0.0F, 0.0F };

rw::gfx::Renderer2D::Renderer2D() {
    quad_vertex_array_ = std::make_shared<rw::gfx::VertexArray>();

    float square_vertices[] = { -0.5F, -0.5F, 0.0F, 0.5F, -0.5F, 0.0F, 0.5F, 0.5F, 0.0F, -0.5F, 0.5F, 0.0F };
    auto  quad_vb{ std::make_shared<rw::gfx::VertexBuffer>() };
    quad_vb->set_data(square_vertices);
    quad_vb->set_layout(rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" } });
    uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
    auto     quad_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
    quad_ib->set_data(square_indices);

    quad_vertex_array_->add_vertex_buffer(quad_vb);
    quad_vertex_array_->set_index_buffer(quad_ib);

    (void) shader_library_.load(flat_colored_shader_id, "../src/RedWolfSandbox/assets/shaders/flat_colored.glsl");

    RendererApi::set_clear_color(clear_color);
}

void rw::gfx::Renderer2D::begin_scene(const Camera& camera) {
    RendererApi::clear_screen();
    view_projection_matrix_ = camera.view_projection_matrix();
}

void rw::gfx::Renderer2D::draw_quad(
    const uint64_t                         shader_id,
    [[maybe_unused]] const rw::math::Vec2& position,
    [[maybe_unused]] const rw::math::Vec2& size,
    const rw::math::Vec4&                  color) {
    auto shader{ shader_library_.get(shader_id) };
    if (!shader.has_value()) {
        RW_CORE_ERR("Drawing with non-existent shader {}", shader_id);
        return;
    }

    shader.value()->bind();
    shader.value()->upload_uniform_mat_f32_4("u_view_projection", view_projection_matrix_);
    shader.value()->upload_uniform_mat_f32_4("u_transform", rw::math::Mat4(1.0F));
    shader.value()->upload_uniform_f32_4("u_color", color);

    quad_vertex_array_->bind();
    RendererApi::draw_indexed(quad_vertex_array_.get());
}

void rw::gfx::Renderer2D::draw_quad(
    [[maybe_unused]] const uint64_t        shader_id,
    [[maybe_unused]] const rw::math::Vec3& position,
    [[maybe_unused]] const rw::math::Vec2& size,
    [[maybe_unused]] const rw::math::Vec4& color) {
    draw_quad(shader_id, { position.x, position.y }, size, color);
}

void rw::gfx::Renderer2D::end_scene() {}
