//
// Created by david on 29/09/2025.
//

#ifndef SRC_REDWOLF_ANIMATED_HPP
#define SRC_REDWOLF_ANIMATED_HPP

#include <algorithm>
#include <concepts>
#include <type_traits>

namespace rw::core {
    /**
     * @brief Concept to check if a type can be animated.
     */
    template<typename T>
    concept CanBeAnimated = requires(T a, T b, float f) {
        { a + b } -> std::same_as<T>;            // Addition
        { a - b } -> std::same_as<T>;            // Subtraction
        { a* f } -> std::same_as<T>;             // Scalar multiplication
        { a / f } -> std::same_as<T>;            // Scalar division
        { a < b } -> std::convertible_to<bool>;  // Less than comparison
        { a > b } -> std::convertible_to<bool>;  // Greater than comparison
        { a == b } -> std::convertible_to<bool>; // Equality comparison
    };

    /**
     * @brief Class for handling animated values.
     * @tparam T Type of the animated value. Must support arithmetic operations and comparisons.
     */
    template<CanBeAnimated T>
    class Animated {
     public:
        /**
         * @brief Constructor.
         */
        Animated() = default;

        /**
         * @brief Construct an animated type.
         * @param initial_value Initial value of the animation.
         * @param speed Speed of the animation. [units/s] This should be positive.
         */
        explicit Animated(T&& initial_value, T&& speed) : value_{ initial_value }, speed_{ speed } {};

        /**
         * @brief Set the value immediately, without animation.
         * @param value Value to set.
         */
        void immediately_set_value(T&& value) {
            value_  = value;
            target_ = value;
        };

        /**
         * @brief Set the animation speed.
         * @param speed Animation speed. [units/s] This should be positive.
         */
        void set_speed(T&& speed) {
            speed_ = speed;
        }

        /**
         * @brief Set the target value.
         * @param target Value to reach through animation.
         */
        void set_target(T&& target) {
            target_ = target;
        }

        /**
         * @brief Get the animation speed.
         * @return Animation speed. [units/s]
         */
        [[nodiscard]] const T& speed() const {
            return speed_;
        }

        /**
         * @brief Get the target value.
         * @return Target value.
         */
        [[nodiscard]] const T& target() const {
            return target_;
        }

        /**
         * @brief Update the animated value towards the target value.
         * @param delta_time Time elapsed since the last update in seconds.
         */
        void update(const float delta_time) {
            if (value_ < target_) {
                value_ = std::min(value_ + static_cast<T>(speed_ * delta_time), target_);
            } else if (value_ > target_) {
                value_ = std::max(value_ - static_cast<T>(speed_ * delta_time), target_);
            }
        }

        /**
         * @brief Get the current animated value.
         * @return Current value.
         */
        [[nodiscard]] const T& value() const {
            return value_;
        }

     private:
        T value_{};  /**< Current value. */
        T target_{}; /**< Target value. */
        T speed_{};  /**< Speed at which to animate towards the target value. [units/s] */
    };
} // namespace rw::core

#endif // SRC_REDWOLF_ANIMATED_HPP
