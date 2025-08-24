//
// Created by cjm036653 on 10/08/25.
//

#ifndef APP_HPP
#define APP_HPP

#include "RedWolf/evt/event.hpp"
#include "RedWolf/gfx/camera.hpp"
#include "RedWolf/gfx/gfx.hpp"
#include "RedWolf/layers/debug_layer.hpp"
#include "RedWolf/layers/layer_stack.hpp"
#include "RedWolf/ui/window.hpp"

#include <concepts>
#include <memory>

namespace rw::engine {
    /**
     * @brief Base class for a RedWolf application. Singleton.
     */
    class App {
     public:
        /**
         * @brief Default constructor.
         */
        explicit App(const rw::ui::WindowDescriptor& window_data);

        /**
         * @brief Destructor.
         */
        virtual ~App();

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
        [[nodiscard]] static App& get();

        /**
         * @brief Event handler for the application.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event);

        /**
         * @brief Create a layer and push it onto the stack.
         * @tparam T Type of layer to create.
         * @param args Arguments for layer creation (except for the ID).
         * @return Non-owning pointer to the created layer.
         */
        template<std::derived_from<rw::layers::Layer> T, typename... Args>
        [[nodiscard]] rw::layers::Layer* push_layer(Args&&... args) {
            return layer_stack_.push_layer<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Create an overlay and push it onto the stack.
         * @tparam T Type of overlay to create.
         * @param args Arguments for overlay creation (except for the ID).
         * @return Non-owning pointer to the created overlay.
         */
        template<std::derived_from<rw::layers::Layer> T, typename... Args>
        [[nodiscard]] rw::layers::Layer* push_overlay(Args&&... args) {
            return layer_stack_.push_overlay<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Run the application main loop.
         */
        void run();

        /**
         * @brief Get the application window.
         * @return Reference to the application window.
         */
        [[nodiscard]] rw::ui::Window& window();

     private:
        static App* instance_; /**< Singleton instance of the application. */

        std::unique_ptr<rw::ui::Window> window_;          /**< Application window. */
        bool                            running_{ true }; /**< Flag to indicate if the application is running. */

        rw::layers::LayerStack  layer_stack_;            /**< Application layer stack. */
        rw::layers::DebugLayer* debug_layer_{ nullptr }; /**< Debug layer. */

        std::unique_ptr<rw::gfx::RendererInterface> renderer_interface_; /**< Interface to the renderer. */

        std::unique_ptr<rw::gfx::VertexArray> vertex_array_;
        std::unique_ptr<rw::gfx::Shader>      shader_;

        std::unique_ptr<rw::gfx::VertexArray> square_va_;

        rw::gfx::Camera camera_;
    };

    /**
     * @brief Factory function to create RedWolf application instances.
     * @details This is required to be defined by the user application.
     * @return New RedWolf application instance.
     */
    [[nodiscard]] std::unique_ptr<App> create_app();

} // namespace rw::engine

#endif // APP_HPP
