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
         */
        DebugLayer();

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
         * @brief Begin ImGui frame.
         */
        void begin_frame();

        /**
         * @brief Detach from the layer stack.
         */
        void detach() override;

        /**
         * @brief End ImGui frame.
         */
        void end_frame();

        /**
         * @brief Render ImGui UI for the layer.
         */
        void render_imgui() override;

        /**
         * @brief Update the layer (called each frame).
         * @param delta_time Time elapsed since the last update in seconds.
         */
        void update(const float delta_time) override;

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event) override;

     private:
        double time_{ 0.0F }; // Time since the last frame
    };
} // namespace rw::layers

#endif // SRC_REDWOLF_DEBUG_LAYER_HPP
