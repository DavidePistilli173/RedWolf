module;

export module redwolf.core.geometry;

import redwolf.core.math;

export namespace rw::core {
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

    /**
     * @brief 2D line segment defined by a start and an end point.
     */
    struct Line {
        Vec2 start{}; /**< Starting point of the line. */
        Vec2 end{};   /**< Ending point of the line. */
    };
} // namespace rw::core
