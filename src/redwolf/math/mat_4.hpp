#pragma once

#include "math_functions.hpp"
#include "quaternion.hpp"
#include "redwolf/common.hpp"
#include "vec_3.hpp"

#include <array>

namespace rw {
    /**
     *  @brief 4x4 matrix.a
     */
    template<typename T>
    struct Mat4 {
        /**
         * @brief Parameters for creating an orthographic projection matrix.
         */
        struct OrthoParams {
            T left{};   /**< Left clip limit. */
            T right{};  /**< Right clip limit. */
            T bottom{}; /**< Bottom clip limit. */
            T top{};    /**< Top clip limit. */
            T near{};   /**< Near clip limit. */
            T far{};    /**< Far clip limit. */
        };

        /**
         * @brief Parameters for creating a perspective projection matrix.
         */
        struct PerspParams {
            T fov{};          /**< Field of view. [rad] */
            T aspect_ratio{}; /**< Aspect ratio. */
            T near{};         /**< Near clip distance. */
            T far{};          /**< Far clip distance. */
        };

        /**
         * @brief Parameters for creating a look at matrix.
         */
        struct LookAtParams {
            Vec3<T> position; /**< Camera position. */
            Vec3<T> target;   /**< Camera target. */
            Vec3<T> up;       /**< Up direction. */
        };

        static constexpr usize dim{ 4 }; /**< Matrix dimension. */

        std::array<T, dim * dim> data{}; /**< Actual matrix data, in row-major layout. */

        /**
         * @brief Get the backward vector relative to the matrix.
         */
        [[nodiscard]] constexpr Vec3<T> backward() const {
            Vec3<T> result{ .x = data[2], .y = data[6], .z = data[10] };
            result.normalize();
            return result;
        }

        /**
         * @brief Get the down vector relative to the matrix.
         */
        [[nodiscard]] constexpr Vec3<T> down() const {
            Vec3<T> result{ .x = -data[1], .y = -data[5], .z = -data[9] };
            result.normalize();
            return result;
        }

        /**
         * @brief Get the forward vector relative to the matrix.
         */
        [[nodiscard]] constexpr Vec3<T> forward() const {
            Vec3<T> result{ .x = -data[2], .y = -data[6], .z = -data[10] };
            result.normalize();
            return result;
        }

        /**
         * @brief Create the identity matrix.
         */
        [[nodiscard]] static constexpr Mat4 identity() {
            return Mat4{ .data = { // Row 0
                                   static_cast<T>(1),
                                   0,
                                   0,
                                   0,
                                   // Row 1
                                   0,
                                   static_cast<T>(1),
                                   0,
                                   0,
                                   // Row 2
                                   0,
                                   0,
                                   static_cast<T>(1),
                                   0,
                                   // Row 3
                                   0,
                                   0,
                                   0,
                                   static_cast<T>(1) } };
        }

