//
// Created by david on 12/09/2025.
//

#include "sandbox_2_d.hpp"

#include "RedWolf/util/logger.hpp"

#include <RedWolf/engine/app.hpp>

static constexpr uint64_t texture_id{ rw::gfx::Renderer2D::max_reserved_texture_id + 1U };

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), camera_controller_{ 1280.0F / 720.0F } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface_2d();

    texture_shader_ = renderer_interface_->get_shader(rw::gfx::Renderer2D::textured_shader_id).get();
    if (nullptr == texture_shader_) {
        RW_ERR("Failed to get shader {}", rw::gfx::Renderer2D::textured_shader_id);
        return;
    }

    texture_ = renderer_interface_->load_texture(texture_id, "../src/RedWolfSandbox/assets/textures/checkerboard.png").get();
    if (nullptr == texture_) {
        RW_ERR("Failed to get texture {}", texture_id);
        return;
    }
}

void Sandbox2D::attach() {}

void Sandbox2D::detach() {}

void Sandbox2D::render_imgui() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square colour.", rw::math::value_ptr(square_color_));
    ImGui::End();
}

void Sandbox2D::update(const float delta_time) {
    static const rw::math::Mat4 first_transform{ rw::math::build_2d_transform({ -1.0F, 0.0F, 0.0F }, 0.0F, { 0.8F, 0.8F }) };
    static const rw::math::Mat4 second_transform{ rw::math::build_2d_transform({ 0.5F, -0.5F, 0.0F }, 0.0F, { 0.5F, 0.75F }) };
    static const rw::math::Mat4 third_transform{ rw::math::build_2d_transform({ 0.5F, 0.5F, 0.0F }, 45.0F, { 0.55F, 0.87F }) };
    static const rw::math::Mat4 textured_transform{ rw::math::build_2d_transform({ 0.0F, 0.0F, -0.1F }, -30.0F, { 10.0F, 10.0F }) };

    // Update
    camera_controller_.update(delta_time);

    // Render
    renderer_interface_->clear_screen();
    renderer_interface_->begin_scene(camera_controller_.camera());
    renderer_interface_->draw_quad(texture_shader_, first_transform, {}, square_color_);
    renderer_interface_->draw_quad(texture_shader_, second_transform, {}, std::optional(rw::math::Vec4(0.2F, 0.3F, 0.8F, 1.0F)));
    renderer_interface_->draw_quad(texture_shader_, third_transform, {}, std::optional(rw::math::Vec4(0.2F, 0.8F, 0.3F, 1.0F)));
    renderer_interface_->draw_quad(texture_shader_, textured_transform, texture_, std::optional(rw::math::Vec4(0.2F, 0.2F, 0.8F, 1.0F)));
    renderer_interface_->end_scene();
}

bool Sandbox2D::on_event(const rw::evt::Event& event) {
    return camera_controller_.on_event(event);
}