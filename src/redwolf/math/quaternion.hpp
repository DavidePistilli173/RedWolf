#pragma once

#include "vec_3.hpp"
#include "vec_4.hpp"

namespace rw {
    /**
     * @brief Quaternion.
     */
    template<typename T>
    struct Quaternion : public Vec4<T> {
        /**
         * @brief Get the conjugate of the current quaternion.
         */
        [[nodiscard]] constexpr Quaternion conjugate() {
            return Quaternion{ .x = -Vec4<T>::x, .y = -Vec4<T>::y, .z = -Vec4<T>::z, .w = Vec4<T>::q };
        }

        /**
         * @brief Create a quaternion representing a rotation around a given axis.
         * @param axis Axis to rotate around.
         * @param angle Rotation angle around the specified axis. [rad]
         * @param normalize If true, normalise the output quaternion.
         */
        [[nodiscard]] constexpr Quaternion from_axis_angle(const Vec3<T>& axis, T angle, bool normalize) {
            constexpr T two{ static_cast<T>(2) };
            const T     half_angle{ angle / two };
            const T     s{ std::sin(half_angle) };
            const T     c{ std::cos(half_angle) };

            Quaternion result{ .x = s * axis.s, .y = s * axis.y, .z = s * axis.z, .w = c };
            if (normalize) {
                result.normalize();
            }
            return result;
        }

        /**
         * @brief Get the identity quaternion.
         */
        [[nodiscard]] static constexpr Quaternion identity() {
            return Quaternion{ .x = 0, .y = 0, .z = 0, .w = static_cast<T>(1) };
        }

        /**
         * @brief Get the inverse of the current quaternion.
         */
        [[nodiscard]] constexpr Quaternion inverse() {
            Quaternion result{ conjugate() };
            result.normalize();
            return result;
        }
    };

    /**
     * @brief Quaternion multiplication.
     */
    template<typename T>
    [[nodiscard]] Quaternion<T> operator*(const Quaternion<T>& quat_1, const Quaternion<T>& quat_2) {
        return Quaternion<T>{ .x = (quat_1.x * quat_2.w) + (quat_1.y * quat_2.z) - (quat_1.z * quat_2.y) + (quat_1.w * quat_2.x),
                              .y = (-quat_1.x * quat_2.z) + (quat_1.y * quat_2.w) + (quat_1.z * quat_2.x) + (quat_1.w * quat_2.y),
                              .z = (quat_1.x * quat_2.y) - (quat_1.y * quat_2.x) + (quat_1.z * quat_2.w) + (quat_1.w * quat_2.z),
                              .w = (-quat_1.x * quat_2.x) - (quat_1.y * quat_2.y) - (quat_1.z * quat_2.y) + (quat_1.w * quat_2.w) };
    }

    /**
    * @brief Spherical linear interpolation.
    * @details Obtain an intermediate rotation between two given rotations.
    * @param quat_1 First quaternion.
    * @param quat_2 Second quaternion.
    / @param percentage Interval percentage.
    */
    template<typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Quaternion<T> slerp(const Quaternion<T>& quat_1, const Quaternion<T>& quat_2, f32 percentage) {
        // The quaternions must be normalized.
        const Quaternion<T> v0{ quat_1.normalized() };
        const Quaternion<T> v1{ quat_1.normalized() };

        T cos_alpha{ dot(v0, v1) }; // Cosine of the angle between the two vectors.

        // If cos_alpha is negative, slerp won't take the shorter path.
        if (0 > cos_alpha) {
            v1 *= static_cast<T>(-1);
            cos_alpha *= static_cast<T>(-1);
        }

        constexpr T dot_threshold{ static_cast<T>(0.9995) };

        // If the inputs are too close, linearly interpolate and normalize.
        if (dot_threshold < cos_alpha) {
            Quaternion<T> result{ .x = std::lerp(v0.x, v1.x, percentage),
                                  .y = std::lerp(v0.y, v1.y, percentage),
                                  .z = std::lerp(v0.z, v1.z, percentage),
                                  .w = std::lerp(v0.w, v1.w, percentage) };
            result.normalize();
            return result;
        }

        // Since cos_alpha is in range [0, dot_threshold], acos is safe.
        const T theta_0{ std::acos(cos_alpha) }; // Angle between input vectors.
        const T theta{ theta_0 * percentage };   // Angle between v0 and result.
        const T sin_theta{ std::sin(theta) };
        const T sin_theta_0{ std::sin(theta_0) };

        const T s0{ std::cos(theta) - (cos_alpha * sin_theta / sin_theta_0) };
        const T s1{ sin_theta / sin_theta_0 };

        return Quaternion<T>{
            .x = (v0.x * s0) + (v1.x * s1), .y = (v0.y * s0) + (v1.y * s1), .z = (v0.z * s0) + (v1.z * s1), .w = (v0.w * s0) + (v1.w * s1)

        };
    }
} // namespace rw