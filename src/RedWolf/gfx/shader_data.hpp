//
// Created by cjm036653 on 21/08/2025.
//

#ifndef SRC_REDWOLF_SHADER_HPP
#define SRC_REDWOLF_SHADER_HPP

#include <cstdint>

namespace rw::gfx {
    /**
     * @brief Data types used in shaders.
     */
    enum class ShaderDataType : uint8_t {
        none,      /**< No data type. */
        f32,       /**< Single float. */
        f32_2,     /**< Vector of 2 floats. */
        f32_3,     /**< Vector of 3 floats. */
        f32_4,     /**< Vector of 4 floats. */
        mat_f32_3, /**< 3x3 matrix of floats. */
        mat_f32_4, /**< 4x4 matrix of floats. */
        i32,       /**< Single integer. */
        i32_2,     /**< Vector of 2 integers. */
        i32_3,     /**< Vector of 3 integers. */
        i32_4,     /**< Vector of 4 integers. */
        boolean    /**< Boolean value. */
    };

    /**
     * @brief Get the size of a shader data type in bytes.
     * @param type Shader data type.
     * @return Size of the data type in bytes.
     */
    [[nodiscard]] static constexpr uint32_t shader_data_type_size(const ShaderDataType type) {
        switch (type) {
        case ShaderDataType::none:
            return 0U;
        case ShaderDataType::f32:
            return sizeof(float);
        case ShaderDataType::f32_2:
            return 2 * sizeof(float);
        case ShaderDataType::f32_3:
            return 3 * sizeof(float);
        case ShaderDataType::f32_4:
            return 4 * sizeof(float);
        case ShaderDataType::mat_f32_3:
            return 3 * 3 * sizeof(float); // 3x3 matrix
        case ShaderDataType::mat_f32_4:
            return 4 * 4 * sizeof(float); // 4x4 matrix
        case ShaderDataType::i32:
            return sizeof(int32_t);
        case ShaderDataType::i32_2:
            return 2 * sizeof(int32_t);
        case ShaderDataType::i32_3:
            return 3 * sizeof(int32_t);
        case ShaderDataType::i32_4:
            return 4 * sizeof(int32_t);
        case ShaderDataType::boolean:
            return 1U; // Boolean is typically 1 byte
        default:
            return 0U; // Invalid type
        }
    }

    /**
     * @brief Get the number of elements in a shader data type.
     * @param type Shader data type.
     * @return Number of elements in the specified data type.
     */
    [[nodiscard]] static constexpr uint32_t shader_data_element_count(const ShaderDataType type) {
        switch (type) {
        case ShaderDataType::none:
            return 0U;
        case ShaderDataType::f32:
            return 1U;
        case ShaderDataType::f32_2:
            return 2U;
        case ShaderDataType::f32_3:
            return 3U;
        case ShaderDataType::f32_4:
            return 4U;
        case ShaderDataType::mat_f32_3:
            return 3U * 3U; // 3x3 matrix has 9 elements
        case ShaderDataType::mat_f32_4:
            return 4U * 4U; // 4x4 matrix has 16 elements
        case ShaderDataType::i32:
            return 1U;
        case ShaderDataType::i32_2:
            return 2U;
        case ShaderDataType::i32_3:
            return 3U;
        case ShaderDataType::i32_4:
            return 4U;
        case ShaderDataType::boolean:
            return 1U; // Boolean is typically a single value
        default:
            return 0U; // Invalid type
        }
    }
} // namespace rw::gfx

#endif // SRC_REDWOLF_SHADER_HPP
