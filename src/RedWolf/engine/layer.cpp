//
// Created by cjm036653 on 14/08/2025.
//

#include "layer.hpp"

rw::engine::Layer::Layer(const ID id, const std::string_view name) : id_{ id }, name_(name) {}

rw::engine::Layer::ID rw::engine::Layer::id() const {
    return id_;
}

std::string_view rw::engine::Layer::name() const {
    return name_;
}