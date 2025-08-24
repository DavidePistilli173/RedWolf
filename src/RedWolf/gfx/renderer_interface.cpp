//
// Created by cjm036653 on 21/08/2025.
//

#include "renderer_interface.hpp"

rw::gfx::RendererInterface::RendererInterface(Renderer& renderer) : renderer_{ renderer } {}

void rw::gfx::RendererInterface::begin_scene(const Camera& camera) {
    renderer_.begin_scene(camera);
}

void rw::gfx::RendererInterface::clear_screen() {
    renderer_.clear_screen();
}

void rw::gfx::RendererInterface::draw(const Shader* shader, const VertexArray* vertex_array) {
    renderer_.draw(shader, vertex_array);
}

void rw::gfx::RendererInterface::end_scene() {
    renderer_.end_scene();
}

void rw::gfx::RendererInterface::set_clear_color(const rw::math::Vec4& color) {
    renderer_.set_clear_color(color);
}
