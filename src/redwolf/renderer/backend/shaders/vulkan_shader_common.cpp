#include "vulkan_shader_common.hpp"

#include "redwolf/platform/file.hpp"

static constexpr usize shader_path_size{ 1024 };

bool rw::vk::create_shader_module(
    VkAllocationCallbacks* allocator,
    Ptr<Device>&           device,
    View<char>             name,
    VkShaderStageFlagBits  type,
    View<char>             type_name,
    ShaderStage&           stage) {
    // Create the shader path.
    std::array<char, shader_path_size> shader_path{};
    const auto header_res{ std::format_to_n(shader_path.data(), shader_path.size() - 1, "data/shaders/{}.{}.spv", name, type_name) };
    if (header_res.size > shader_path_size - 1) {
        error("Path of shader '{}' is too long.", name);
        return false;
    }

    View<char> shader_path_view{ shader_path.data(), static_cast<usize>(header_res.size) };
    File       shader_file{ MemoryCategory::renderer, shader_path_view };
    if (!shader_file.open(File::OpenMode::read, true)) {
        error("Failed to open shader '{}'", shader_path_view);
        return false;
    }

    Vec<u32> shader_contents{ MemoryCategory::renderer };
    if (!shader_file.read_all(shader_contents.to<u8>())) {
        error("Failed to read shader '{}'", shader_path_view);
        return false;
    }

    stage.create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    stage.create_info.codeSize = shader_contents.size();
    stage.create_info.pCode    = shader_contents.data();

    RW_VK_CHECK(
        vkCreateShaderModule(device->logical(), &stage.create_info, allocator, &stage.handle),
        "Failed to create shader module: '{}'",
        false)

    stage.stage_create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage.stage_create_info.stage  = type;
    stage.stage_create_info.module = stage.handle;
    stage.stage_create_info.pName  = "main"; // Name of the shader entry point.

    return true;
}