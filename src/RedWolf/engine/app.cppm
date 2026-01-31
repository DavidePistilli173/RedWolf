module;

#include "Redwolf/macros.hpp"

#include <chrono>
#include <concepts>
#include <memory>
#include <ranges>

export module redwolf.engine.app;

import redwolf.common;
import redwolf.evt.application_event;
import redwolf.evt.event;
import redwolf.gfx.renderer_interface_2_d;
import redwolf.layers.debug_layer;
import redwolf.layers.layer;
import redwolf.layers.layer_stack;
import redwolf.ui.window;
import redwolf.util.logger;

export namespace rw::engine {
    /**
     * @brief Base class for a RedWolf application. Singleton.
     */
    class App {
     public:
        /**
         * @brief Default constructor.
         */
        explicit App(const rw::ui::WindowDescriptor& window_data) {
            // Initialise the logger as first instruction.
            RW_CORE_INFO("Constructing application");

            // Ensure only one instance of the application exists.
            if (nullptr != instance_) {
                RW_CORE_FATAL("Application instance already exists. Only one instance of App is allowed.");
                return;
            }
            instance_ = this;

            // Create the main application window.
            window_ = std::make_unique<rw::ui::Window>(window_data);
            window_->set_event_callback([this](const rw::evt::Event& event) { return on_event(event); });

            renderer_interface_2d_ = window_->renderer_interface_2d();

            debug_layer_ = dynamic_cast<rw::layers::DebugLayer*>(layer_stack_.push_layer<rw::layers::DebugLayer>());
            if (nullptr == debug_layer_) {
                RW_CORE_FATAL("Failed to create debug layer.");
                return;
            }
        }

        /**
         * @brief Destructor.
         */
        virtual ~App() {
            instance_ = nullptr;
        }

        /**
         * @brief Copy-constructor.
         */
        App(const App&) = delete;

        /**
         * @brief Copy-assignment operator.
         */
        App& operator=(const App&) = delete;

        /**
         * @brief Move-constructor.
         */
        App(App&&) = delete;

        /**
         * @brief Move-assignment operator.
         */
        App& operator=(App&&) = delete;

        /**
         * @brief Get the application instance.
         * @return Application instance.
         */
        [[nodiscard]] static App& get() {
            return *instance_;
        }

        /**
         * @brief Event handler for the application.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event) {
            bool result{ false };

            switch (event.type()) {
            case rw::evt::EventType::window_resize:
                on_window_resize_(dynamic_cast<const rw::evt::WindowResizedEvent&>(event));
                result = false;
                break;
            case rw::evt::EventType::window_close:
                running_ = false;
                result   = true;
                break;
            default:
                break;
            }

            // Reverse loop to let the overlays get events before normal layers.
            for (std::ranges::reverse_view rv{ layer_stack_ }; auto& layer : rv) {
                if (layer->on_event(event)) {
                    result = true;
                    break;
                }
            }

            return result;
        }

        /**
         * @brief Create a layer and push it onto the stack.
         * @tparam T Type of layer to create.
         * @param args Arguments for layer creation (except for the ID).
         * @return Non-owning pointer to the created layer.
         */
        template<std::derived_from<rw::layers::Layer> T, typename... Args>
        [[nodiscard]] T* push_layer(Args&&... args) {
            return layer_stack_.push_layer<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Create an overlay and push it onto the stack.
         * @tparam T Type of overlay to create.
         * @param args Arguments for overlay creation (except for the ID).
         * @return Non-owning pointer to the created overlay.
         */
        template<std::derived_from<rw::layers::Layer> T, typename... Args>
        [[nodiscard]] T* push_overlay(Args&&... args) {
            return layer_stack_.push_overlay<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Run the application main loop.
         */
        void run() {
            auto last_ts{ std::chrono::high_resolution_clock::now() };

            // Main application loop.
            while (running_) {
                auto       current_ts{ std::chrono::high_resolution_clock::now() };
                const auto delta_time{ static_cast<float>((current_ts - last_ts).count()) * nanoseconds_to_seconds };

                // Don't update the layers if the application is minimised.
                if (!minimized_) {
                    for (auto& layer : layer_stack_) {
                        layer->update(delta_time);
                    }
                }

                // Always update the debug UI since it could be in a separate window.
                debug_layer_->begin_frame();
                for (auto& layer : layer_stack_) {
                    layer->render_imgui();
                }
                debug_layer_->end_frame();

                window_->update();

                last_ts = current_ts;
            }
        }

        /**
         * @brief Get the application window.
         * @return Reference to the application window.
         */
        [[nodiscard]] rw::ui::Window& window() {
            return *window_;
        }

     private:
        /**
         * @brief Handle a window resized event.
         * @param event Event to handle.
         */
        void on_window_resize_(const rw::evt::WindowResizedEvent& event) {
            if (0 == event.width && 0 == event.height) {
                minimized_ = true;
                return;
            }
            minimized_ = false;

            renderer_interface_2d_->set_viewport(0, 0, event.width, event.height);
        }

        static App* instance_; /**< Singleton instance of the application. */

        std::unique_ptr<rw::ui::Window> window_;             /**< Application window. */
        bool                            running_{ true };    /**< Flag to indicate if the application is running. */
        bool                            minimized_{ false }; /**< If true, the window is minimised. */

        std::unique_ptr<rw::gfx::RendererInterface2D> renderer_interface_2d_; /**< Interface for controlling the renderer. */

        rw::layers::LayerStack  layer_stack_;            /**< Application layer stack. */
        rw::layers::DebugLayer* debug_layer_{ nullptr }; /**< Debug layer. */
    };

    /**
     * @brief Factory function to create RedWolf application instances.
     * @details This is required to be defined by the user application.
     * @return New RedWolf application instance.
     */
    [[nodiscard]] std::unique_ptr<App> create_app();

    App* App::instance_{ nullptr };

} // namespace rw::engine