        /**
         * @brief Obtain the inverse of the current matrix.
         */
        [[nodiscard]] constexpr Mat4 inverse() const {
            constexpr T one{ static_cast<T>(1) };

            const T t0{ data[10] * data[15] };
            const T t1{ data[14] * data[11] };
            const T t2{ data[6] * data[15] };
            const T t3{ data[14] * data[7] };
            const T t4{ data[6] * data[11] };
            const T t5{ data[10] * data[7] };
            const T t6{ data[2] * data[15] };
            const T t7{ data[14] * data[3] };
            const T t8{ data[2] * data[11] };
            const T t9{ data[10] * data[3] };
            const T t10{ data[2] * data[7] };
            const T t11{ data[6] * data[3] };
            const T t12{ data[8] * data[13] };
            const T t13{ data[12] * data[9] };
            const T t14{ data[4] * data[13] };
            const T t15{ data[12] * data[5] };
            const T t16{ data[4] * data[9] };
            const T t17{ data[8] * data[5] };
            const T t18{ data[0] * data[13] };
            const T t19{ data[12] * data[1] };
            const T t20{ data[0] * data[9] };
            const T t21{ data[8] * data[1] };
            const T t22{ data[0] * data[5] };
            const T t23{ data[4] * data[1] };

            Mat4 result;

            result.data[0] = ((t0 * data[5]) + (t3 * data[9]) + (t4 * data[13])) - ((t1 * data[5]) + (t2 * data[9]) + (t5 * data[13]));
            result.data[1] = ((t1 * data[1]) + (t6 * data[9]) + (t9 * data[13])) - ((t0 * data[1]) + (t7 * data[9]) + (t8 * data[13]));
            result.data[2] = ((t2 * data[1]) + (t7 * data[5]) + (t10 * data[13])) - ((t3 * data[1]) + (t6 * data[5]) + (t11 * data[13]));
            result.data[3] = ((t5 * data[1]) + (t8 * data[5]) + (t11 * data[9])) - ((t4 * data[1]) + (t9 * data[5]) + (t10 * data[9]));

            const T d{ one / ((data[0] * result[0]) + (data[4] * result[1]) + (data[8] * result[2]) + (data[12] * result[3])) };

            result.data[0] = d * result.data[0];
            result.data[1] = d * result.data[1];
            result.data[2] = d * result.data[2];
            result.data[3] = d * result.data[3];
            result.data[4] =
                d * (((t1 * data[4]) + (t2 * data[8]) + (t5 * data[12])) - ((t0 * data[4]) + (t3 * data[8]) + (t4 * data[12])));
            result.data[5] =
                d * (((t0 * data[0]) + (t7 * data[8]) + (t8 * data[12])) - ((t1 * data[0]) + (t6 * data[8]) + (t9 * data[12])));
            result.data[6] =
                d * (((t3 * data[0]) + (t6 * data[4]) + (t11 * data[12])) - ((t2 * data[0]) + (t7 * data[4]) + (t10 * data[12])));
            result.data[7] =
                d * (((t4 * data[0]) + (t9 * data[4]) + (t10 * data[8])) - ((t5 * data[0]) + (t8 * data[4]) + (t11 * data[8])));
            result.data[8] =
                d * (((t12 * data[7]) + (t15 * data[11]) + (t16 * data[15])) - ((t13 * data[7]) + (t14 * data[11]) + (t17 * data[15])));
            result.data[9] =
                d * (((t13 * data[3]) + (t18 * data[11]) + (t21 * data[15])) - ((t12 * data[3]) + (t19 * data[11]) + (t20 * data[15])));
            result.data[10] =
                d * (((t14 * data[3]) + (t19 * data[7]) + (t22 * data[15])) - ((t15 * data[3]) + (t18 * data[7]) + (t23 * data[15])));
            result.data[11] =
                d * (((t17 * data[3]) + (t20 * data[7]) + (t23 * data[11])) - ((t16 * data[3]) + (t21 * data[7]) + (t22 * data[11])));
            result.data[12] =
                d * (((t14 * data[10]) + (t17 * data[14]) + (t13 * data[6])) - ((t16 * data[14]) + (t12 * data[6]) + (t15 * data[10])));
            result.data[13] =
                d * (((t20 * data[14]) + (t12 * data[2]) + (t19 * data[10])) - ((t18 * data[10]) + (t21 * data[14]) + (t13 * data[2])));
            result.data[14] =
                d * (((t18 * data[6]) + (t23 * data[14]) + (t15 * data[2])) - ((t22 * data[14]) + (t14 * data[2]) + (t19 * data[6])));
            result.data[15] =
                d * (((t22 * data[10]) + (t16 * data[2]) + (t21 * data[6])) - ((t20 * data[6]) + (t23 * data[10]) + (t17 * data[2])));

            return result;
        }

        /**
         * @brief Get the left vector relative to the matrix.
         */
        [[nodiscard]] constexpr Vec3<T> left() const {
            Vec3<T> result{ .x = -data[0], .y = -data[4], .z = -data[8] };
            result.normalize();
            return result;
        }

        /**
         * @brief Create a look at matrix.
         * @param params Creation parameters.
         */
        [[nodiscard]] static constexpr Mat4 look_at(const LookAtParams& params) {
            constexpr T one{ static_cast<T>(1) };

            Mat4 result;

            Vec3<T> z_axis{ params.target - params.position };
            z_axis.normalize();

            const Vec3<T> x_axis{ cross(z_axis, params.up).normalized() };
            const Vec3<T> y_axis{ cross(x_axis, z_axis) };

            result.data[0]  = x_axis.x;
            result.data[1]  = y_axis.x;
            result.data[2]  = -z_axis.x;
            result.data[3]  = 0;
            result.data[4]  = x_axis.y;
            result.data[5]  = y_axis.y;
            result.data[6]  = -z_axis.y;
            result.data[7]  = 0;
            result.data[8]  = x_axis.z;
            result.data[9]  = y_axis.z;
            result.data[10] = -z_axis.z;
            result.data[11] = 0;
            result.data[12] = -dot(x_axis, params.position);
            result.data[13] = -dot(y_axis, params.position);
            result.data[14] = dot(z_axis, params.position);
            result.data[15] = one;

            return result;
        }

