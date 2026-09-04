#pragma once

#include "redwolf/common.hpp"

#include <array>

namespace rw {
    /**
     * @brief Random number generator.
     * @details Currently global for the entire engine, uses xoshiro256**.
     */
    class Random {
     public:
        Random() = default;

        /**
         * @brief Initialise the random number generator.
         * @param seed Seed to use for initialising the generator.
         */
        static void init(u64 seed);

        /**
         * @brief Get a random f32 number in the [0,1] range.
         */
        [[nodiscard]] static f32 rand_f32();

        /**
         * @brief Get a random f32 number in the [min, max] range.
         */
        [[nodiscard]] static f32 rand_f32_in_range(f32 min, f32 max);

        /**
         * @brief Get a random f64 number in the [0,1] range.
         */
        [[nodiscard]] static f64 rand_f64();

        /**
         * @brief Get a random i32 number in the [min, max] range.
         */
        [[nodiscard]] static i32 rand_i32_in_range(i32 min, i32 max);

        /**
         * @brief Get a random u32 number across the full u32 range.
         */
        [[nodiscard]] static u32 rand_u32();

        /**
         * @brief Get a random u32 number in the [min, max] range.
         */
        [[nodiscard]] static u32 rand_u32_in_range(u32 min, u32 max);

        /**
         * @brief Get a random u64 number across the full u64 range.
         */
        [[nodiscard]] static u64 rand_u64();

        /**
         * @brief Shutdown the random number generator.
         */
        static void shutdown();

     private:
        /**
         * @brief Compute the next seed value.
         * @param state Current seed computation state.
         */
        [[nodiscard]] static u64 next_seed_(u64& state);

        /**
         * @brief Bitwise left rotation.
         */
        static u64 rotl_(u64 x, int k);

        std::array<u64, 4> state_{};                /**< Current state of the generator. */
        u64                cached_u64_     = 0;     /**< Cache of a generated u64 for u32 random number generation. */
        bool               has_cached_u32_ = false; /**< If true, the cached u64 number is valid. */
    };
} // namespace rw