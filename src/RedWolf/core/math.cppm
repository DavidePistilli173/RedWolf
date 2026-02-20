module;

export module redwolf.core.math;

import glm;

export namespace rw::core {
    using Vec2 = glm::vec2; /**< 2D vector type. */
    using Vec3 = glm::vec3; /**< 3D vector type. */
    using Vec4 = glm::vec4; /**< 4D vector type. */
    using Mat3 = glm::mat3; /**< 3x3 matrix type. */
    using Mat4 = glm::mat4; /**< 4x4 matrix type. */

    /**
     * @brief Translation, rotation and scale.
     */
    struct Transform {
        Vec3  position{ 0.0F }; /**< Position. */
        float rotation{ 0.0F }; /**< Rotation angle. [rad] */
        Vec2  size{ 1.0F };     /**< Size (x, y axes). */
    };

    /**
     * @brief Convert degrees to radians.
     * @param degrees Input angle in degrees.
     * @return Angle in radians.
     */
    [[nodiscard]] float radians(const float degrees) {
        return glm::radians(degrees);
    }

    /**
     * @brief Rotate a matrix by a given angle around a specified axis.
     * @param matrix Matrix to rotate.
     * @param angle Angle in degrees to rotate the matrix by.
     * @param axis Axis of rotation.
     * @return Rotated matrix.
     */
    [[nodiscard]] Mat4 rotate(const Mat4& matrix, const float angle, const Vec3& axis) {
        return glm::gtc::rotate(matrix, angle, axis);
    }

    /**
     * @brief Scale a 4x4 matrix by given scale factors.
     * @param matrix Matrix to scale.
     * @param scale_factors Scale factors for each axis.
     * @return Scaled matrix.
     */
    [[nodiscard]] Mat4 scale(const Mat4& matrix, const Vec3& scale_factors) {
        return glm::gtc::scale(matrix, scale_factors);
    }

    /**
     * @brief Translate a matrix by a given vector.
     * @param matrix Matrix to translate.
     * @param translation Translation vector.
     * @return Translated matrix.
     */
    [[nodiscard]] Mat4 translate(const Mat4& matrix, const Vec3& translation) {
        return glm::gtc::translate(matrix, translation);
    }

    /**
     * @brief Build a 2D transformation matrix from translation, rotation (in degrees), and scale vectors.
     * @param translation Translation vector.
     * @param rotation Rotation angle in degrees.
     * @param scale Scale vector (one component for each axis).
     * @return Transformation matrix resulting from T * R * S.
     */
    [[nodiscard]] Mat4 build_2d_transform(const Vec3& translation, const float rotation, const Vec2& scale) {
        return translate(Mat4(1.0F), translation) * rotate(Mat4(1.0F), radians(rotation), Vec3(0.0F, 0.0F, 1.0F)) *
               rw::core::scale(Mat4(1.0F), { scale.x, scale.y, 1.0F });
    }

    /**
     * @brief Compute the inverse of a matrix.
     * @param matrix Matrix to invert.
     * @return Inverted input matrix.
     */
    [[nodiscard]] Mat4 inverse(const Mat4& matrix) {
        return glm::inverse(matrix);
    }

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
    [[nodiscard]] Mat4 ortho(float left, float right, float bottom, float top, float near, float far) {
        return glm::gtc::ortho(left, right, bottom, top, near, far);
    }

    /**
     * @brief Get a raw data pointer to the vector data to use for OpenGL.
     * @param vec Vector to get the data pointer from.
     * @return Raw data pointer.
     */
    [[nodiscard]] float* value_ptr(Vec4& vec) {
        return glm::gtc::value_ptr(vec);
    }

    /**
     * @brief Get a const raw data pointer to the matrix data to use for OpenGL.
     * @param matrix Matrix to get the data pointer from.
     * @return Raw data pointer.
     */
    [[nodiscard]] const float* value_ptr(const Mat4& matrix) {
        return glm::gtc::value_ptr(matrix);
    }

    /**
     * @brief Get a raw data pointer to the matrix data to use for OpenGL.
     * @param matrix Matrix to get the data pointer from.
     * @return Raw data pointer.
     */
    [[nodiscard]] float* value_ptr(Mat4& matrix) {
        return glm::gtc::value_ptr(matrix);
    }
} // namespace rw::core