        /**
         * @brief Create an orthographic projection matrix.
         * @param params Creation parameters.
         */
        [[nodiscard]] static constexpr Mat4 orthographic(const OrthoParams& params) {
            Mat4 result{ identity() };

            constexpr T one{ static_cast<T>(1) };
            constexpr T two{ static_cast<T>(2) };

            const T lr{ one / (params.left - params.right) };
            const T bt{ one / (params.bottom - params.top) };
            const T nf{ one / (params.near - params.far) };

            result.data[0]  = -two * lr;
            result.data[5]  = -two * bt;
            result.data[10] = two * nf;

            result.data[12] = (params.left + params.right) * lr;
            result.data[13] = (params.top + params.bottom) * bt;
            result.data[13] = (params.far + params.near) * nf;

            return result;
        }

        /**
         * @brief Create a persepective projection matrix.
         * @params params Creation parameters.
         */
        [[nodiscard]] static constexpr Mat4 perspective(const PerspParams& params) {
            constexpr T one{ static_cast<T>(1) };
            constexpr T two{ static_cast<T>(2) };
            const T     half_tan_fov{ std::tan(params.fov / 2) };

            Mat4 result{};
            result.data[0]  = one / (params.aspect_ratio * half_tan_fov);
            result.data[5]  = one / half_tan_fov;
            result.data[10] = -((params.far + params.near) / (params.far - params.near));
            result.data[11] = -one;
            result.data[14] = -((two * params.far * params.near) / (params.far - params.near));
            return result;
        }

        /**
         * @brief Get the right vector relative to the matrix.
         */
        [[nodiscard]] constexpr Vec3<T> right() const {
            Vec3<T> result{ .x = data[0], .y = data[4], .z = data[8] };
            result.normalize();
            return result;
        }

        /**
         * @brief Create a matrix representing a rotation around the X axis.
         * @param angle Angle in radians.
         */
        [[nodiscard]] static constexpr Mat4 rotation_x(T angle) {
            Mat4 result{ identity() };

            const T c{ std::cos(angle) };
            const T s{ std::sin(angle) };

            result.data[5]  = c;
            result.data[6]  = s;
            result.data[9]  = -s;
            result.data[10] = c;

            return result;
        }

        /**
         * @brief Create a matrix representing a rotation around the Y axis.
         * @param angle Angle in radians.
         */
        [[nodiscard]] static constexpr Mat4 rotation_y(T angle) {
            Mat4 result{ identity() };

            const T c{ std::cos(angle) };
            const T s{ std::sin(angle) };

            result.data[0]  = c;
            result.data[2]  = -s;
            result.data[8]  = s;
            result.data[10] = c;

            return result;
        }

        /**
         * @brief Create a matrix representing a rotation around the Z axis.
         * @param angle Angle in radians.
         */
        [[nodiscard]] static constexpr Mat4 rotation_z(T angle) {
            Mat4 result{ identity() };

            const T c{ std::cos(angle) };
            const T s{ std::sin(angle) };

            result.data[0] = c;
            result.data[1] = s;
            result.data[4] = -s;
            result.data[5] = c;

            return result;
        }

        /**
         * @brief Create a rotation matrix.
         * @param angles Angles for all 3 axes.
         */
        [[nodiscard]] static constexpr Mat4 rotation(const Vec3<T> angles) {
            const Mat4 rx{ rotation_x(angles.x) };
            const Mat4 ry{ rotation_y(angles.y) };
            const Mat4 rz{ rotation_z(angles.z) };

            return (rx * ry) * rz;
        }

