#pragma once

#include <cstdint>
#include <limits>
#include <numbers>

using u8    = std::uint8_t;
using u16   = std::uint16_t;
using u32   = std::uint32_t;
using u64   = std::uint64_t;
using i8    = std::int8_t;
using i16   = std::int16_t;
using i32   = std::int32_t;
using i64   = std::int64_t;
using usize = std::size_t;
using isize = std::ptrdiff_t;
using f32   = float;
using f64   = double;

static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(i8) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(usize) == sizeof(void*));
static_assert(sizeof(isize) == sizeof(void*));
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

namespace rw {
    // Mathematical constants.
    constexpr f32 pi{ std::numbers::pi_v<f32> };
    constexpr f32 pi_2{ 2.0F * pi };
    constexpr f32 half_pi{ 0.5F * pi };
    constexpr f32 quarter_pi{ 0.25F * pi };
    constexpr f32 one_over_pi{ 1.0F / pi };
    constexpr f32 one_over_two_pi{ 1.0F / pi_2 };
    constexpr f32 sqrt_2{ std::numbers::sqrt2_v<f32> };
    constexpr f32 sqrt_3{ std::numbers::sqrt3_v<f32> };
    constexpr f32 sqrt_one_over_two{ 0.70710678118654752440F };
    constexpr f32 sqrt_one_over_three{ std::numbers::inv_sqrt3_v<f32> };
    constexpr f32 infinity{ std::numeric_limits<f32>::infinity() };
    constexpr f32 float_epsilon{ 1.192092896e-7F }; /**< Small number for floating point comparisons. */

    // Angle conversion factors.
    constexpr f32 deg_to_rad{ pi / 180.0F }; /**< [deg] -> [rad] */
    constexpr f32 rad_to_deg{ 180.0F / pi }; /**< [rad] -> [deg] */

    // Time conversion factors.
    constexpr f32 seconds_to_milliseconds{ 1000.0F }; /**< [s] -> [ms] */
    constexpr f32 milliseconds_to_seconds{ 1e-3F };   /**< [ms] -> [s] */
    constexpr f32 nanoseconds_to_seconds{ 1e-9F };    /**< [ns] -> [s] */
    constexpr f32 microseconds_to_seconds{ 1e-6F };   /**< [us] -> [s] */

    // Data size conversion factors.
    constexpr f32 bytes_to_gibs{ 1 / (1024.F * 1024.F * 1024.F) }; /**< [B] -> [GiB] */

    /**
     * @brief Version information.
     */
    struct VersionInfo {
        u8  major{ 0 }; /**< Major version number. */
        u16 minor{ 0 }; /**< Minor version number. */
        u16 patch{ 0 }; /**< Patch version number. */
        u64 build{ 0 }; /**< Build version number. */
    };
} // namespace rw