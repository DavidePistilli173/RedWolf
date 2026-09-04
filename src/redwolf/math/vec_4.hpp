#pragma once

#include "redwolf/common.hpp"

#include <cmath>

namespace rw {
    /**
     * @brief 4-dimensional vector.
     */
    template<typename T>
    struct Vec4 {
        T x{ 0.0F };
        T y{ 0.0F };
        T z{ 0.0F };
        T w{ 0.0F };
        /**
         * @brief Equality.
         */
        [[nodiscard]] constexpr bool operator==(const Vec4& value) const {
            return are_equal(x, value.x) && are_equal(y, value.y) && are_equal(z, value.z) && are_equal(w, value.w);
        }

        /**
         * @brief Sum.
         */
        [[nodiscard]] constexpr Vec4 operator+(const Vec4& value) const {
            return Vec4{ .x = x + value.x, .y = y + value.y, .z = z + value.z, .w = w + value.w };
        }

        /**
         * @brief Sum.
         */
        constexpr Vec4& operator+=(const Vec4& value) {
            x += value.x;
            y += value.y;
            z += value.z;
            w += value.w;
            return *this;
        }

        /**
         * @brief Subtract.
         */
        [[nodiscard]] constexpr Vec4 operator-(const Vec4& value) const {
            return Vec4{ .x = x - value.x, .y = y - value.y, .z = z - value.z, .w = w - value.w };
        }

        /**
         * @brief Subtract.
         */
        constexpr Vec4& operator-=(const Vec4& value) {
            x -= value.x;
            y -= value.y;
            z -= value.z;
            w -= value.w;
            return *this;
        }

        /**
         * @brief Multiply by vector.
         */
        [[nodiscard]] constexpr Vec4 operator*(const Vec4& value) const {
            return Vec4{ .x = x * value.x, .y = y * value.y, .z = z * value.z, .w = w * value.w };
        }

        /**
         * @brief Multiply by scalar.
         */
        [[nodiscard]] constexpr Vec4 operator*(T scalar) {
            return Vec4{ .x = x * scalar, .y = y * scalar, .z = z * scalar, .w = w * scalar };
        }

        /**
         * @brief Multiply by vector.
         */
        constexpr Vec4& operator*=(const Vec4& value) {
            x *= value.x;
            y *= value.y;
            z *= value.z;
            w *= value.w;
            return *this;
        }

        /**
         * @brief Multiply by scalar.
         */
        constexpr Vec4& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        /**
         * @brief Divide.
         */
        [[nodiscard]] constexpr Vec4 operator/(const Vec4& value) const {
            return Vec4{ .x = x / value.x, .y = y / value.y, .z = z / value.z, .w = w / value.w };
        }

        /**
         * @brief Divide.
         */
        constexpr Vec4& operator/=(const Vec4& value) {
            x /= value.x;
            y /= value.y;
            z /= value.z;
            w /= value.w;
            return *this;
        }

        /**
         * @brief Get the lenght of the vector.s
         */
        [[nodiscard]] constexpr T length() const {
            return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
        }

        /**
         * @brief Get the lenght of the vector squared.
         */
        [[nodiscard]] constexpr T length_squared() const {
            return (x * x) + (y * y) + (z * z) + (w * w);
        }

        /**
         * @brief Normalize the vector in-place sto have unit length.
         */
        constexpr void normalize() {
            const T curr_length{ length() };
            x /= curr_length;
            y /= curr_length;
            z /= curr_length;
            w /= curr_length;
        }

        /**
         * @brief Get a normalized copy of the vector.
         */
        [[nodiscard]] constexpr Vec4 normalized() const {
            Vec4 result{ *this };
            result.normalize();
            return result;
        }

        /**
         * @brief Create the unit vector.
         */
        [[nodiscard]] static constexpr Vec4 one() {
            return Vec4{ .x = static_cast<T>(1), .y = static_cast<T>(1), .z = static_cast<T>(1), .w = static_cast<T>(1) };
        }
    };

    /**
     * @brief Multiply by scalar.
     */
    template<typename T>
    [[nodiscard]] constexpr Vec4<T> operator*(T scalar, const Vec4<T> vec) {
        return vec * scalar;
    }

    /**
     * @brief Compute the cross product between two vectors.
     */
    template<typename T>
    [[nodiscard]] constexpr Vec4<T> cross(const Vec4<T>& vec_1, const Vec4<T>& vec_2) {
        return Vec4<T>{ .x = (vec_1.y * vec_2.z) - (vec_1.z * vec_2.y),
                        .y = (vec_1.z * vec_2.x) - (vec_1.x * vec_2.z),
                        .z = (vec_1.x * vec_2.y) - (vec_1.y * vec_2.x) };
    }

    /**
     * @brief Compute the dot product between two vectors.
     */
    template<typename T>
    [[nodiscard]] constexpr T dot(const Vec4<T>& vec_1, const Vec4<T>& vec_2) {
        T result{};
        result += (vec_1.x * vec_2.x);
        result += (vec_1.y * vec_2.y);
        result += (vec_1.z * vec_2.z);
        result += (vec_1.w * vec_2.w);
        return result;
    }

    /**
     * @brief Calculate the distance between two vectors.
     * @param source Source vector.
     * @param destination Destination vector.
     * @return ||destination - source||
     */
    template<typename T>
    [[nodiscard]] constexpr T distance(const Vec4<T>& destination, const Vec4<T>& source) {
        Vec4<T> diff{ destination - source };
        return diff.length();
    }
} // namespace rw