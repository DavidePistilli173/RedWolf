//
// Created by david on 11/09/2025.
//

#include "shader_library.hpp"

rw::gfx::Shader* rw::gfx::ShaderLibrary::get(const uint64_t id) {
    const auto it{ shaders_.find(id) };
    if (shaders_.end() == it) {
        return nullptr;
    }
    return it->second.get();
}

rw::gfx::Shader* rw::gfx::ShaderLibrary::load(const uint64_t id, const std::string& file_path) {
    auto  new_shader{ std::unique_ptr<Shader>(new Shader(file_path)) };
    auto* result{ new_shader.get() };
    shaders_[id] = std::move(new_shader);
    return result;
}