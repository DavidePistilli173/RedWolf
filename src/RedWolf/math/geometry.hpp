//
// Created by david on 13/10/2025.
//

#ifndef SRC_REDWOLF_GEOMETRY_HPP
#define SRC_REDWOLF_GEOMETRY_HPP

namespace rw::math {
    /**
     * @brief Struct representing a rectangle.
     * @tparam T Type of the underlying data.
     */
    template<typename T>
    struct Rect {
        T x{};      /**< X-coordinate of the bottom-left corner of the rectangle. */
        T y{};      /**< Y-coordinate of the bottom-left corner of the rectangle. */
        T width{};  /**< Width of the rectangle. */
        T height{}; /**< Height of the rectangle. */
    };
} // namespace rw::math

#endif // SRC_REDWOLF_GEOMETRY_HPP
