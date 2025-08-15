//
// Created by cjm036653 on 14/08/2025.
//

#ifndef SRC_REDWOLF_DEBUG_LAYER_HPP
#define SRC_REDWOLF_DEBUG_LAYER_HPP

#include "../evt/application_event.hpp"
#include "../evt/key_event.hpp"
#include "../evt/mouse_event.hpp"
#include "layer.hpp"

#include <imgui/imgui.h>

namespace rw::layers {
    class DebugLayer : public Layer {
     public:
        /**
         * @brief Constructor.
         * @param id Layer ID.
         */
        DebugLayer(const Layer::ID id);

        /**
         * @brief Destructor.
         */
        ~DebugLayer() override = default;

        /**
         * @brief Copy constructor.
         */
        DebugLayer(const DebugLayer&) = delete;

        /**
         * @brief Copy assignment operator.
         */
        DebugLayer& operator=(const DebugLayer&) = delete;

        /**
         * @brief Move constructor.
         */
        DebugLayer(DebugLayer&&) = default;

        /**
         * @brief Move assignment operator.
         */
        DebugLayer& operator=(DebugLayer&&) = default;

        /**
         * @brief Attach to the layer stack.
         */
        void attach() override;

        /**
         * @brief Detach from the layer stack.
         */
        void detach() override;

        /**
         * @brief Update the layer (called each frame).
         */
        void update() override;

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event) override;

     private:
        /**
         * @brief Convert a GLFW keycode to an ImGui key.
         * @param keycode GLFW keycode to convert.
         * @return ImGUi key corresponding to the input keycode.
         */
        [[nodiscard]] static ImGuiKey imgui_key_(const rw::input::Key keycode);

        /**
         * @brief Handle KeyPressed events.
         * @param event Event to handle.
         */
        void on_key_pressed_event_(const rw::evt::KeyPressedEvent& event);

        /**
         * @brief Handle KeyReleased events.
         * @param event Event to handle.
         */
        void on_key_released_event_(const rw::evt::KeyReleasedEvent& event);

        /**
         * @brief Handle KeyTyped events.
         * @param event Event to handle.
         */
        void on_key_typed_event_(const rw::evt::KeyTypedEvent& event);

        /**
         * @brief Handle MouseButtonPressed events.
         * @param event Event to handle.
         */
        void on_mouse_button_pressed_event_(const rw::evt::MouseButtonPressedEvent& event);

        /**
         * @brief Handle MouseButtonReleased events.
         * @param event Event to handle.
         */
        void on_mouse_button_released_event_(const rw::evt::MouseButtonReleasedEvent& event);

        /**
         * @brief Handle MouseMoved events.
         * @param event Event to handle.
         */
        void on_mouse_moved_event_(const rw::evt::MouseMovedEvent& event);

        /**
         * @brief Handle MouseScrolled events.
         * @param event Event to handle.
         */
        void on_mouse_scrolled_event_(const rw::evt::MouseScrolledEvent& event);

        /**
         * @brief Handle WindowResized events.
         * @param event Event to handle.
         */
        void on_window_resized_event_(const rw::evt::WindowResizedEvent& event);

        /**
         * @brief Update the control key values.
         */
        static void update_control_keys_();

        double time_{ 0.0F }; // Time since the last frame
    };
} // namespace rw::layers

#endif // SRC_REDWOLF_DEBUG_LAYER_HPP
