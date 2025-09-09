//
// Created by david on 24/08/2025.
//

#include "example_layer.hpp"

#include <RedWolf/engine/app.hpp>
#include <RedWolf/gfx/gfx.hpp>
#include <RedWolf/input/polling.hpp>
#include <RedWolf/util/logger.hpp>

static constexpr int32_t texture_slot{ 0 };

ExampleLayer::ExampleLayer() : Layer("Sandbox Example Layer"), camera_{ rw::gfx::Camera::orthographic(-1.6F, 1.6F, -0.9F, 0.9F) } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface();
    renderer_interface_->set_clear_color(rw::math::Vec4(1.0F, 1.0F, 0.0F, 0.0F));

    float square_vertices[] = { -0.5F, -0.5F, 0.0F, 0.0F, 0.0F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.0F,
                                0.5F,  0.5F,  0.0F, 1.0F, 1.0F, -0.5F, 0.5F,  0.0F, 0.0F, 1.0F };

    square_va_ = std::make_shared<rw::gfx::VertexArray>();
    auto square_vb{ std::make_shared<rw::gfx::VertexBuffer>() };
    square_vb->set_data(square_vertices);
    square_vb->set_layout(
        rw::gfx::BufferLayout{ { rw::gfx::ShaderDataType::f32_3, "in_position" }, { rw::gfx::ShaderDataType::f32_2, "in_tex_coord" } });

    uint32_t square_indices[] = { 0, 1, 2, 2, 3, 0 };
    auto     square_ib{ std::make_shared<rw::gfx::IndexBuffer>() };
    square_ib->set_data(square_indices);

    square_va_->add_vertex_buffer(square_vb);
    square_va_->set_index_buffer(square_ib);

    shader_ = std::make_shared<rw::gfx::api::gl::Shader>(
        R"(
        #version 330 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec4 in_color;

        uniform mat4 u_view_projection;
        uniform mat4 u_transform;

        out vec3 v_position;
        out vec4 v_color;

        void main() {
            v_position = in_position;
            v_color = in_color;
            gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
        }
    )",
        R"(
        #version 330 core

        in vec3 v_position;
        in vec4 v_color;

        uniform vec4 u_color;

        layout(location = 0) out vec4 color;

        void main() {
            color = u_color * v_color * 2;
        }
    )");

    texture_shader_ = std::make_shared<rw::gfx::api::gl::Shader>(
        R"(
        #version 330 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec2 in_tex_coord;

        uniform mat4 u_view_projection;
        uniform mat4 u_transform;

        out vec2 v_tex_coord;

        void main() {
            v_tex_coord = in_tex_coord;
            gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
        }
    )",
        R"(
        #version 330 core

        in vec2 v_tex_coord;

        uniform sampler2D u_texture;

        layout(location = 0) out vec4 color;

        void main() {
            color = texture(u_texture, v_tex_coord);
        }
    )");

    texture_             = std::make_shared<rw::gfx::api::gl::Texture2D>("../src/RedWolfSandbox/assets/checkerboard.png");
    transparent_texture_ = std::make_shared<rw::gfx::api::gl::Texture2D>("../src/RedWolfSandbox/assets/ChernoLogo.png");

    texture_shader_->bind();
    texture_shader_->upload_uniform_i32("u_texture", texture_slot);
}

void ExampleLayer::attach() {}

void ExampleLayer::detach() {}

void ExampleLayer::render_imgui() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square colour.", rw::math::value_ptr(square_color_));
    ImGui::End();
}

void ExampleLayer::update(const float delta_time) {
    if (rw::input::is_key_down(rw::input::Key::a)) {
        speed_x = -1.0F;
    } else if (rw::input::is_key_down(rw::input::Key::d)) {
        speed_x = 1.0F;
    } else {
        speed_x = 0.0F;
    }

    if (rw::input::is_key_down(rw::input::Key::s)) {
        speed_y = -1.0F;
    } else if (rw::input::is_key_down(rw::input::Key::w)) {
        speed_y = 1.0F;
    } else {
        speed_y = 0.0F;
    }

    if (rw::input::is_key_down(rw::input::Key::q)) {
        rotation_speed_ = 4.0F;
    } else if (rw::input::is_key_down(rw::input::Key::e)) {
        rotation_speed_ = -4.0F;
    } else {
        rotation_speed_ = 0.0F;
    }

    if (rw::input::is_key_down(rw::input::Key::j)) {
        square_pos_.x -= 1.0F * delta_time;
    } else if (rw::input::is_key_down(rw::input::Key::l)) {
        square_pos_.x += 1.0F * delta_time;
    }

    if (rw::input::is_key_down(rw::input::Key::k)) {
        square_pos_.y -= 1.0F * delta_time;
    } else if (rw::input::is_key_down(rw::input::Key::i)) {
        square_pos_.y += 1.0F * delta_time;
    }

    camera_.roto_translate(rw::math::Vec3(speed_x, speed_y, 0.0F) * delta_time, rotation_speed_);

    renderer_interface_->clear_screen();
    renderer_interface_->begin_scene(camera_);

    shader_->bind();
    shader_->upload_uniform_f32_4("u_color", square_color_);

    texture_->bind(texture_slot);
    for (int y{ 0 }; y < 20; ++y) {
        for (int x{ 0 }; x < 20; ++x) {
            const rw::math::Mat4 transform{ rw::math::translate(
                rw::math::Mat4(1.0F), rw::math::Vec3{ static_cast<float>(x) * 0.5F, static_cast<float>(y) * 0.5F, 0.0F }) };
            renderer_interface_->draw(texture_shader_.get(), square_va_.get(), transform);
        }
    }

    // renderer_interface_->draw(shader_.get(), vertex_array_.get(), rw::math::Mat4(1.0F));
    static const rw::math::Mat4 scale{ rw::math::scale(rw::math::Mat4(1.0F), rw::math::Vec3{ 5.0F, 5.0F, 1.0F }) };
    renderer_interface_->draw(
        texture_shader_.get(),
        square_va_.get(),
        rw::math::scale(rw::math::translate(rw::math::Mat4(1.0F), square_pos_), rw::math::Vec3{ 1.2F, 1.2F, 1.0F }));

    transparent_texture_->bind(texture_slot);
    renderer_interface_->draw(
        texture_shader_.get(),
        square_va_.get(),
        rw::math::scale(rw::math::translate(rw::math::Mat4(1.0F), square_pos_), rw::math::Vec3{ 1.2F, 1.2F, 1.0F }));
    renderer_interface_->end_scene();
}

bool ExampleLayer::on_event(const rw::evt::Event& event) {
    return false;
}