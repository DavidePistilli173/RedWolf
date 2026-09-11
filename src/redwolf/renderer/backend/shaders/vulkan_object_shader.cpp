#include "vulkan_object_shader.hpp"

#include "redwolf/containers/view.hpp"
#include "vulkan_shader_common.hpp"

rw::vk::ObjectShader::~ObjectShader() {}

bool rw::vk::ObjectShader::init(VkAllocationCallbacks* allocator, Ptr<Device> device) {
    allocator_ = allocator;
    device_    = std::move(device);

    constexpr std::array<VkShaderStageFlagBits, stage_count> stage_types{ VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT };
    constexpr std::array<View<char>, stage_count>            stage_type_strs{ "vert", "frag" };

    for (usize i{ 0 }; i < stage_count; ++i) {
        if (!create_shader_module(allocator_, device_, shader_name, stage_types[i], stage_type_strs[i], stages_[i])) {
            error("Failed to create '{}' shader module for shader '{}'", stage_type_strs[i], shader_name);
            return false;
        }
    }

    return true;
}

void rw::vk::ObjectShader::use() {}