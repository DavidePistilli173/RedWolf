#pragma once

#include "vec_2.hpp"
#include "vec_3.hpp"
#include "vec_4.hpp"

namespace rw {
    /**
     * @brief Convert a Vec4 into a Vec3 by dropping the fourth coordinate.
     */
    template<typename T>
    [[nodiscard]] Vec3<T> to_vector3(const Vec4<T>& vec) {
        return Vec3<T>{ .x = vec.x, .y = vec.y, .z = vec.z };
    }

    /**
     * @brief Convert a Vec3 into a Vec4 by adding a user-specified w coordinate.
     */
    template<typename T>
    [[nodiscard]] Vec4<T> to_vector4(const Vec3<T>& vec, T w) {
        return Vec4<T>{ .x = vec.x, .y = vec.y, .z = vec.z, .w = w };
    }
} // namespace rw