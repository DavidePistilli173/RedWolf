//
// Created by cjm036653 on 21/08/2025.
//

#include "renderer_interface.hpp"

rw::gfx::RendererInterface::RendererInterface(std::shared_ptr<Renderer> renderer) : renderer_{ std::move(renderer) } {}

void rw::gfx::RendererInterface::begin_scene(const Camera& camera) {
    renderer_->begin_scene(camera);
}

void rw::gfx::RendererInterface::clear_screen() {
    renderer_->clear_screen();
}

void rw::gfx::RendererInterface::draw(Shader* shader, const VertexArray* vertex_array, const rw::math::Mat4& transform) {
    renderer_->draw(shader, vertex_array, transform);
}

void rw::gfx::RendererInterface::end_scene() {
    renderer_->end_scene();
}

std::future<rw::gfx::Shader*> rw::gfx::RendererInterface::load_shader(const uint64_t id, const std::string& file_path) {
    std::promise<rw::gfx::Shader*> promise;
    promise.set_value(renderer_->load_shader(id, file_path));
    return promise.get_future();
}

void rw::gfx::RendererInterface::set_clear_color(const rw::math::Vec4& color) {
    renderer_->set_clear_color(color);
}

void rw::gfx::RendererInterface::set_viewport(const uint32_t x, uint32_t y, const uint32_t width, const uint32_t height) {
    renderer_->set_viewport(x, y, width, height);
}
