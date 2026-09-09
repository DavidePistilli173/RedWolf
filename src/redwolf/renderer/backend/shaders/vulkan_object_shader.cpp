#include "vulkan_object_shader.hpp"

rw::vk::ObjectShader::~ObjectShader() {}

bool rw::vk::ObjectShader::init() {
    constexpr std::array<VkShaderStageFlagBits, stage_count> stage_types{ VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT };
    constexpr std::array<std::string_view, stage_count>      stage_type_strs{ "vert", "frag" };

    for (usize i{ 0 }; i < stage_count; ++i) {
        if (!create_shader_module(shader_name, stage_types[i], stage_type_strs[i], i, stages_)) {
            error("Failed to create '{}' shader module for shader '{}'", stage_type_strs[i], shader_name);
            return false;
        }
    }

    return true;
}

void rw::vk::ObjectShader::use() {}