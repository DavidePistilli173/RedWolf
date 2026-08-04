#pragma once

#include "redwolf/common.hpp"

#include <array>
#include <chrono>
#include <format>
#include <print>
#include <source_location>

namespace rw {
    /**
     * @brief Concept for objects that can be used as std::format arguments.
     */
    template<typename T>
    concept IsFormattable = std::is_default_constructible_v<std::formatter<T>>;

    /**
     * @brief Logging levels.
     */
    enum class LogLevel : u8 { fatal, error, warn, info, trace };

    /**
     * @brief Engine logger.
     */
    class Logger {
     public:
        static constexpr usize max_message_size{ 2048 }; /**< Maximum size of a log message, with null terminator. */

        template<usize N>
        struct FixedString {
            std::array<char, N> data{};

            consteval FixedString(const std::array<char, N>& str) : data{ str } {}

            consteval operator std::string_view() const {
                return { data.data(), N - 1 };
            }
        };

        /**
         * @brief Initialise the logger.
         */
        static void init();

        /**
         * @brief Get the logger's instance.
         * @return Logger's instance.
         */
        [[nodiscard]] static Logger* instance();

        /**
         * @brief Base message logging function, should not be used directly.
         * @tparam FmtMsg Message to log, with std::format syntax.
         * @tparam Args Optional argument type for the format string.
         * @param level Logging level.
         * @param args Optional arguments for the format string.
         * @param loc Source code location that originated the log.
         */
        template<rw::IsFormattable... Args>
        void message_base(
            const LogLevel              level,
            std::format_string<Args...> fmt,
            Args&&... args,
            std::source_location loc = std::source_location::current()) {
            std::array<char, max_message_size> formatted_msg{};

            // Format the header.
            const auto header_res{ std::format_to_n(
                formatted_msg.data(),
                formatted_msg.size() - 1,
                "[{:10.6}]({}:{}) - ",
                static_cast<f64>(
                    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time_)
                        .count()) *
                    microseconds_to_seconds,
                loc.file_name(),
                loc.line()) };
            if (header_res.size > max_message_size - 1) {
                return;
            }

            const usize remaining_size{ formatted_msg.size() - header_res.size - 1 };
            const auto  final_res{ std::format_to_n(
                header_res.out, formatted_msg.size() - header_res.size - 1, fmt, std::forward<Args>(args)...) };
            if (final_res.size > remaining_size) {
                return;
            }
            *final_res.out = '\0';

            switch (level) {
            case LogLevel::trace:
                std::println("\033[0m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::info:
                std::println("\033[32m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::warn:
                std::println("\033[33m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::error:
                std::println("\033[31m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            case LogLevel::fatal:
                std::println("\033[35m{}\033[0m", std::string_view(formatted_msg.data(), header_res.size + final_res.size));
                break;
            }
        }

     private:
        /**
         * @brief Default constructor.
         */
        Logger();

        /**
         * @brief Starting time of the logger.
         */
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_{ std::chrono::high_resolution_clock::now() };
    };

    template<rw::IsFormattable... Args>
    void trace(std::format_string<Args...> fmt, Args&&... args, std::source_location src_loc = std::source_location::current()) {
        Logger::instance()->message_base(LogLevel::trace, fmt, std::forward<Args>(args)..., src_loc);
    }
} // namespace rw
