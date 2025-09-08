//
// Created by david on 24/08/2025.
//

#include "math.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

rw::math::Mat4 rw::math::inverse(const Mat4& matrix) {
    return glm::inverse(matrix);
}

rw::math::Mat4 rw::math::ortho(float left, float right, float bottom, float top, float near, float far) {
    return glm::ortho(left, right, bottom, top, near, far);
}

float rw::math::radians(float degrees) {
    return glm::radians(degrees);
}

rw::math::Mat4 rw::math::rotate(const Mat4& matrix, float angle, const Vec3& axis) {
    return glm::rotate(matrix, angle, axis);
}

rw::math::Mat4 rw::math::scale(const Mat4& matrix, const Vec3& scale_factors) {
    return glm::scale(matrix, scale_factors);
}

rw::math::Mat4 rw::math::translate(const Mat4& matrix, const Vec3& translation) {
    return glm::translate(matrix, translation);
}

float* rw::math::value_ptr(Vec4& vec) {
    return glm::value_ptr(vec);
}

const float* rw::math::value_ptr(const Mat4& matrix) {
    return glm::value_ptr(matrix);
}

float* rw::math::value_ptr(Mat4& matrix) {
    return glm::value_ptr(matrix);
}