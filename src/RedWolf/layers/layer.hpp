//
// Created by cjm036653 on 14/08/2025.
//

#ifndef SRC_REDWOLF_LAYER_HPP
#define SRC_REDWOLF_LAYER_HPP

#include "RedWolf/evt/event.hpp"

#include <string>
#include <string_view>

namespace rw::layers {
    /**
     * @brief Logic/graphics layer of the application.
     */
    class Layer {
     public:
        /**
         * @brief Constructor.
         * @param name Readable name of the layer.
         */
        explicit Layer(const std::string_view name);

        /**
         * @brief Destructor.
         */
        virtual ~Layer() = default;

        /**
         * @brief Copy constructor.
         */
        Layer(const Layer&) = delete;

        /**
         * @brief Copy assignment operator.
         */
        Layer& operator=(const Layer&) = delete;

        /**
         * @brief Move constructor.
         */
        Layer(Layer&&) = default;

        /**
         * @brief Move assignment operator.
         */
        Layer& operator=(Layer&&) = default;

        /**
         * @brief Attach to the layer stack.
         */
        virtual void attach() = 0;

        /**
         * @brief Detach from the layer stack.
         */
        virtual void detach() = 0;

        /**
         * @brief Get the layer name.
         * @return Readable name of the layer.
         */
        [[nodiscard]] std::string_view name() const;

        /**
         * @brief Render ImGui UI for the layer.
         */
        virtual void render_imgui() = 0;

        /**
         * @brief Update the layer (called each frame).
         * @param delta_time Time elapsed since the last update in seconds.
         */
        virtual void update(const float delta_time) = 0;

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] virtual bool on_event(const rw::evt::Event& event) = 0;

     protected:
        std::string name_; /**< Name of the layer. */
    };
} // namespace rw::layers

#endif // SRC_REDWOLF_LAYER_HPP
