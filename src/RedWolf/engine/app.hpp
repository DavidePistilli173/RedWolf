//
// Created by cjm036653 on 10/08/25.
//

#ifndef APP_HPP
#define APP_HPP

#include "../evt/event.hpp"
#include "../ui/window.hpp"
#include "layer_stack.hpp"

#include <memory>

namespace rw::engine {
    /**
     * @brief Base class for a RedWolf applications.
     */
    class App {
     public:
        /**
         * @brief Default constructor.
         */
        App();

        /**
         * @brief Destructor.
         */
        virtual ~App() = default;

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
         * @brief Event handler for the application.
         */
        [[nodiscard]] bool on_event(const rw::evt::Event& event);

        /**
         * @brief Create a layer and push it onto the stack.
         * @param args Arguments for layer creation (except for the ID).
         * @return ID of the created layer.
         */
        template<typename... Args>
        [[nodiscard]] Layer::ID push_layer(Args&&... args) {
            return layer_stack_.push_layer(std::forward<Args>(args)...);
        }

        /**
         * @brief Create an overlay and push it onto the stack.
         * @param args Arguments for overlay creation (except for the ID).
         * @return ID of the created overlay.
         */
        template<typename... Args>
        [[nodiscard]] Layer::ID push_overlay(Args&&... args) {
            return layer_stack_.push_overlay(std::forward<Args>(args)...);
        }

        /**
         * @brief Run the application main loop.
         */
        void run();

     private:
        std::unique_ptr<rw::ui::Window> window_;          /**< Application window. */
        bool                            running_{ true }; /**< Flag to indicate if the application is running. */

        LayerStack layer_stack_; /**< Application layer stack. */
    };

    /**
     * @brief Factory function to create RedWolf application instances.
     * @details This is required to be defined by the user application.
     * @return New RedWolf application instance.
     */
    [[nodiscard]] std::unique_ptr<App> create_app();

} // namespace rw::engine

#endif // APP_HPP
