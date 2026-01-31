module;

#include <glfw/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

export module redwolf.layers.debug_layer;

import redwolf.layers.layer;
import redwolf.evt.event;
import redwolf.ui.common;
import redwolf.ui.window;

export namespace rw::layers {
    class DebugLayer : public Layer {
     public:
        /**
         * @brief Constructor.
         */
        DebugLayer() : Layer("DebugLayer") {}

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
        void begin_frame() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        /**
         * @brief Detach from the layer stack.
         */
        void detach() override {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        /**
         * @brief End ImGui frame.
         */
        void end_frame();

        /**
         * @brief Render ImGui UI for the layer.
         */
        void render_imgui() override {}

        /**
         * @brief Update the layer (called each frame).
         * @param delta_time Time elapsed since the last update in seconds.
         */
        void update(const float delta_time) override {}

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event) override {
            return false;
        }

     private:
        double time_{ 0.0F }; // Time since the last frame
    };
} // namespace rw::layers
