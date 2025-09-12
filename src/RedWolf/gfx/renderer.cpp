//
// Created by cjm036653 on 21/08/2025.
//

#include "renderer.hpp"

#include "RedWolf/util/logger.hpp"

rw::gfx::Renderer::Renderer() {
    RendererApi::init();
}

void rw::gfx::Renderer::begin_scene(const Camera& camera) {
    view_projection_matrix_ = camera.view_projection_matrix();
}

void rw::gfx::Renderer::clear_screen() {
    RendererApi::clear_screen();
}

void rw::gfx::Renderer::draw(const uint64_t shader_id, const VertexArray* vertex_array, const rw::math::Mat4& transform) {
    if (nullptr == vertex_array) {
        RW_CORE_ERR("Null draw parameter: vertex_array.");
        return;
    }

    auto shader{ shader_library_.get(shader_id) };
    if (!shader.has_value()) {
        RW_CORE_ERR("Drawing with non-existent shader {}", shader_id);
        return;
    }

    shader.value()->bind();
    shader.value()->upload_uniform_mat_f32_4("u_view_projection", view_projection_matrix_);
    shader.value()->upload_uniform_mat_f32_4("u_transform", transform);

    vertex_array->bind();
    RendererApi::draw_indexed(vertex_array);
}

void rw::gfx::Renderer::end_scene() {}

std::shared_ptr<rw::gfx::Shader> rw::gfx::Renderer::load_shader(const uint64_t id, const std::string& file_path) {
    return shader_library_.load(id, file_path);
}

void rw::gfx::Renderer::set_clear_color(const rw::math::Vec4& color) {
    RendererApi::set_clear_color(color);
}

void rw::gfx::Renderer::set_viewport(const uint32_t x, uint32_t y, const uint32_t width, const uint32_t height) {
    RendererApi::set_viewport(x, y, width, height);
}