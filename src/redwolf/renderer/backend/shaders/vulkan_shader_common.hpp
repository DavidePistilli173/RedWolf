#pragma once

#include "redwolf/containers/view.hpp"
#include "redwolf/renderer/backend/vulkan_common.hpp"
#include "redwolf/renderer/backend/vulkan_device.hpp"

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
     * @param allocator Custom vulkan allocator.
     * @param device Rendering device.
     * @param name Name of the shader.
     * @param type Type of shader stage.
     * @param type_name Name of the shader type.
     * @param stage Output shader stage.
     * @return true on success, false otherwise.
     */
    [[nodiscard]] bool create_shader_module(
        VkAllocationCallbacks* allocator,
        Ptr<Device>&           device,
        View<char>             name,
        VkShaderStageFlagBits  type,
        View<char>             type_name,
        ShaderStage&           stage);
} // namespace rw::vk