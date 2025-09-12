//
// Created by david on 12/09/2025.
//

#include "sandbox_2_d.hpp"

#include <RedWolf/engine/app.hpp>

static constexpr uint64_t flat_colored_shader_id{ 0 };

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), camera_controller_{ 1280.0F / 720.0F } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface_2d();
}

void Sandbox2D::attach() {}

void Sandbox2D::detach() {}

void Sandbox2D::render_imgui() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square colour.", rw::math::value_ptr(square_color_));
    ImGui::End();
}

void Sandbox2D::update(const float delta_time) {
    // Update
    camera_controller_.update(delta_time);

    // Render
    renderer_interface_->begin_scene(camera_controller_.camera());
    renderer_interface_->draw_quad(rw::gfx::Renderer2D::flat_colored_shader_id, { 0.0F, 0.0F }, { 1.0F, 1.0F }, square_color_);
    renderer_interface_->end_scene();
}

bool Sandbox2D::on_event(const rw::evt::Event& event) {
    return camera_controller_.on_event(event);
}