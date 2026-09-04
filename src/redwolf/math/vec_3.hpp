#pragma once

#include "math_functions.hpp"
#include "redwolf/common.hpp"

namespace rw {
    /**
     * @brief 3-component vector.
     */
    template<typename T>
    struct Vec3 {
        T x{ 0.0F };
        T y{ 0.0F };
        T z{ 0.0F };

        /**
         * @brief Equality.
         */
        [[nodiscard]] constexpr bool operator==(const Vec3& value) const {
            return are_equal(x, value.x) && are_equal(y, value.y) && are_equal(z, value.z);
        }

        /**
         * @brief Sum.
         */
        [[nodiscard]] constexpr Vec3 operator+(const Vec3& value) const {
            return Vec3{ .x = x + value.x, .y = y + value.y, .z = z + value.z };
        }

        /**
         * @brief Sum.
         */
        constexpr Vec3& operator+=(const Vec3& value) {
            x += value.x;
            y += value.y;
            z += value.z;
            return *this;
        }

        /**
         * @brief Subtract.
         */
        [[nodiscard]] constexpr Vec3 operator-(const Vec3& value) const {
            return Vec3{ .x = x - value.x, .y = y - value.y, .z = z - value.z };
        }

        /**
         * @brief Subtract.
         */
        constexpr Vec3& operator-=(const Vec3& value) {
            x -= value.x;
            y -= value.y;
            z -= value.z;
            return *this;
        }

        /**
         * @brief Multiply by vector.
         */
        [[nodiscard]] constexpr Vec3 operator*(const Vec3& value) const {
            return Vec3{ .x = x * value.x, .y = y * value.y, .z = z * value.z };
        }

        /**
         * @brief Multiply by scalar.
         */
        [[nodiscard]] constexpr Vec3 operator*(T scalar) {
            return Vec3{ .x = x * scalar, .y = y * scalar, .z = z * scalar };
        }

        /**
         * @brief Multiply by vector.
         */
        constexpr Vec3& operator*=(const Vec3& value) {
            x *= value.x;
            y *= value.y;
            z *= value.z;
            return *this;
        }

        /**
         * @brief Multiply by scalar.
         */
        constexpr Vec3& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        /**
         * @brief Divide.
         */
        [[nodiscard]] constexpr Vec3 operator/(const Vec3& value) const {
            return Vec3{ .x = x / value.x, .y = y / value.y, .z = z / value.z };
        }

        /**
         * @brief Divide.
         */
        constexpr Vec3& operator/=(const Vec3& value) {
            x /= value.x;
            y /= value.y;
            z /= value.z;
            return *this;
        }

        /**
         * @brief Create the back vector.
         */
        [[nodiscard]] static constexpr Vec3 back() {
            return Vec3{ .x = 0, .y = 0, .z = static_cast<T>(1) };
        }

        /**
         * @brief Create the down vector.
         */
        [[nodiscard]] static constexpr Vec3 down()
            requires std::is_signed_v<T>
        {
            return Vec3{ .x = 0, .y = static_cast<T>(-1), .z = 0 };
        }

        /**
         * @brief Create the forward vector.
         */
        [[nodiscard]] static constexpr Vec3 forward()
            requires std::is_signed_v<T>
        {
            return Vec3{ .x = 0, .y = 0, .z = static_cast<T>(-1) };
        }

        /**
         * @brief Create the left vector.s
         */
        [[nodiscard]] static constexpr Vec3 left()
            requires std::is_signed_v<T>
        {
            return Vec3{ .x = static_cast<T>(-1), .y = 0, .z = 0 };
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
        [[nodiscard]] constexpr Vec3 normalized() const {
            Vec3 result{ *this };
            result.normalize();
            return result;
        }

        /**
         * @brief Create the unit vector.
         */
        [[nodiscard]] static constexpr Vec3 one() {
            return Vec3{ .x = static_cast<T>(1), .y = static_cast<T>(1), .z = static_cast<T>(1) };
        }

        /**
         * @brief Create the right vector.s
         */
        [[nodiscard]] static constexpr Vec3 right() {
            return Vec3{ .x = static_cast<T>(1), .y = 0, .z = 0 };
        }

        /**
         * @brief Create the up vector.
         */
        [[nodiscard]] static constexpr Vec3 up() {
            return Vec3{ .x = 0, .y = static_cast<T>(1), .z = 0 };
        }
    };

    /**
     * @brief Multiply by scalar.
     */
    template<typename T>
    [[nodiscard]] constexpr Vec3<T> operator*(T scalar, const Vec3<T> vec) {
        return vec * scalar;
    }

    /**
     * @brief Compute the cross product between two vectors.
     */
    template<typename T>
    [[nodiscard]] constexpr Vec3<T> cross(const Vec3<T>& vec_1, const Vec3<T>& vec_2) {
        return Vec3<T>{ .x = (vec_1.y * vec_2.z) - (vec_1.z * vec_2.y),
                        .y = (vec_1.z * vec_2.x) - (vec_1.x * vec_2.z),
                        .z = (vec_1.x * vec_2.y) - (vec_1.y * vec_2.x) };
    }

    /**
     * @brief Compute the dot product between two vectors.
     */
    template<typename T>
    [[nodiscard]] constexpr T dot(const Vec3<T>& vec_1, const Vec3<T>& vec_2) {
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
    [[nodiscard]] constexpr T distance(const Vec3<T>& destination, const Vec3<T>& source) {
        Vec3<T> diff{ destination - source };
        return diff.length();
    }
} // namespace rw