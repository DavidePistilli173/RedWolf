//
// Created by cjm036653 on 15/08/2025.
//

#ifndef SRC_REDWOLF_POINT_HPP
#define SRC_REDWOLF_POINT_HPP

namespace rw::core {
    /**
     * @brief 2D point.
     * @tparam T Coordinate type.
     */
    template<typename T = double>
    struct Point2D {
        T x{}; /**< X coordinate. */
        T y{}; /**< Y coordinate. */
    };

    /**
     * @brief 3D point.
     * @tparam T Coordinate type.
     */
    template<typename T = double>
    struct Point3D {
        T x{}; /**< X coordinate. */
        T y{}; /**< Y coordinate. */
        T z{}; /**< Z coordinate. */
    };
} // namespace rw::core

#endif // SRC_REDWOLF_POINT_HPP