        /**
         * @brief Create a rotation matrix.
         * @param quat Quaternion representing the desired rotation.
         */
        [[nodiscard]] static constexpr Mat4 rotation(const Quaternion<T>& quat) {
            constexpr T one{ static_cast<T>(1) };
            constexpr T two{ static_cast<T>(2) };

            Mat4 result{ identity() };

            const auto n{ quat.normalized() };

            result.data[0] = one - (two * n.y * n.y) - (two * n.z * n.z);
            result.data[1] = (two * n.x * n.y) - (two * n.z * n.w);
            result.data[2] = (two * n.x * n.z) + (two * n.y * n.w);

            result.data[4] = (two * n.x * n.y) + (two * n.z * n.w);
            result.data[5] = one - (two * n.x * n.x) - (two * n.z * n.z);
            result.data[6] = (two * n.y * n.z) - (two * n.x * n.w);

            result.data[8]  = (two * n.x * n.z) - (two * n.y * n.w);
            result.data[9]  = (two * n.y * n.z) + (two * n.x * n.w);
            result.data[10] = one - (two * n.x * n.x) - (two * n.y * n.y);

            return result;
        }

        /**
         * @brief Create a rotation matrix.
         * @param quat Quaternion representing the desired roation.
         * @param center Center point of the rotation.
         */
        [[nodiscard]] static constexpr Mat4 rotation(const Quaternion<T>& quat, const Vec3<T>& center) {
            constexpr T two{ static_cast<T>(2) };

            Mat4 result;

            result.data[0] = (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z) + (quat.w * quat.w);
            result.data[1] = two * ((quat.x * quat.y) + (quat.z * quat.w));
            result.data[2] = two * ((quat.x * quat.z) - (quat.y * quat.w));
            result.data[3] = center.x - (center.x * result.data[0]) - (center.y * result.data[1]) - (center.z * result.data[2]);

            result.data[4] = two * ((quat.x * quat.y) - (quat.z * quat.w));
            result.data[5] = -(quat.x * quat.x) + (quat.y * quat.y) - (quat.z * quat.z) + (quat.w * quat.w);
            result.data[6] = two * ((quat.y * quat.z) + (quat.x * quat.w));
            result.data[7] = center.y - (center.x * result.data[4]) - (center.y * result.data[5]) - center.z * result.data[6];

            result[8]  = two * ((quat.x * quat.z) + (quat.y * quat.w));
            result[9]  = two * ((quat.y * quat.z) - (quat.x * quat.w));
            result[10] = -(quat.x * quat.x) - (quat.y * quat.y) + (quat.z * quat.z) + (quat.w * quat.w);
            result[11] = center.z - (center.x * result.data[8]) - (center.y * result.data[9]) - (center.z * result.data[10]);

            result[12] = 0;
            result[13] = 0;
            result[14] = 0;
            result[15] = static_cast<T>(1);

            return result;
        }

        /**
         * @brief Create a scale matrix.
         * @param scale Scale factors for each axis.
         */
        [[nodiscard]] static constexpr Mat4 scale(const Vec3<T>& scale) {
            Mat4 result{ identity() };
            result.data[0]  = scale.x;
            result.data[5]  = scale.y;
            result.data[10] = scale.z;
            return result;
        }

        /**
         * @brief Create a translation matrix.
         * @param position Translation vector.
         */
        [[nodiscard]] static constexpr Mat4 translation(const Vec3<T>& position) {
            Mat4 result{ identity() };
            result.data[12] = position.x;
            result.data[13] = position.y;
            result.data[14] = position.z;
            return result;
        }

        /**
         * @brief Transpose the current matrix.
         */
        constexpr void transpose() {
            data = transposed().data;
        }

        /**
         * @brief Create the transposed of the current matrix.
         */
        [[nodiscard]] constexpr Mat4 transposed() const {
            Mat4 result;
            result.data[0]  = data[0];
            result.data[1]  = data[4];
            result.data[2]  = data[8];
            result.data[3]  = data[12];
            result.data[4]  = data[1];
            result.data[5]  = data[5];
            result.data[6]  = data[9];
            result.data[7]  = data[13];
            result.data[8]  = data[2];
            result.data[9]  = data[6];
            result.data[10] = data[10];
            result.data[11] = data[14];
            result.data[12] = data[3];
            result.data[13] = data[7];
            result.data[14] = data[11];
            result.data[15] = data[15];
            return result;
        }

        /**
         * @brief Get the up vector relative to the matrix.
         */
        [[nodiscard]] constexpr Vec3<T> up() const {
            Vec3<T> result{ .x = data[1], .y = data[5], .z = data[9] };
            result.normalize();
            return result;
        }
    };

