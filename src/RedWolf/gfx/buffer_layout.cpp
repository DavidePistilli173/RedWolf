//
// Created by cjm036653 on 21/08/2025.
//

#include "buffer_layout.hpp"

rw::gfx::BufferLayoutElement::BufferLayoutElement(const ShaderDataType p_type, const std::string_view p_name, const bool p_normalized) :
    type{ p_type }, name{ p_name }, normalized{ p_normalized }, size{ shader_data_type_size(p_type) } {}

uint32_t rw::gfx::BufferLayoutElement::component_count() const {
    return shader_data_element_count(type);
}

rw::gfx::BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutElement>& elements) : elements_(elements) {
    uint32_t offset{ 0U };
    for (auto& element : elements_) {
        element.offset = offset;
        offset += element.size;
    }
    stride_ = offset; // Set the stride to the total size of the elements
}

rw::gfx::BufferLayout::iterator rw::gfx::BufferLayout::begin() {
    return elements_.begin();
}

rw::gfx::BufferLayout::const_iterator rw::gfx::BufferLayout::begin() const {
    return elements_.begin();
}

rw::gfx::BufferLayout::const_iterator rw::gfx::BufferLayout::cbegin() const {
    return elements_.cbegin();
}

rw::gfx::BufferLayout::const_iterator rw::gfx::BufferLayout::cend() const {
    return elements_.cend();
}

const std::vector<rw::gfx::BufferLayoutElement>& rw::gfx::BufferLayout::elements() const {
    return elements_;
}

rw::gfx::BufferLayout::iterator rw::gfx::BufferLayout::end() {
    return elements_.end();
}

rw::gfx::BufferLayout::const_iterator rw::gfx::BufferLayout::end() const {
    return elements_.end();
}

uint32_t rw::gfx::BufferLayout::stride() const {
    return stride_;
}
