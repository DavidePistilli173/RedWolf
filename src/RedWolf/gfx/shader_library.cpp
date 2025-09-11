//
// Created by david on 11/09/2025.
//

#include "shader_library.hpp"

void rw::gfx::ShaderLibrary::add(const uint64_t id, std::shared_ptr<Shader> shader) {
    shaders_[id] = std::move(shader);
}

std::optional<std::shared_ptr<rw::gfx::Shader>> rw::gfx::ShaderLibrary::get(const uint64_t id) {
    const auto it{ shaders_.find(id) };
    if (shaders_.end() == it) {
        return {};
    }
    return it->second;
}

std::shared_ptr<rw::gfx::Shader> rw::gfx::ShaderLibrary::load(const uint64_t id, const std::string& file_path) {
    auto new_shader{ std::make_shared<Shader>(file_path) };
    shaders_[id] = new_shader;
    return new_shader;
}