#pragma once

#include "math_functions.hpp"
#include "redwolf/common.hpp"

namespace rw {
    /**
     * @brief 3-component vector.
     */
    template<typename T>
    struct Vector3 {
        T x{ 0.0F };
        T y{ 0.0F };
        T z{ 0.0F };

        /**
         * @brief Equality.
         */
        [[nodiscard]] constexpr bool operator==(const Vector3& value) const {
            return are_equal(x, value.x) && are_equal(y, value.y) && are_equal(z, value.z);
        }

        /**
         * @brief Sum.
         */
        [[nodiscard]] constexpr Vector3 operator+(const Vector3& value) const {
            return Vector3{ .x = x + value.x, .y = y + value.y, .z = z + value.z };
        }

        /**
         * @brief Sum.
         */
        constexpr Vector3& operator+=(const Vector3& value) {
            x += value.x;
            y += value.y;
            z += value.z;
            return *this;
        }

        /**
         * @brief Subtract.
         */
        [[nodiscard]] constexpr Vector3 operator-(const Vector3& value) const {
            return Vector3{ .x = x - value.x, .y = y - value.y, .z = z - value.z };
        }

        /**
         * @brief Subtract.
         */
        constexpr Vector3& operator-=(const Vector3& value) {
            x -= value.x;
            y -= value.y;
            z -= value.z;
            return *this;
        }

        /**
         * @brief Multiply by vector.
         */
        [[nodiscard]] constexpr Vector3 operator*(const Vector3& value) const {
            return Vector3{ .x = x * value.x, .y = y * value.y, .z = z * value.z };
        }

        /**
         * @brief Multiply by scalar.
         */
        [[nodiscard]] constexpr Vector3 operator*(T scalar) {
            return Vector3{ .x = x * scalar, .y = y * scalar, .z = z * scalar };
        }

        /**
         * @brief Multiply by vector.
         */
        constexpr Vector3& operator*=(const Vector3& value) {
            x *= value.x;
            y *= value.y;
            z *= value.z;
            return *this;
        }

        /**
         * @brief Multiply by scalar.
         */
        constexpr Vector3& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        /**
         * @brief Divide.
         */
        [[nodiscard]] constexpr Vector3 operator/(const Vector3& value) const {
            return Vector3{ .x = x / value.x, .y = y / value.y, .z = z / value.z };
        }

        /**
         * @brief Divide.
         */
        constexpr Vector3& operator/=(const Vector3& value) {
            x /= value.x;
            y /= value.y;
            z /= value.z;
            return *this;
        }

        /**
         * @brief Create the back vector.
         */
        [[nodiscard]] static constexpr Vector3 back() {
            return Vector3{ .x = static_cast<T>(0), .y = static_cast<T>(0), .z = static_cast<T>(1) };
        }

        /**
         * @brief Create the down vector.
         */
        [[nodiscard]] static constexpr Vector3 down()
            requires std::is_signed_v<T>
        {
            return Vector3{ .x = static_cast<T>(0), .y = static_cast<T>(-1), .z = static_cast<T>(0) };
        }

        /**
         * @brief Create the forward vector.
         */
        [[nodiscard]] static constexpr Vector3 forward()
            requires std::is_signed_v<T>
        {
            return Vector3{ .x = static_cast<T>(0), .y = static_cast<T>(0), .z = static_cast<T>(-1) };
        }

        /**
         * @brief Create the left vector.s
         */
        [[nodiscard]] static constexpr Vector3 left()
            requires std::is_signed_v<T>
        {
            return Vector3{ .x = static_cast<T>(-1), .y = static_cast<T>(0), .z = static_cast<T>(0) };
        }

        /**
         * @brief Get the lenght of the vector.s
         */
        [[nodiscard]] constexpr T length() const {
            return std::sqrt((x * x) + (y * y) + (z * z));
        }

        /**
         * @brief Get the lenght of the vector squared.
         */
        [[nodiscard]] constexpr T length_squared() const {
            return (x * x) + (y * y) + (z * z);
        }

        /**
         * @brief Normalize the vector in-place sto have unit length.
         */
        constexpr void normalize() {
            const T curr_length{ length() };
            x /= curr_length;
            y /= curr_length;
            z /= curr_length;
        }

        /**
         * @brief Get a normalized copy of the vector.
         */
        [[nodiscard]] constexpr Vector3 normalized() const {
            Vector3 result{ *this };
            result.normalize();
            return result;
        }

        /**
         * @brief Create the unit vector.
         */
        [[nodiscard]] static constexpr Vector3 one() {
            return Vector3{ .x = static_cast<T>(1), .y = static_cast<T>(1), .z = static_cast<T>(1) };
        }

        /**
         * @brief Create the right vector.s
         */
        [[nodiscard]] static constexpr Vector3 right() {
            return Vector3{ .x = static_cast<T>(1), .y = static_cast<T>(0), .z = static_cast<T>(0) };
        }

        /**
         * @brief Create the up vector.
         */
        [[nodiscard]] static constexpr Vector3 up() {
            return Vector3{ .x = static_cast<T>(0), .y = static_cast<T>(1), .z = static_cast<T>(0) };
        }
    };

    /**
     * @brief Multiply by scalar.
     */
    template<typename T>
    [[nodiscard]] constexpr Vector3<T> operator*(T scalar, const Vector3<T> vec) {
        return vec * scalar;
    }

    /**
     * @brief Compute the cross product between two vectors.
     */
    template<typename T>
    [[nodiscard]] constexpr Vector3<T> cross(const Vector3<T>& vec_1, const Vector3<T>& vec_2) {
        return Vector3<T>{ .x = (vec_1.y * vec_2.z) - (vec_1.z * vec_2.y),
                           .y = (vec_1.z * vec_2.x) - (vec_1.x * vec_2.z),
                           .z = (vec_1.x * vec_2.y) - (vec_1.y * vec_2.x) };
    }

    /**
     * @brief Compute the dot product between two vectors.
     */
    template<typename T>
    [[nodiscard]] constexpr T dot(const Vector3<T>& vec_1, const Vector3<T>& vec_2) {
        T result{};
        result += (vec_1.x * vec_2.x);
        result += (vec_1.y * vec_2.y);
        result += (vec_1.z * vec_2.z);
        return result;
    }

    /**
     * @brief Calculate the distance between two vectors.
     * @param source Source vector.
     * @param destination Destination vector.
     * @return ||destination - source||
     */
    template<typename T>
    [[nodiscard]] constexpr T distance(const Vector3<T>& destination, const Vector3<T>& source) {
        Vector3<T> diff{ destination - source };
        return diff.length();
    }
} // namespace rw