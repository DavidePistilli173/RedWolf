#pragma once

#include "vulkan_shader_common.hpp"

#include <array>
#include <string_view>

namespace rw::vk {
    /**
     * @brief Generic object shader.
     */
    class ObjectShader {
     public:
        static constexpr std::string_view shader_name{ "rw.object_shader" }; /**< Name of the shader. */
        static constexpr usize            stage_count{ 2 };                  /**< Number of shader stages (vertex + fragment). */

        ObjectShader() = default;
        ~ObjectShader();

        ObjectShader(const ObjectShader&)            = delete;
        ObjectShader& operator=(const ObjectShader&) = delete;

        ObjectShader(ObjectShader&&)            = delete;
        ObjectShader& operator=(ObjectShader&&) = delete;

        /**
         * @brief Initialise the shader.
         */
        [[nodiscard]] bool init();

        /**
         * @brief Use the shader.
         */
        void use();

     private:
        std::array<ShaderStage, stage_count> stages_{}; /**< Stages of the shader. */
    };
} // namespace rw::vk