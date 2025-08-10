//
// Created by cjm036653 on 10/08/25.
//

#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

#include "../common.hpp"

#include <chrono>
#include <format>

namespace rw::time {
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
            time_{ std::chrono::duration_cast<std::chrono::seconds>(T::now() - std::chrono::floor<std::chrono::days>(time_point)) } {}

        /**
         * @brief Get the current date.
         * @return Current date.
         */
        [[nodiscard]] std::chrono::year_month_day date() const;

        /**
         * @brief Get the current day.
         * @return Current day.
         */
        [[nodiscard]] std::chrono::day day() const;

        /**
         * @brief Get the current hour of the day.
         * @return Current hour of the day.
         */
        [[nodiscard]] std::chrono::hours hour() const;

        /**
         * @brief Get the current minute.
         * @return Current minute.
         */
        [[nodiscard]] std::chrono::minutes minute() const;

        /**
         * @brief Get the current month.
         * @return Current month.
         */
        [[nodiscard]] std::chrono::month month() const;

        /**
         * @brief Get the current second.
         * @return Current second.
         */
        [[nodiscard]] std::chrono::seconds second() const;

        /**
         * @brief Get the current time.
         * @return Current time.
         */
        [[nodiscard]] std::chrono::hh_mm_ss<std::chrono::seconds> time() const;

        /**
         * @brief Get the current year.
         * @return Current year.
         */
        [[nodiscard]] std::chrono::year year() const;

     private:
        std::chrono::year_month_day                 date_{}; /**< Date. */
        std::chrono::hh_mm_ss<std::chrono::seconds> time_;   /**< Time. */
    };
} // namespace rw::time

/**
 * @brief std::formatter specialization for rw::time::DateTime
 */
template<>
struct std::formatter<rw::time::DateTime> : std::formatter<std::string> {
    auto format(const rw::time::DateTime& date_time, std::format_context& ctx) const {
        return std::formatter<std::string>::format(std::format("{}|{}", date_time.date(), date_time.time()), ctx);
    }
};

#endif // DATE_TIME_HPP
