//
// Created by david on 12/09/2025.
//

#include "renderer_interface_2_d.hpp"
rw::gfx::RendererInterface2D::RendererInterface2D(std::shared_ptr<Renderer2D> renderer) : renderer_{ std::move(renderer) } {}

void rw::gfx::RendererInterface2D::begin_scene(const Camera& camera) {
    renderer_->begin_scene(camera);
}

void rw::gfx::RendererInterface2D::draw_quad(
    const uint64_t shader_id, const rw::math::Vec2& position, const rw::math::Vec2& size, const rw::math::Vec4& color) {
    renderer_->draw_quad(shader_id, position, size, color);
}

void rw::gfx::RendererInterface2D::draw_quad(
    const uint64_t shader_id, const rw::math::Vec3& position, const rw::math::Vec2& size, const rw::math::Vec4& color) {
    renderer_->draw_quad(shader_id, position, size, color);
}

void rw::gfx::RendererInterface2D::end_scene() {
    renderer_->end_scene();
}