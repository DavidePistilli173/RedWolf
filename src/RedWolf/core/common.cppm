module;

export module redwolf.core.common;

export namespace rw {
    constexpr float seconds_to_milliseconds{ 1e3F };      /**< Conversion factor between seconds and milliseconds. */
    constexpr float nanoseconds_to_milliseconds{ 1e-6F }; /**< Conversion factor between nanoseconds and milliseconds. */
    constexpr float nanoseconds_to_seconds{ 1e-9F };      /**< Conversion factor between nanoseconds and seconds. */
} // namespace rw
