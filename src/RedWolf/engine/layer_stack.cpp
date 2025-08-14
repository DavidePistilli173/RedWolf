//
// Created by cjm036653 on 14/08/2025.
//

#include "layer_stack.hpp"

#include <algorithm>

rw::engine::LayerStack::LayerStack() {
    layers_.reserve(default_layer_num);
}

rw::engine::LayerStack::iterator rw::engine::LayerStack::begin() {
    return layers_.begin();
}

rw::engine::LayerStack::const_iterator rw::engine::LayerStack::begin() const {
    return layers_.begin();
}

rw::engine::LayerStack::const_iterator rw::engine::LayerStack::cbegin() const {
    return layers_.cbegin();
}

rw::engine::LayerStack::const_iterator rw::engine::LayerStack::cend() const {
    return layers_.cend();
}

rw::engine::LayerStack::iterator rw::engine::LayerStack::end() {
    return layers_.end();
}

void rw::engine::LayerStack::pop_layer(const Layer::ID layer_id) {
    if (const auto it{ std::find_if(
            layers_.begin(), layers_.end(), [layer_id](const std::unique_ptr<Layer>& layer) { return layer_id == layer->id(); }) };
        layers_.end() != it) {
        layers_.erase(it);
        --layer_insert_it_;
    }
}

void rw::engine::LayerStack::pop_overlay(const Layer::ID overlay_id) {
    if (const auto it{ std::find_if(
            layers_.rbegin(), layers_.rend(), [overlay_id](const std::unique_ptr<Layer>& layer) { return overlay_id == layer->id(); }) };
        layers_.rend() != it) {
        layers_.erase(it.base());
    }
}