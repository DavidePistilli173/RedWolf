//
// Created by david on 11/09/2025.
//

#ifndef SRC_REDWOLF_SHADER_LIBRARY_HPP
#define SRC_REDWOLF_SHADER_LIBRARY_HPP

#include "gfx.hpp"

#include <cstdint>
#include <optional>
#include <unordered_map>

namespace rw::gfx {
    /**
     * @brief Class for managing a collection of shaders.
     */
    class ShaderLibrary {
     public:
        /**
         * @brief Get the specified shader.
         * @param id ID of the shader to get.
         * @return Shader with the specified ID, if it exists. nullptr otherwise.
         */
        [[nodiscard]] Shader* get(const uint64_t id);

        /**
         * @brief Load a shader from file and store it.
         * @param id Id of the shader. If the ID already exists, the shader is replaced.
         * @param file_path Path to the shader source code.
         * @return Pointer to the newly created shader.
         */
        [[nodiscard]] Shader* load(const uint64_t id, const std::string& file_path);

     private:
        std::unordered_map<uint64_t, std::unique_ptr<Shader>> shaders_; /**< Actual shader collection. */
    };
} // namespace rw::gfx

#endif // SRC_REDWOLF_SHADER_LIBRARY_HPP
