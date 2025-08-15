//
// Created by cjm036653 on 14/08/2025.
//

#include "layer.hpp"

rw::layers::Layer::Layer(const ID id, const std::string_view name) : id_{ id }, name_(name) {}

rw::layers::Layer::ID rw::layers::Layer::id() const {
    return id_;
}

std::string_view rw::layers::Layer::name() const {
    return name_;
}