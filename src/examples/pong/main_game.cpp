//
// Created by david on 12/09/2025.
//

#include "main_game.hpp"

#include "common.hpp"

#include <RedWolf/engine/app.hpp>
#include <RedWolf/gfx/font.hpp>
#include <RedWolf/input/polling.hpp>
#include <RedWolf/util/logger.hpp>

MainGame::MainGame() :
    Layer("PongMainGame"), camera_controller_{ window_settings.width, window_settings.height, false, false, false },
    font_{ "C:/Windows/Fonts/arial.ttf" } {
    renderer_interface_ = window_.renderer_interface_2d();

    base_shader_ = renderer_interface_->get_shader(rw::gfx::Renderer2D::base_shader_id).get();
    if (!base_shader_.valid()) {
        RW_ERR("Failed to get shader {}", rw::gfx::Renderer2D::base_shader_id);
        return;
    }

    background_quad_ = { .position           = { window_settings.width / 2.0F, window_settings.height / 2.0F, -0.5F },
                         .rotation           = 0.0F,
                         .size               = { window_settings.width, window_settings.height },
                         .color              = {},
                         .tiling_factor      = 1.0F,
                         .texture            = rw::Handle<rw::gfx::Texture2D>{},
                         .texture_sub_region = std::nullopt };

    mid_field_quad_ = { .position           = { window_settings.width / 2.0F, window_settings.height / 2.0F, 0.0F },
                        .rotation           = 0.0F,
                        .size               = { 10.0F, window_settings.height },
                        .color              = { 1.0F, 1.0F, 1.0F, 1.0F },
                        .tiling_factor      = 1.0F,
                        .texture            = rw::Handle<rw::gfx::Texture2D>{},
                        .texture_sub_region = std::nullopt };

    paddle_1_quad_ = { .position           = { 50.0F, window_settings.height / 2.0F, 0.0F },
                       .rotation           = 0.0F,
                       .size               = { 25.0F, 100.0F },
                       .color              = { 1.0F, 1.0F, 1.0F, 1.0F },
                       .tiling_factor      = 1.0F,
                       .texture            = rw::Handle<rw::gfx::Texture2D>{},
                       .texture_sub_region = std::nullopt };

    paddle_2_quad_ = { .position           = { window_settings.width - 50.0F, window_settings.height / 2.0F, 0.0F },
                       .rotation           = 0.0F,
                       .size               = { 25.0F, 100.0F },
                       .color              = { 1.0F, 1.0F, 1.0F, 1.0F },
                       .tiling_factor      = 1.0F,
                       .texture            = rw::Handle<rw::gfx::Texture2D>{},
                       .texture_sub_region = std::nullopt };

    auto ball_texture{ renderer_interface_->load_texture(100005U, "assets/textures/ball.png").get() };
    if (!ball_texture.valid()) {
        RW_ERR("Failed to load ball texture: {}", "assets/textures/ball.png");
        return;
    }

    ball_quad_ = { .position           = { window_settings.width / 2.0F, window_settings.height / 2.0F, 0.0F },
                   .rotation           = 0.0F,
                   .size               = { 20.0F, 20.0F },
                   .color              = { 1.0F, 1.0F, 1.0F, 1.0F },
                   .tiling_factor      = 1.0F,
                   .texture            = ball_texture,
                   .texture_sub_region = std::nullopt };
}

void MainGame::attach() {}

void MainGame::detach() {}

void MainGame::render_imgui() {
    ImGui::Begin("Test");
    ImGui::Image(font_.atlas_texture().renderer_id(), { 512, 512 }, { 0, 1 }, { 1, 0 });
    ImGui::End();
}

void MainGame::update(const float delta_time) {
    update_logic_(delta_time);
    render_();
}

bool MainGame::on_event(const rw::evt::Event& event) {
    (void) camera_controller_.on_event(event);

    if (event.type() == rw::evt::EventType::window_resize) {
        const auto& resize_event = static_cast<const rw::evt::WindowResizedEvent&>(event);

        background_quad_.size *= resize_event.scale_factor;
        background_quad_.position.x *= resize_event.scale_factor.x;
        background_quad_.position.y *= resize_event.scale_factor.y;

        mid_field_quad_.position.x *= resize_event.scale_factor.x;
        mid_field_quad_.position.y *= resize_event.scale_factor.y;
        mid_field_quad_.size *= resize_event.scale_factor;

        paddle_1_quad_.position.x *= resize_event.scale_factor.x;
        paddle_1_quad_.position.y *= resize_event.scale_factor.y;
        paddle_1_quad_.size *= resize_event.scale_factor;

        paddle_2_quad_.position.x *= resize_event.scale_factor.x;
        paddle_2_quad_.position.y *= resize_event.scale_factor.y;
        paddle_2_quad_.size *= resize_event.scale_factor;

        ball_quad_.position.x *= resize_event.scale_factor.x;
        ball_quad_.position.y *= resize_event.scale_factor.y;
        ball_quad_.size *= resize_event.scale_factor;
        ball_velocity_ *= resize_event.scale_factor;
    };

    return false;
}

