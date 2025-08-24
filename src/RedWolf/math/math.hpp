//
// Created by cjm036653 on 18/08/2025.
//

#ifndef SRC_REDWOLF_MATH_HPP
#define SRC_REDWOLF_MATH_HPP

#include <glm/glm.hpp>

namespace rw::math {
    using Vec2 = glm::vec2; /**< 2D vector type. */
    using Vec3 = glm::vec3; /**< 3D vector type. */
    using Vec4 = glm::vec4; /**< 4D vector type. */
    using Mat4 = glm::mat4; /**< 4x4 matrix type. */

    /**
     * @brief Compute the inverse of a matrix.
     * @param matrix Matrix to invert.
     * @return Inverted input matrix.
     */
    [[nodiscard]] Mat4 inverse(const Mat4& matrix);

    /**
     * @brief Create an orthographic projection matrix.
     * @param left Left limit of the projection.
     * @param right Right limit of the projection.
     * @param bottom Bottom limit of the projection.
     * @param top Top limit of the projection.
     * @param near Near limit of the projection.
     * @param far Far limit of the projection.
     * @return Orthographic projection matrix.
     */
    [[nodiscard]] Mat4 ortho(float left, float right, float bottom, float top, float near, float far);

    /**
     * @brief Convert degrees to radians.
     * @param degrees Input angle in degrees.
     * @return Angle in radians.
     */
    [[nodiscard]] float radians(float degrees);

    /**
     * @brief Rotate a matrix by a given angle around a specified axis.
     * @param matrix Matrix to rotate.
     * @param angle Angle in degrees to rotate the matrix by.
     * @param axis Axis of rotation.
     * @return Rotated matrix.
     */
    [[nodiscard]] Mat4 rotate(const Mat4& matrix, float angle, const Vec3& axis);

    /**
     * @brief Translate a matrix by a given vector.
     * @param matrix Matrix to translate.
     * @param translation Translation vector.
     * @return Translated matrix.
     */
    [[nodiscard]] Mat4 translate(const Mat4& matrix, const Vec3& translation);

    /**
     * @brief Get a raw data pointer to the matrix data to use for OpenGL.
     * @param matrix Matrix to get the data pointer from.
     * @return Raw data pointer.
     */
    [[nodiscard]] const float* value_ptr(const Mat4& matrix);
} // namespace rw::math

#endif // SRC_REDWOLF_MATH_HPP
