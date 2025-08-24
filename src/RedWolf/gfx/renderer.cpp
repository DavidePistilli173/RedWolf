//
// Created by cjm036653 on 21/08/2025.
//

#include "renderer.hpp"

#include "RedWolf/util/logger.hpp"

void rw::gfx::Renderer::begin_scene(const Camera& camera) {
    view_projection_matrix_ = camera.view_projection_matrix();
}

void rw::gfx::Renderer::clear_screen() {
    RendererApi::clear_screen();
}

void rw::gfx::Renderer::draw(const Shader* shader, const VertexArray* vertex_array) {
    if (nullptr == shader || nullptr == vertex_array) {
        RW_CORE_ERR("Null draw parameters: shader: {}, vertex_array: {}", (nullptr == shader), (nullptr == vertex_array));
        return;
    }

    shader->bind();
    shader->upload_uniform_mat4("u_view_projection", view_projection_matrix_);

    vertex_array->bind();
    RendererApi::draw_indexed(vertex_array);
}

void rw::gfx::Renderer::end_scene() {}

void rw::gfx::Renderer::set_clear_color(const rw::math::Vec4& color) {
    RendererApi::set_clear_color(color);
}