void MainGame::update_ball_(const float delta_time) {
    static int reset_dir = 1; // toggles horizontal direction on reset

    // Move
    ball_quad_.position.x += ball_velocity_.x * delta_time;
    ball_quad_.position.y += ball_velocity_.y * delta_time;

    // Helpers
    const float half_ball_w = ball_quad_.size.x * 0.5F;
    const float half_ball_h = ball_quad_.size.y * 0.5F;

    const float ball_left   = ball_quad_.position.x - half_ball_w;
    const float ball_right  = ball_quad_.position.x + half_ball_w;
    const float ball_top    = ball_quad_.position.y + half_ball_h;
    const float ball_bottom = ball_quad_.position.y - half_ball_h;

    const auto check_paddle_collision = [&](const auto& paddle) -> bool {
        const float half_pw  = paddle.size.x * 0.5F;
        const float half_ph  = paddle.size.y * 0.5F;
        const float p_left   = paddle.position.x - half_pw;
        const float p_right  = paddle.position.x + half_pw;
        const float p_top    = paddle.position.y + half_ph;
        const float p_bottom = paddle.position.y - half_ph;

        return !(ball_right < p_left || ball_left > p_right || ball_top < p_bottom || ball_bottom > p_top);
    };

    // Top / bottom bounce
    if (ball_top >= window_.height()) {
        ball_quad_.position.y = window_.height() - half_ball_h;
        ball_velocity_.y      = -ball_velocity_.y;
    } else if (ball_bottom <= 0.0F) {
        ball_quad_.position.y = half_ball_h;
        ball_velocity_.y      = -ball_velocity_.y;
    }

    // Paddle collisions
    if (check_paddle_collision(paddle_1_quad_)) {
        // place ball outside paddle to avoid sticking
        const float paddle_right = paddle_1_quad_.position.x + paddle_1_quad_.size.x * 0.5F;
        ball_quad_.position.x    = paddle_right + half_ball_w + 0.1F;
        ball_velocity_.x         = std::abs(ball_velocity_.x); // ensure moving right

        // change Y velocity based on hit location
        float hit_norm = (ball_quad_.position.y - paddle_1_quad_.position.y) / (paddle_1_quad_.size.y * 0.5F);
        ball_velocity_.y += hit_norm * 200.0F;
    } else if (check_paddle_collision(paddle_2_quad_)) {
        const float paddle_left = paddle_2_quad_.position.x - paddle_2_quad_.size.x * 0.5F;
        ball_quad_.position.x   = paddle_left - half_ball_w - 0.1F;
        ball_velocity_.x        = -std::abs(ball_velocity_.x); // ensure moving left

        float hit_norm = (ball_quad_.position.y - paddle_2_quad_.position.y) / (paddle_2_quad_.size.y * 0.5F);
        ball_velocity_.y += hit_norm * 200.0F;
    }

    // Cap vertical speed to avoid extreme angles
    const float max_vy = 1000.0F;
    if (ball_velocity_.y > max_vy) ball_velocity_.y = max_vy;
    if (ball_velocity_.y < -max_vy) ball_velocity_.y = -max_vy;

    // Reset if out of horizontal bounds (score)
    if (ball_right < 0.0F || ball_left > window_.width()) {
        // toggle direction so ball serves to the side that conceded
        reset_dir             = -reset_dir;
        ball_quad_.position.x = window_.width() * 0.5F;
        ball_quad_.position.y = window_.height() * 0.5F;
        ball_velocity_.x      = default_ball_velocity.x * static_cast<float>(reset_dir);
        ball_velocity_.y      = default_ball_velocity.y * ((reset_dir > 0) ? 0.5F : -0.5F); // slight vertical bias
    }
}

void MainGame::update_logic_(const float delta_time) {
    camera_controller_.update(delta_time);
    update_paddles_(delta_time);
    update_ball_(delta_time);
}

void MainGame::update_paddles_(const float delta_time) {
    static constexpr float paddle_speed{ 600.0F };

    // Paddle 1 (W/S keys)
    if (rw::input::is_key_down(rw::input::Key::w)) {
        paddle_1_quad_.position.y += paddle_speed * delta_time;
        if (paddle_1_quad_.position.y + paddle_1_quad_.size.y / 2.0F > window_.height()) {
            paddle_1_quad_.position.y = window_.height() - paddle_1_quad_.size.y / 2.0F;
        }
    } else if (rw::input::is_key_down(rw::input::Key::s)) {
        paddle_1_quad_.position.y -= paddle_speed * delta_time;
        if (paddle_1_quad_.position.y - paddle_1_quad_.size.y / 2.0F < 0.0F) {
            paddle_1_quad_.position.y = paddle_1_quad_.size.y / 2.0F;
        }
    }

    // Paddle 2 (Up/Down arrow keys)
    if (rw::input::is_key_down(rw::input::Key::up)) {
        paddle_2_quad_.position.y += paddle_speed * delta_time;
        if (paddle_2_quad_.position.y + paddle_2_quad_.size.y / 2.0F > window_.height()) {
            paddle_2_quad_.position.y = window_.height() - paddle_2_quad_.size.y / 2.0F;
        }
    } else if (rw::input::is_key_down(rw::input::Key::down)) {
        paddle_2_quad_.position.y -= paddle_speed * delta_time;
        if (paddle_2_quad_.position.y - paddle_2_quad_.size.y / 2.0F < 0.0F) {
            paddle_2_quad_.position.y = paddle_2_quad_.size.y / 2.0F;
        }
    }
}

void MainGame::render_() {
    renderer_interface_->reset_stats();
    renderer_interface_->clear_screen();

    renderer_interface_->clear_screen();

    renderer_interface_->begin_scene(camera_controller_.camera());
    renderer_interface_->draw_quad(base_shader_, background_quad_);
    renderer_interface_->draw_quad(base_shader_, mid_field_quad_);
    renderer_interface_->draw_quad(base_shader_, paddle_1_quad_);
    renderer_interface_->draw_quad(base_shader_, paddle_2_quad_);
    renderer_interface_->draw_quad(base_shader_, ball_quad_);
    renderer_interface_->end_scene();
}