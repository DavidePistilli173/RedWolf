#pragma once

#include "vector_2.hpp"
#include "vector_3.hpp"
#include "vector_4.hpp"

namespace rw {
    /**
     * @brief Convert a Vector4 into a Vector3 by dropping the fourth coordinate.
     */
    template<typename T>
    [[nodiscard]] Vector3<T> to_vector3(const Vector4<T>& vec) {
        return Vector3<T>{ .x = vec.x, .y = vec.y, .z = vec.z };
    }

    /**
     * @brief Convert a Vector3 into a Vector4 by adding a user-specified w coordinate.
     */
    template<typename T>
    [[nodiscard]] Vector4<T> to_vector4(const Vector3<T>& vec, T w) {
        return Vector4<T>{ .x = vec.x, .y = vec.y, .z = vec.z, .w = w };
    }
} // namespace rw