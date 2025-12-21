//
// Created by david on 12/09/2025.
//

#ifndef SRC_REDWOLF_CAMERA_CONTROLLER_HPP
#define SRC_REDWOLF_CAMERA_CONTROLLER_HPP

#include "RedWolf/core/animated.hpp"
#include "RedWolf/evt/application_event.hpp"
#include "RedWolf/evt/mouse_event.hpp"
#include "RedWolf/gfx/camera.hpp"

namespace rw::engine {
    /**
     * @brief Wrapper around a camera that also handles input and screen resolution.
     */
    class CameraController {
     public:
        static constexpr float speed_scaling{ 300.0F }; /**< Speed scaling factor for camera movement. */

        /**
         * @brief Constructor.
         * @param width Width of the viewport.
         * @param height Height of the viewport.
         * @param movement_enabled If true, enables movement input control. Otherwise, the camera position will be fixed.
         * @param rotation_enabled If true, enables rotation input control. Otherwise, the camera orientation will be fixed.
         * @param zoom_enabled If true, enables zoom input control. Otherwise, the camera zoom level will be fixed.
         */
        explicit CameraController(
            const uint32_t width, const uint32_t height, const bool movement_enabled, const bool rotation_enabled, const bool zoom_enabled);

        /**
         * @brief Get a reference to the actual camera.
         * @return Reference to the actual camera.
         */
        [[nodiscard]] rw::gfx::Camera& camera();

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event);

        /**
         * @brief Update the controller.
         * @param delta_time Time elapsed since the last update in seconds.
         */
        void update(const float delta_time);

     private:
        /**
         * @brief Handle mouse scrolled events.
         * @param event Event to handle.
         * @return true if the event was consumed, false otherwise.
         */
        [[nodiscard]] bool on_mouse_scrolled_(const rw::evt::MouseScrolledEvent& event);

        /**
         * @brief Handle window resize events.
         * @param event Event to handle.
         * @return true if the event was consumed, false otherwise.
         */
        [[nodiscard]] bool on_window_resized_(const rw::evt::WindowResizedEvent& event);

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

#endif // SRC_REDWOLF_CAMERA_CONTROLLER_HPP
