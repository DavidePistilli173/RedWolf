//
// Created by cjm036653 on 14/08/2025.
//

#ifndef SRC_REDWOLF_LAYER_HPP
#define SRC_REDWOLF_LAYER_HPP

#include "../evt/event.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace rw::engine {
    /**
     * @brief Logic/graphics layer of the application.
     */
    class Layer {
     public:
        /**
         * @brief Unique identifier type for layers.
         */
        using ID = uint64_t;

        /**
         * @brief Constructor.
         * @param id Unique identifier for the layer.
         * @param name Readable name of the layer.
         */
        explicit Layer(const ID id, const std::string_view name);

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
         * @brief Get the ID of the layer.
         * @return ID of the layer.
         */
        [[nodiscard]] ID id() const;

        /**
         * @brief Get the layer name.
         * @return Readable name of the layer.
         */
        [[nodiscard]] std::string_view name() const;

        /**
         * @brief Update the layer (called each frame).
         */
        virtual void update() = 0;

        /**
         * @brief Event handling function.
         * @param event Event to handle.
         */
        [[nodiscard]] virtual bool on_event(const rw::evt::Event& event) = 0;

     protected:
        ID          id_{ 0U }; /**< Unique identifier for the layer. */
        std::string name_;     /**< Name of the layer. */
    };
} // namespace rw::engine

#endif // SRC_REDWOLF_LAYER_HPP
