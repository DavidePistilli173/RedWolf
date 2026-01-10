//
// Created by cjm036653 on 14/08/2025.
//

#ifndef SRC_REDWOLF_LAYER_STACK_HPP
#define SRC_REDWOLF_LAYER_STACK_HPP

#include "layer.hpp"

#include <concepts>
#include <memory>
#include <vector>

namespace rw::layers {
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
         * @tparam T Type of layer to create.
         * @param args Arguments for layer creation.
         * @return Non-owning pointer to the layer.
         */
        template<std::derived_from<Layer> T, typename... Args>
        [[nodiscard]] T* push_layer(Args&&... args) {
            const auto it     = layers_.emplace(layers_.begin() + layer_insert_idx_, std::make_unique<T>(std::forward<Args>(args)...));
            layer_insert_idx_ = it - layers_.begin();
            (*it)->attach();
            return dynamic_cast<T*>(it->get());
        }

        /**
         * @brief Create an overlay and push it onto the stack.
         * @tparam T Type of overlay to create.
         * @param args Arguments for overlay creation.
         * @return Non-owning pointer to the overlay.
         */
        template<std::derived_from<Layer> T, typename... Args>
        [[nodiscard]] T* push_overlay(Args&&... args) {
            auto& element{ layers_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)) };
            element->attach();
            return dynamic_cast<T*>(element.get());
        }

        /**
         * @brief Pop a layer from the stack.
         * @param layer_id Layer to pop from the stack.
         */
        void pop_layer(const Layer* layer_id);

        /**
         * @brief Pop an overlay from the stack.
         * @param overlay_id Overlay to pop from the stack.
         */
        void pop_overlay(const Layer* overlay_id);

     private:
        std::vector<std::unique_ptr<Layer>> layers_;                 /**< Actual stack of layers. */
        size_t                              layer_insert_idx_{ 0U }; /**< Position where to place layers. */
    };
} // namespace rw::layers
#endif // SRC_REDWOLF_LAYER_STACK_HPP
