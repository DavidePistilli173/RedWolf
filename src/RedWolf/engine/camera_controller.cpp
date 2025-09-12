//
// Created by david on 12/09/2025.
//

#include "camera_controller.hpp"

#include "RedWolf/input/polling.hpp"

static constexpr rw::math::Vec3 zero_velocity{ 0.0 };
static constexpr float          max_zoom_level{ 0.25F };
static constexpr float          zoom_scaling{ 0.25F };

rw::engine::CameraController::CameraController(const float aspect_ratio, const bool rotation_enabled) :
    aspect_ratio_{ aspect_ratio }, rotation_enabled_{ rotation_enabled } {
    camera_ = rw::gfx::Camera::orthographic(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_);
}

rw::gfx::Camera& rw::engine::CameraController::camera() {
    return camera_;
}

bool rw::engine::CameraController::on_event(const rw::evt::Event& event) {
    switch (event.type()) {
    case rw::evt::EventType::mouse_scrolled:
        return on_mouse_scrolled_(dynamic_cast<const rw::evt::MouseScrolledEvent&>(event));
    case rw::evt::EventType::window_resize:
        return on_window_resized_(dynamic_cast<const rw::evt::WindowResizedEvent&>(event));
    default:
        break;
    }
    return false;
}

void rw::engine::CameraController::update(const float delta_time) {
    rw::math::Vec3 actual_translation_speed{ 0.0 };

    // Check translation input keys.
    if (rw::input::is_key_down(rw::input::Key::a)) {
        actual_translation_speed.x = -(translation_speed_ * delta_time);
    } else if (rw::input::is_key_down(rw::input::Key::d)) {
        actual_translation_speed.x = translation_speed_ * delta_time;
    }

    if (rw::input::is_key_down(rw::input::Key::s)) {
        actual_translation_speed.y = -(translation_speed_ * delta_time);
    } else if (rw::input::is_key_down(rw::input::Key::w)) {
        actual_translation_speed.y = translation_speed_ * delta_time;
    }

    // If the rotation input is enabled, check the rotation input keys.
    if (rotation_enabled_) {
        float actual_rotation_speed{ 0.0F };

        if (rw::input::is_key_down(rw::input::Key::q)) {
            actual_rotation_speed = rotation_speed_ * delta_time;
        } else if (rw::input::is_key_down(rw::input::Key::e)) {
            actual_rotation_speed = -(rotation_speed_ * delta_time);
        }

        // Handle roto-translation input.
        if (zero_velocity != actual_translation_speed && 0.0F != actual_rotation_speed) {
            camera_.roto_translate(actual_translation_speed, actual_rotation_speed);
        } else if (zero_velocity != actual_translation_speed) {
            camera_.translate(actual_translation_speed);
        } else if (0.0F != actual_rotation_speed) {
            camera_.rotate(actual_rotation_speed);
        }
    }
    // Only handle translation input.
    else if (zero_velocity != actual_translation_speed) {
        camera_.translate(actual_translation_speed);
    }
}

bool rw::engine::CameraController::on_mouse_scrolled_(const rw::evt::MouseScrolledEvent& event) {
    zoom_level_ -= zoom_scaling * static_cast<float>(event.y_offset);
    zoom_level_ = std::max(max_zoom_level, zoom_level_);
    camera_.set_ortho_projection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_);
    translation_speed_ = zoom_level_; // Make the speed dependent on the zoom level.
    return false;
}

bool rw::engine::CameraController::on_window_resized_(const rw::evt::WindowResizedEvent& event) {
    aspect_ratio_ = static_cast<float>(event.width) / static_cast<float>(event.height);
    camera_.set_ortho_projection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_);
    return false;
}