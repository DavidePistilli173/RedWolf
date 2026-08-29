#pragma once

#include "math_functions.hpp"

#include <cmath>
#include <type_traits>

namespace rw {
    /**
     * @brief 2-component vector.
     */
    template<typename T>
    struct Vector2 {
        T x{ 0.0F };
        T y{ 0.0F };

        /**
         * @brief Equality.
         */
        [[nodiscard]] constexpr bool operator==(const Vector2& value) const {
            return are_equal(x, value.x) && are_equal(y, value.y);
        }

        /**
         * @brief Sum.
         */
        [[nodiscard]] constexpr Vector2 operator+(const Vector2& value) const {
            return Vector2{ .x = x + value.x, .y = y + value.y };
        }

        /**
         * @brief Sum.
         */
        constexpr Vector2& operator+=(const Vector2& value) {
            x += value.x;
            y += value.y;
            return *this;
        }

        /**
         * @brief Subtract.
         */
        [[nodiscard]] constexpr Vector2 operator-(const Vector2& value) const {
            return Vector2{ .x = x - value.x, .y = y - value.y };
        }

        /**
         * @brief Subtract.
         */
        constexpr Vector2& operator-=(const Vector2& value) {
            x -= value.x;
            y -= value.y;
            return *this;
        }

        /**
         * @brief Multiply by vector.
         */
        [[nodiscard]] constexpr Vector2 operator*(const Vector2& value) const {
            return Vector2{ .x = x * value.x, .y = y * value.y };
        }

        /**
         * @brief Multiply by scalar.
         */
        [[nodiscard]] constexpr Vector2 operator*(T scalar) {
            return Vector2{ .x = x * scalar, .y = y * scalar };
        }

        /**
         * @brief Multiply by vector.
         */
        constexpr Vector2& operator*=(const Vector2& value) {
            x *= value.x;
            y *= value.y;
            return *this;
        }

        /**
         * @brief Multiply by scalar.
         */
        constexpr Vector2& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /**
         * @brief Divide.
         */
        [[nodiscard]] constexpr Vector2 operator/(const Vector2& value) const {
            return Vector2{ .x = x / value.x, .y = y / value.y };
        }

        /**
         * @brief Divide.
         */
        constexpr Vector2& operator/=(const Vector2& value) {
            x /= value.x;
            y /= value.y;
            return *this;
        }

        /**
         * @brief Create the down vector.
         */
        [[nodiscard]] static constexpr Vector2 down()
            requires std::is_signed_v<T>
        {
            return Vector2{ .x = static_cast<T>(0), .y = static_cast<T>(-1) };
        }

        /**
         * @brief Create the left vector.s
         */
        [[nodiscard]] static constexpr Vector2 left()
            requires std::is_signed_v<T>
        {
            return Vector2{ .x = static_cast<T>(-1), .y = static_cast<T>(0) };
        }

        /**
         * @brief Get the lenght of the vector.s
         */
        [[nodiscard]] constexpr T length() const {
            return std::sqrt((x * x) + (y * y));
        }

        /**
         * @brief Get the lenght of the vector squared.
         */
        [[nodiscard]] constexpr T length_squared() const {
            return (x * x) + (y * y);
        }

        /**
         * @brief Normalize the vector in-place sto have unit length.
         */
        constexpr void normalize() {
            const T curr_length{ length() };
            x /= curr_length;
            y /= curr_length;
        }

        /**
         * @brief Get a normalized copy of the vector.
         */
        [[nodiscard]] constexpr Vector2 normalized() const {
            Vector2 result{ *this };
            result.normalize();
            return result;
        }

        /**
         * @brief Create the unit vector.
         */
        [[nodiscard]] static constexpr Vector2 one() {
            return Vector2{ .x = static_cast<T>(1), .y = static_cast<T>(1) };
        }

        /**
         * @brief Create the right vector.s
         */
        [[nodiscard]] static constexpr Vector2 right() {
            return Vector2{ .x = static_cast<T>(1), .y = static_cast<T>(0) };
        }

        /**
         * @brief Create the up vector.
         */
        [[nodiscard]] static constexpr Vector2 up() {
            return Vector2{ .x = static_cast<T>(0), .y = static_cast<T>(1) };
        }
    };

    /**
     * @brief Multiply by scalar.
     */
    template<typename T>
    [[nodiscard]] constexpr Vector2<T> operator*(T scalar, const Vector2<T> vec) {
        return vec * scalar;
    }

    /**
     * @brief Calculate the distance between two vectors.
     * @param source Source vector.
     * @param destination Destination vector.
     * @return ||destination - source||
     */
    template<typename T>
    [[nodiscard]] constexpr T distance(const Vector2<T>& destination, const Vector2<T>& source) {
        Vector2<T> diff{ destination - source };
        return diff.length();
    }
} // namespace rw