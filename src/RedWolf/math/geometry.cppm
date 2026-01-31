module;

export module redwolf.math.geometry;

export namespace rw::math {
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
