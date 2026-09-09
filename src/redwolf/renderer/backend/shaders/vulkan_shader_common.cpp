#include "vulkan_shader_common.hpp"

static constexpr usize shader_path_size{ 1024 };

bool rw::vk::create_shader_module(
    std::string_view name, VkShaderStageFlagBits type, std::string_view type_name, usize index, ShaderStage& stage) {
    // Create the shader path.
    std::array<char, shader_path_size> shader_path{};
    const auto header_res{ std::format_to_n(shader_path.data(), shader_path.size() - 1, "assets/shaders/{}.{}.spv", name, type_name) };
    if (header_res.size > shader_path_size - 1) {
        error("Path of shader '{}' is too long.", name);
        return false;
    }

    stage.create_info = VkShaderModuleCreateInfo{

    };
}