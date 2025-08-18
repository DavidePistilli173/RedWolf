//
// Created by cjm036653 on 14/08/2025.
//

#include "layer.hpp"

rw::layers::Layer::Layer(const std::string_view name) : name_(name) {}

std::string_view rw::layers::Layer::name() const {
    return name_;
}