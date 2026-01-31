module;

#include <chrono>
#include <format>

export module redwolf.time.date_time;

import redwolf.common;

export namespace rw::time {
    /**
     * @brief Class containing date and time information.
     */
    class DateTime {
     public:
        /**
         * @brief Construct a DateTime object from a given time point.
         * @tparam T Type of clock the time point is based on.
         * @param time_point Time point from which to create the object.
         */
        template<rw::time::IsChronoClock T>
        explicit DateTime(std::chrono::time_point<T> time_point) :
            date_{ std::chrono::floor<std::chrono::days>(time_point) },
            time_{ std::chrono::duration_cast<std::chrono::nanoseconds>(T::now() - std::chrono::floor<std::chrono::days>(time_point)) } {}

        /**
         * @brief Get the current date.
         * @return Current date.
         */
        [[nodiscard]] std::chrono::year_month_day date() const {
            return date_;
        }

        /**
         * @brief Get the current day.
         * @return Current day.
         */
        [[nodiscard]] std::chrono::day day() const {
            return date_.day();
        }

        /**
         * @brief Get the current hour of the day.
         * @return Current hour of the day.
         */
        [[nodiscard]] std::chrono::hours hour() const {
            return time_.hours();
        }

        /**
         * @brief Get the current minute.
         * @return Current minute.
         */
        [[nodiscard]] std::chrono::minutes minute() const {
            return time_.minutes();
        }

        /**
         * @brief Get the current month.
         * @return Current month.
         */
        [[nodiscard]] std::chrono::month month() const {
            return date_.month();
        }

        /**
         * @brief Get the current second.
         * @return Current second.
         */
        [[nodiscard]] std::chrono::seconds second() const {
            return time_.seconds();
        }

        /**
         * @brief Get the current time.
         * @return Current time.
         */
        [[nodiscard]] std::chrono::hh_mm_ss<std::chrono::nanoseconds> time() const {
            return time_;
        }

        /**
         * @brief Get the current year.
         * @return Current year.
         */
        [[nodiscard]] std::chrono::year year() const {
            return date_.year();
        }

     private:
        std::chrono::year_month_day                     date_{}; /**< Date. */
        std::chrono::hh_mm_ss<std::chrono::nanoseconds> time_;   /**< Time. */
    };
} // namespace rw::time

/**
 * @brief std::formatter specialization for rw::time::DateTime
 */
template<>
struct std::formatter<rw::time::DateTime> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::time::DateTime& date_time, std::format_context& ctx) const {
        const auto& ymd = date_time.date();
        const auto& hms = date_time.time();
        return std::format_to(
            ctx.out(),
            "{:04}-{:02}-{:02}|{:02}:{:02}:{:02}.{:09}",
            int(ymd.year()),
            unsigned(ymd.month()),
            unsigned(ymd.day()),
            hms.hours().count(),
            hms.minutes().count(),
            hms.seconds().count(),
            hms.subseconds().count());
    }
};
