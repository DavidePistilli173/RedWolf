#pragma once

#include "math_functions.hpp"

#include <cmath>
#include <type_traits>

namespace rw {
    /**
     * @brief 2-component vector.
     */
    template<typename T>
    struct Vec2 {
        T x{ 0.0F };
        T y{ 0.0F };

        /**
         * @brief Equality.
         */
        [[nodiscard]] constexpr bool operator==(const Vec2& value) const {
            return are_equal(x, value.x) && are_equal(y, value.y);
        }

        /**
         * @brief Sum.
         */
        [[nodiscard]] constexpr Vec2 operator+(const Vec2& value) const {
            return Vec2{ .x = x + value.x, .y = y + value.y };
        }

        /**
         * @brief Sum.
         */
        constexpr Vec2& operator+=(const Vec2& value) {
            x += value.x;
            y += value.y;
            return *this;
        }

        /**
         * @brief Subtract.
         */
        [[nodiscard]] constexpr Vec2 operator-(const Vec2& value) const {
            return Vec2{ .x = x - value.x, .y = y - value.y };
        }

        /**
         * @brief Subtract.
         */
        constexpr Vec2& operator-=(const Vec2& value) {
            x -= value.x;
            y -= value.y;
            return *this;
        }

        /**
         * @brief Multiply by vector.
         */
        [[nodiscard]] constexpr Vec2 operator*(const Vec2& value) const {
            return Vec2{ .x = x * value.x, .y = y * value.y };
        }

        /**
         * @brief Multiply by scalar.
         */
        [[nodiscard]] constexpr Vec2 operator*(T scalar) {
            return Vec2{ .x = x * scalar, .y = y * scalar };
        }

        /**
         * @brief Multiply by vector.
         */
        constexpr Vec2& operator*=(const Vec2& value) {
            x *= value.x;
            y *= value.y;
            return *this;
        }

        /**
         * @brief Multiply by scalar.
         */
        constexpr Vec2& operator*=(T scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        /**
         * @brief Divide.
         */
        [[nodiscard]] constexpr Vec2 operator/(const Vec2& value) const {
            return Vec2{ .x = x / value.x, .y = y / value.y };
        }

        /**
         * @brief Divide.
         */
        constexpr Vec2& operator/=(const Vec2& value) {
            x /= value.x;
            y /= value.y;
            return *this;
        }

        /**
         * @brief Create the down vector.
         */
        [[nodiscard]] static constexpr Vec2 down()
            requires std::is_signed_v<T>
        {
            return Vec2{ .x = 0, .y = static_cast<T>(-1) };
        }

        /**
         * @brief Create the left vector.s
         */
        [[nodiscard]] static constexpr Vec2 left()
            requires std::is_signed_v<T>
        {
            return Vec2{ .x = static_cast<T>(-1), .y = 0 };
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
        [[nodiscard]] constexpr Vec2 normalized() const {
            Vec2 result{ *this };
            result.normalize();
            return result;
        }

        /**
         * @brief Create the unit vector.
         */
        [[nodiscard]] static constexpr Vec2 one() {
            return Vec2{ .x = static_cast<T>(1), .y = static_cast<T>(1) };
        }

        /**
         * @brief Create the right vector.s
         */
        [[nodiscard]] static constexpr Vec2 right() {
            return Vec2{ .x = static_cast<T>(1), .y = 0 };
        }

        /**
         * @brief Create the up vector.
         */
        [[nodiscard]] static constexpr Vec2 up() {
            return Vec2{ .x = 0, .y = static_cast<T>(1) };
        }
    };

    /**
     * @brief Multiply by scalar.
     */
    template<typename T>
    [[nodiscard]] constexpr Vec2<T> operator*(T scalar, const Vec2<T> vec) {
        return vec * scalar;
    }

    /**
     * @brief Calculate the distance between two vectors.
     * @param source Source vector.
     * @param destination Destination vector.
     * @return ||destination - source||
     */
    template<typename T>
    [[nodiscard]] constexpr T distance(const Vec2<T>& destination, const Vec2<T>& source) {
        Vec2<T> diff{ destination - source };
        return diff.length();
    }
} // namespace rw