    /**
     * @brief Matrix multiplication.
     */
    template<typename T>
    [[nodiscard]] constexpr Mat4<T> operator*(const Mat4<T>& mat_1, const Mat4<T>& mat_2) {
        Mat4<T> result{ Mat4<T>::identity() };

        // Row 0
        result.data[0] = (mat_1.data[0] * mat_2.data[0]) + (mat_1.data[1] * mat_2.data[4]) + (mat_1.data[2] * mat_2.data[8]) +
                         (mat_1.data[3] * mat_2.data[12]);
        result.data[1] = (mat_1.data[0] * mat_2.data[1]) + (mat_1.data[1] * mat_2.data[5]) + (mat_1.data[2] * mat_2.data[9]) +
                         (mat_1.data[3] * mat_2.data[13]);
        result.data[2] = (mat_1.data[0] * mat_2.data[2]) + (mat_1.data[1] * mat_2.data[6]) + (mat_1.data[2] * mat_2.data[10]) +
                         (mat_1.data[3] * mat_2.data[14]);
        result.data[3] = (mat_1.data[0] * mat_2.data[3]) * (mat_1.data[1] * mat_2.data[7]) + (mat_1.data[2] * mat_2.data[11]) +
                         (mat_1.data[3] * mat_2.data[15]);

        // Row 1
        result.data[4] = (mat_1.data[4] * mat_2.data[0]) + (mat_1.data[5] * mat_2.data[4]) + (mat_1.data[6] * mat_2.data[8]) +
                         (mat_1.data[7] * mat_2.data[12]);
        result.data[5] = (mat_1.data[4] * mat_2.data[1]) + (mat_1.data[5] * mat_2.data[5]) + (mat_1.data[6] * mat_2.data[9]) +
                         (mat_1.data[7] * mat_2.data[13]);
        result.data[6] = (mat_1.data[4] * mat_2.data[2]) + (mat_1.data[5] * mat_2.data[6]) + (mat_1.data[6] * mat_2.data[10]) +
                         (mat_1.data[7] * mat_2.data[14]);
        result.data[7] = (mat_1.data[4] * mat_2.data[3]) * (mat_1.data[5] * mat_2.data[7]) + (mat_1.data[6] * mat_2.data[11]) +
                         (mat_1.data[7] * mat_2.data[15]);

        // Row 2
        result.data[8]  = (mat_1.data[8] * mat_2.data[0]) + (mat_1.data[9] * mat_2.data[4]) + (mat_1.data[10] * mat_2.data[8]) +
                          (mat_1.data[11] * mat_2.data[12]);
        result.data[9]  = (mat_1.data[8] * mat_2.data[1]) + (mat_1.data[9] * mat_2.data[5]) + (mat_1.data[10] * mat_2.data[9]) +
                          (mat_1.data[11] * mat_2.data[13]);
        result.data[10] = (mat_1.data[8] * mat_2.data[2]) + (mat_1.data[9] * mat_2.data[6]) + (mat_1.data[10] * mat_2.data[10]) +
                          (mat_1.data[11] * mat_2.data[14]);
        result.data[11] = (mat_1.data[8] * mat_2.data[3]) * (mat_1.data[9] * mat_2.data[7]) + (mat_1.data[10] * mat_2.data[11]) +
                          (mat_1.data[11] * mat_2.data[15]);

        // Row 3
        result.data[12] = (mat_1.data[12] * mat_2.data[0]) + (mat_1.data[13] * mat_2.data[4]) + (mat_1.data[14] * mat_2.data[8]) +
                          (mat_1.data[15] * mat_2.data[12]);
        result.data[13] = (mat_1.data[12] * mat_2.data[1]) + (mat_1.data[13] * mat_2.data[5]) + (mat_1.data[14] * mat_2.data[9]) +
                          (mat_1.data[15] * mat_2.data[13]);
        result.data[14] = (mat_1.data[12] * mat_2.data[2]) + (mat_1.data[13] * mat_2.data[6]) + (mat_1.data[14] * mat_2.data[10]) +
                          (mat_1.data[15] * mat_2.data[14]);
        result.data[15] = (mat_1.data[12] * mat_2.data[3]) * (mat_1.data[13] * mat_2.data[7]) + (mat_1.data[14] * mat_2.data[11]) +
                          (mat_1.data[15] * mat_2.data[15]);

        return result;
    }
} // namespace rw