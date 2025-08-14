//
// Created by cjm036653 on 14/08/2025.
//

#ifndef SRC_REDWOLF_DEBUG_LAYER_HPP
#define SRC_REDWOLF_DEBUG_LAYER_HPP

#include "../engine/layer.hpp"

namespace rw::ui {
    class DebugLayer : public rw::engine::Layer {
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
        double time_{ 0.0F }; // Time since the last frame
    };
} // namespace rw::ui

#endif // SRC_REDWOLF_DEBUG_LAYER_HPP
