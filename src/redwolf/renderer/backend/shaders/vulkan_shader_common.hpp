#pragma once

#include "redwolf/renderer/backend/vulkan_common.hpp"

#include <string_view>

namespace rw::vk {
    /**
     * @brief Vulkan pipeline data.
     */
    struct Pipeline {
        VkPipeline       pipeline{ VK_NULL_HANDLE }; /**< Handle to the pipeline. */
        VkPipelineLayout layout{ VK_NULL_HANDLE };   /**< Handle to the pipeline layout. */
    };

    /**
     * @brief Single stage of a shader.
     */
    struct ShaderStage {
        VkShaderModuleCreateInfo        create_info{};            /**< Creation parameters of the shader module.  */
        VkShaderModule                  handle{ VK_NULL_HANDLE }; /**< Handle to the shader module. */
        VkPipelineShaderStageCreateInfo stage_create_info{};      /**< Pipeline stage creation parameters. */
    };

    /**
     * @brief Create a shader module.
     * @param name Name of the shader.
     * @param type Type of shader stage.
     * @param type_name Name of the shader type.
     * @param index Index of the current stage.
     * @param stage Output shader stage.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool create_shader_module(
        std::string_view name, VkShaderStageFlagBits type, std::string_view type_name, usize index, ShaderStage& stage);
} // namespace rw::vk