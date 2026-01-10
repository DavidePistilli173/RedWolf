//
// Created by cjm036653 on 14/08/2025.
//

#include "layer_stack.hpp"

#include "../util/logger.hpp"

#include <algorithm>
#include <ranges>

rw::layers::LayerStack::LayerStack() {
    layers_.reserve(default_layer_num);
}

rw::layers::LayerStack::iterator rw::layers::LayerStack::begin() {
    return layers_.begin();
}

rw::layers::LayerStack::const_iterator rw::layers::LayerStack::begin() const {
    return layers_.begin();
}

rw::layers::LayerStack::const_iterator rw::layers::LayerStack::cbegin() const {
    return layers_.cbegin();
}

rw::layers::LayerStack::const_iterator rw::layers::LayerStack::cend() const {
    return layers_.cend();
}

rw::layers::LayerStack::iterator rw::layers::LayerStack::end() {
    return layers_.end();
}

void rw::layers::LayerStack::pop_layer(const Layer* layer_id) {
    if (const auto it{ std::ranges::find_if(layers_, [layer_id](const std::unique_ptr<Layer>& layer) { return layer_id == layer.get(); }) };
        layers_.end() != it) {
        (*it)->detach();
        layers_.erase(it);
        --layer_insert_idx_;
    }
}

void rw::layers::LayerStack::pop_overlay(const Layer* overlay_id) {
    if (const auto it{ std::ranges::find_if(
            std::views::reverse(layers_), [overlay_id](const std::unique_ptr<Layer>& layer) { return overlay_id == layer.get(); }) };
        layers_.rend() != it) {
        (*it)->detach();
        layers_.erase(it.base());
    }
}