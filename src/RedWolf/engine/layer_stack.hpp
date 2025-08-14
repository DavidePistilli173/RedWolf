//
// Created by cjm036653 on 14/08/2025.
//

#ifndef SRC_REDWOLF_LAYER_STACK_HPP
#define SRC_REDWOLF_LAYER_STACK_HPP

#include "layer.hpp"

#include <concepts>
#include <memory>
#include <vector>

namespace rw::engine {
    /**
     * @brief Stack of layers in the application.
     */
    class LayerStack {
     public:
        /**
         * @brief Iterator type for the layer stack.
         */
        using iterator = std::vector<std::unique_ptr<Layer>>::iterator;

        /**
         * @brief Const iterator type for the layer stack.
         */
        using const_iterator = std::vector<std::unique_ptr<Layer>>::const_iterator;

        /**
         * @brief Default number of layers.
         */
        static constexpr size_t default_layer_num{ 32 };

        /**
         * @brief Constructor.
         */
        LayerStack();

        /**
         * @brief Get the begin iterator.
         * @return Begin iterator.
         */
        [[nodiscard]] iterator begin();

        /**
         * @brief Get the begin iterator (const).
         * @return Const begin iterator.
         */
        [[nodiscard]] const_iterator begin() const;

        /**
         * @brief Get the begin iterator (const).
         * @return Const begin iterator.
         */
        [[nodiscard]] const_iterator cbegin() const;

        /**
         * @brief Get the end iterator (const).
         * @return Const end iterator.
         */
        [[nodiscard]] const_iterator cend() const;

        /**
         * @brief Get the end iterator.
         * @return End iterator.
         */
        [[nodiscard]] iterator end();

        /**
         * @brief Get the end iterator (const).
         * @return Const end iterator.
         */
        [[nodiscard]] const_iterator end() const;

        /**
         * @brief Create a layer and push it onto the stack.
         * @param T Type of layer to create.
         * @param args Arguments for layer creation (except for the ID).
         * @return ID of the created layer.
         */
        template<std::derived_from<Layer> T, typename... Args>
        [[nodiscard]] Layer::ID push_layer(Args&&... args) {
            const Layer::ID id{ next_layer_id_++ };
            layer_insert_it_ = layers_.emplace(layer_insert_it_, std::make_unique<T>(id, std::forward<Args>(args)...));
            (*layer_insert_it_)->attach();
            return id;
        }

        /**
         * @brief Create an overlay and push it onto the stack.
         * @tparam T Type of overlay to create.
         * @param args Arguments for overlay creation (except for the ID).
         * @return ID of the created overlay.
         */
        template<std::derived_from<Layer> T, typename... Args>
        [[nodiscard]] Layer::ID push_overlay(Args&&... args) {
            const Layer::ID id{ next_layer_id_++ };
            auto&           element{ layers_.emplace_back(std::make_unique<T>(id, std::forward<Args>(args)...)) };
            element->attach();
            return id;
        }

        /**
         * @brief Pop a layer from the stack.
         * @param layer_id Layer to pop from the stack.
         */
        void pop_layer(const Layer::ID layer_id);

        /**
         * @brief Pop an overlay from the stack.
         * @param overlay_id Overlay to pop from the stack.
         */
        void pop_overlay(const Layer::ID overlay_id);

     private:
        Layer::ID                           next_layer_id_{ 1 };                 /**< ID for the next layer that will be created. */
        std::vector<std::unique_ptr<Layer>> layers_;                             /**< Actual stack of layers. */
        iterator                            layer_insert_it_{ layers_.begin() }; /**< Position where to place layers. */
    };
} // namespace rw::engine
#endif // SRC_REDWOLF_LAYER_STACK_HPP
