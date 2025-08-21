//
// Created by cjm036653 on 21/08/2025.
//

#include "renderer.hpp"

void rw::gfx::Renderer::begin_scene() {}

void rw::gfx::Renderer::clear_screen() {
    RendererApi::clear_screen();
}

void rw::gfx::Renderer::draw_indexed(const VertexArray* vertex_array) {
    vertex_array->bind();
    RendererApi::draw_indexed(vertex_array);
}

void rw::gfx::Renderer::end_scene() {}

void rw::gfx::Renderer::set_clear_color(const rw::math::Vec4& color) {
    RendererApi::set_clear_color(color);
}