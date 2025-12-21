//
// Created by david on 21/12/2025.
//

//
// Created by david on 12/09/2025.
//

#include "main_game.hpp"

#include "common.hpp"

#include <RedWolf/engine/app.hpp>
#include <RedWolf/util/logger.hpp>

MainGame::MainGame() : Layer("PongMainGame"), camera_controller_{ window_settings.width, window_settings.height, false, false, false } {
    renderer_interface_ = rw::engine::App::get().window().renderer_interface_2d();

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
}

void MainGame::attach() {}

void MainGame::detach() {}

void MainGame::render_imgui() {}

void MainGame::update(const float delta_time) {
    update_logic_(delta_time);
    render_();
}

bool MainGame::on_event(const rw::evt::Event& event) {
    return camera_controller_.on_event(event);
}
void MainGame::update_logic_(const float delta_time) {
    camera_controller_.update(delta_time);
}

void MainGame::render_() {
    renderer_interface_->reset_stats();
    renderer_interface_->clear_screen();

    renderer_interface_->clear_screen();

    renderer_interface_->begin_scene(camera_controller_.camera());
    renderer_interface_->draw_quad(base_shader_, background_quad_);
    renderer_interface_->end_scene();
}