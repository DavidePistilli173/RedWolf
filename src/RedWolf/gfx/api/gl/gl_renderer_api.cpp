//
// Created by cjm036653 on 21/08/2025.
//

#include "gl_renderer_api.hpp"

#include <glad/glad.h>

void rw::gfx::api::gl::RendererApi::clear_screen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rw::gfx::api::gl::RendererApi::draw_indexed(const VertexArray* vertex_array) {
    glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
}

void rw::gfx::api::gl::RendererApi::set_clear_color(const rw::math::Vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}