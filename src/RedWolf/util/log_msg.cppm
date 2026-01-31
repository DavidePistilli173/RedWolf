module;

#include <filesystem>
#include <format>
#include <source_location>
#include <string>
#include <string_view>

export module redwolf.util.log_msg;

import redwolf.time.date_time;

export namespace rw::util {
    /**
     * @brief Struct containing a log message.
     */
    struct LogMsg {
        /**
         * @brief Logging levels.
         */
        enum class Level { trace = 0, info = 1, warning = 2, error = 3, fatal = 4 };

        /**
         * @brief Construct a new log message with the current date and time.
         * @param lvl Logging level of the message.
         * @param src_loc Location in the source code that generated the message.
         * @param msg Actual message.
         */
        LogMsg(Level lvl, std::source_location src_loc, std::string_view msg) :
            level{ lvl }, date_time{ std::chrono::system_clock::now() }, source_location{ src_loc }, message{ msg } {}

        Level              level{ Level::trace }; /**< Level of the message. */
        rw::time::DateTime date_time;             /**< Date and time of the message. */
        /**
         * @brief Info about the source code location where the message was generated.
         */
        std::source_location source_location{ std::source_location::current() };
        std::string          message; /**< Actual message. */
    };
} // namespace rw::util

/**
 * @brief std::formatter specialization for rw::utils::LogMsg::Level
 */
template<>
struct std::formatter<rw::util::LogMsg::Level> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rw::util::LogMsg::Level lvl, std::format_context& ctx) const {
        const std::string val{ std::to_string(static_cast<int>(lvl)) };
        return std::format_to(ctx.out(), "{}", val);
    }
};

/**
 * @brief std::formatter specialization for rw::dat::LogMsg
 */
template<>
struct std::formatter<rw::util::LogMsg> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    auto format(const rw::util::LogMsg& msg, std::format_context& ctx) const {
        std::filesystem::path file_path{ msg.source_location.file_name() };

        return std::format_to(
            ctx.out(),
            "[{}] - <{}> ({}::{}) {}",
            msg.level,
            msg.date_time,
            file_path.filename().string(),
            msg.source_location.line(),
            msg.message);
    }
};
