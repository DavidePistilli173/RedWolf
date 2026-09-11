#pragma once

#include "redwolf/containers/view.hpp"
#include "vulkan_shader_common.hpp"

#include <array>

namespace rw::vk {
    /**
     * @brief Generic object shader.
     */
    class ObjectShader {
     public:
        static constexpr View<char> shader_name{ "rw.object_shader" }; /**< Name of the shader. */
        static constexpr usize      stage_count{ 2 };                  /**< Number of shader stages (vertex + fragment). */

        ObjectShader() = default;
        ~ObjectShader();

        ObjectShader(const ObjectShader&)            = delete;
        ObjectShader& operator=(const ObjectShader&) = delete;

        ObjectShader(ObjectShader&&)            = delete;
        ObjectShader& operator=(ObjectShader&&) = delete;

        /**
         * @brief Initialise the shader.
         * @param allocator Custom vulkan allocator.
         * @param device Rendering device.
         */
        [[nodiscard]] bool init(VkAllocationCallbacks* allocator, Ptr<Device> device);

        /**
         * @brief Use the shader.
         */
        void use();

     private:
        VkAllocationCallbacks*               allocator_{ nullptr }; /**< Custom vulkan allocator. */
        Ptr<Device>                          device_;               /**< Rendering device. */
        std::array<ShaderStage, stage_count> stages_{};             /**< Stages of the shader. */
    };
} // namespace rw::vk