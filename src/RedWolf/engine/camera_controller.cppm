module;

#include <algorithm>
#include <cstdint>

export module redwolf.engine.camera_controller;

import redwolf.gfx.camera;
import redwolf.evt.event;
import redwolf.evt.application_event;
import redwolf.evt.mouse_event;
import redwolf.input.keyboard;
import redwolf.input.polling;
import redwolf.core.math;

export namespace rw::engine {
    /**
     * @brief Wrapper around a camera that also handles input and screen resolution.
     */
    class CameraController {
     public:
        static constexpr rw::core::Vec3 zero_velocity{ 0.0 };
        static constexpr float          max_zoom_level{ 0.75F };
        static constexpr float          zoom_scaling{ 0.25F };
        static constexpr float          speed_scaling{ 300.0F }; /**< Speed scaling factor for camera movement. */

        /**
         * @brief Constructor.
         * @param width Width of the viewport.
         * @param height Height of the viewport.
         * @param movement_enabled If true, enables movement input control. Otherwise, the camera position will be fixed.
         * @param rotation_enabled If true, enables rotation input control. Otherwise, the camera orientation will be fixed.
         * @param zoom_enabled If true, enables zoom input control. Otherwise, the camera zoom level will be fixed.
         */
        explicit CameraController(
            const uint32_t width,
            const uint32_t height,
            const bool     movement_enabled,
            const bool     rotation_enabled,
            const bool     zoom_enabled) :
            width_{ width }, height_{ height }, aspect_ratio_{ static_cast<float>(width) / static_cast<float>(height) },
            movement_enabled_{ movement_enabled }, rotation_enabled_{ rotation_enabled }, zoom_enabled_{ zoom_enabled } {
            camera_ = rw::gfx::Camera::orthographic(
                (1 - zoom_level_) * static_cast<float>(width_),
                static_cast<float>(width_) * zoom_level_,
                (1 - zoom_level_) * static_cast<float>(height_),
                static_cast<float>(height_) * zoom_level_);
        }

        /**
         * @brief Get a reference to the actual camera.
         * @return Reference to the actual camera.
         */
        [[nodiscard]] rw::gfx::Camera& camera() {
            return camera_;
        }

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event) {
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

        /**
         * @brief Update the controller.
         * @param delta_time Time elapsed since the last update in seconds.
         */
        void update(const float delta_time) {
            rw::core::Vec3 actual_translation_speed{ 0.0 };

            // Check translation input keys.
            if (movement_enabled_) {
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

     private:
        /**
         * @brief Handle mouse scrolled events.
         * @param event Event to handle.
         * @return true if the event was consumed, false otherwise.
         */
        [[nodiscard]] bool on_mouse_scrolled_(const rw::evt::MouseScrolledEvent& event) {
            if (zoom_enabled_) {
                zoom_level_ -= zoom_scaling * static_cast<float>(event.y_offset);
                zoom_level_ = std::max(max_zoom_level, zoom_level_);
                camera_.set_ortho_projection(
                    (1 - zoom_level_) * static_cast<float>(width_),
                    static_cast<float>(width_) * zoom_level_,
                    (1 - zoom_level_) * static_cast<float>(height_),
                    static_cast<float>(height_) * zoom_level_);
                translation_speed_ = zoom_level_ * speed_scaling; // Make the speed dependent on the zoom level.
            }

            return false;
        }

        /**
         * @brief Handle window resize events.
         * @param event Event to handle.
         * @return true if the event was consumed, false otherwise.
         */
        [[nodiscard]] bool on_window_resized_(const rw::evt::WindowResizedEvent& event) {
            width_        = event.width;
            height_       = event.height;
            aspect_ratio_ = static_cast<float>(event.width) / static_cast<float>(event.height);
            camera_.set_ortho_projection(
                (1 - zoom_level_) * static_cast<float>(width_),
                static_cast<float>(width_) * zoom_level_,
                (1 - zoom_level_) * static_cast<float>(height_),
                static_cast<float>(height_) * zoom_level_);

            return false;
        }

        uint32_t width_{ 0 };                /**< Width of the viewport. */
        uint32_t height_{ 0 };               /**< Height of the viewport. */
        float    aspect_ratio_{ 0.0F };      /**< Aspect ratio of the camera. */
        float    zoom_level_{ 1.0F };        /**< Camera zoom level. Higher values mean the camera is further out. */
        bool     movement_enabled_{ false }; /**< If true, input controls the camera movement. */
        bool     rotation_enabled_{ false }; /**< If true, input controls the camera rotation. */
        bool     zoom_enabled_{ false };     /**< If true, input controls the camera zoom level. */

        float translation_speed_{ zoom_level_ * speed_scaling }; /**< Camera translation speed. */
        float rotation_speed_{ 90.0F };                          /**< Camera rotation speed. [deg/s] */

        rw::gfx::Camera camera_; /**< Actual camera. */
    };
} // namespace rw::engine
