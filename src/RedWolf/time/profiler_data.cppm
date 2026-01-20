module;

#include <chrono>
#include <limits>
#include <string_view>

export module redwolf.time.profiler_data;

import redwolf.common;

export namespace rw::time {
    /**
     * @brief Time profiler data.
     */
    struct ProfilerData {
        /**
         * @brief Constructor.
         * @param p_name Readable name of the profiler. This must remain valid for the entire lifetime of the profiler data.
         */
        explicit ProfilerData(const std::string_view p_name) : name{ p_name } {}

        /**
         * @brief Get the last recorded elapsed time in milliseconds.
         * @return Last recorded elapsed time in milliseconds.
         */
        [[nodiscard]] float elapsed_last_milliseconds() const {
            return static_cast<float>(elapsed_last.count()) * nanoseconds_to_milliseconds;
        }
#
        /**
         * @brief Get the minimum recorded elapsed time in milliseconds.
         * @return Minimum recorded elapsed time in milliseconds.
         */
        [[nodiscard]] float elapsed_min_milliseconds() const {
            return static_cast<float>(elapsed_min.count()) * nanoseconds_to_milliseconds;
        }

        /**
         * @brief Get the maximum recorded elapsed time in milliseconds.
         * @return Maximum recorded elapsed time in milliseconds.
         */
        [[nodiscard]] float elapsed_max_milliseconds() const {
            return static_cast<float>(elapsed_max.count()) * nanoseconds_to_milliseconds;
        }

        std::string_view name; /**< Readable name for the profiling data. */

        std::chrono::nanoseconds elapsed_min{ std::numeric_limits<int64_t>::max() }; /**< Minimum elapsed time. */
        std::chrono::nanoseconds elapsed_last{ 0 };                                  /**< Last elapsed time. */
        std::chrono::nanoseconds elapsed_max{ 0 };                                   /**< Maximum elapsed time. */
    };
} // namespace rw::time
