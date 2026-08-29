#pragma once

#include "redwolf/common.hpp"

#include <cmath>
#include <type_traits>

namespace rw {
    /**
     * @brief Utility for templated math functions.
     */
    template<typename T>
        requires std::is_integral_v<T>
    [[nodiscard]] constexpr bool are_equal(T value_1, T value_2) {
        return value_1 == value_2;
    }

    /**
     * @brief Check if two floating point values are equal.
     * @param value_1 First value to check.
     * @param value_2 Second value to check.
     * @return true if the values are effectively equal, false otherwise.
     */
    template<typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr bool are_equal(T value_1, T value_2) {
        return std::abs(value_1 - value_2) <= float_epsilon;
    }

    /**
     * @brief Check whether a value is a power of two or not.
     * @param value Value to check.
     * @return true if the value is a power of two, false otherwise.
     */
    template<typename T>
        requires std::is_integral_v<T>
    [[nodiscard]] constexpr bool is_power_of_two(T value) {
        return (0 != value) && (0 == (value & (value - 1)));
    }

    /**
     * @brief Check whether a value is a power of two or not.
     * @param value Value to check.
     * @return true if the value is a power of two, false otherwise.
     */
    template<typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr bool is_power_of_two(T value) {
        const auto integral{ std::round(value) };
        if (!float_equals(value, static_cast<T>(integral))) {
            return false;
        }

        return is_power_of_two(integral);
    }

    /**
     * @brief Get a random number.
     */
    template<typename T>
    [[nodiscard]] T random() {}

    /**
     * @brief Get a random number in the [min, max] range.
     */
    template<typename T>
    [[nodiscard]] T random_in_range(T min, T max) {}
} // namespace rw