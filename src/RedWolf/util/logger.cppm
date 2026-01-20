module;

#include <atomic>
#include <format>
#include <memory>
#include <mutex>
#include <print>
#include <source_location>
#include <string_view>

export module redwolf.util.logger;

import redwolf.common;
import redwolf.util.log_msg;

export namespace rw::util {
    /**
     * @brief Singleton, thread-safe logger.
     */
    class Logger {
     public:
        using Level = LogMsg::Level;

        /**
         * @brief Utility structure for storing a message and the source code location that generated.
         */
        struct MessageWithLocation {
            /**
             * @brief Constructor. It is not made explicit on purpose, so that it is transparent to the user.
             * @param p_msg Text message with ftm formatting syntax.
             * @param p_loc Source code location data.
             */
            template<typename T>
            MessageWithLocation(T p_msg, const std::source_location p_loc = std::source_location::current()) : txt{ p_msg }, loc{ p_loc } {}

            std::string_view     txt; /**< Text message with formatting syntax. */
            std::source_location loc; /**< Source code location data. */
        };

        /**
         * @brief Log an error message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void err(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::error, msg, args...);
        }

        /**
         * @brief Log a fatal message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void fatal(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::fatal, msg, args...);
        }

        /**
         * @brief Get the instance of the logger.
         * @return Instance of the logger.
         */
        [[nodiscard]] static Logger& get() {
            static Logger instance;
            return instance;
        }

        /**
         * @brief Log an information message.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void info(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::info, msg, args...);
        }

        /**
         * @brief Get the current log level.
         * @return Current log level.
         */
        [[nodiscard]] Level level() const {
            return level_;
        }

        /**
         * @brief Base message logging function, should not be used directly.
         * @tparam Args Optional argument type for the format string.
         * @param level Logging level.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments for the format string.
         */
        template<rw::core::IsFormattable... Args>
        void message_base(const Level level, const MessageWithLocation& msg, const Args&... args) {
            if (level >= level_) {
                LogMsg message{ level, msg.loc, std::vformat(msg.txt, std::make_format_args(args...)) };

                std::scoped_lock lck{ mtx_ };

                switch (level) {
                case Level::trace:
                    std::println("\033[0m{}\033[0m", message);
                    break;
                case Level::info:
                    std::println("\033[32m{}\033[0m", message);
                    break;
                case Level::warning:
                    std::println("\033[33m{}\033[0m", message);
                    break;
                case Level::error:
                    std::println("\033[31m{}\033[0m", message);
                    break;
                case Level::fatal:
                    std::println("\033[35m{}\033[0m", message);
                    break;
                }
            }
        }

        /**
         * @brief Set the logging level for release builds.
         * @param level New logging level for release builds.
         */
        void set_level(Level level) {
            info("Logger level changed from {} to {}.", level_.load(), level);
            level_ = level;
        }

        /**
         * @brief Log a trace message only in debug builds.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void trace(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::trace, msg, args...);
        }

        /**
         * @brief Log a warning message only in debug builds.
         * @param msg Message to log. Supports the std::format syntax.
         * @param args Optional arguments to insert into the message.
         */
        template<rw::core::IsFormattable... Args>
        void warn(const MessageWithLocation& msg, const Args&... args) {
            message_base(Level::warning, msg, args...);
        }

     private:
        /**
         * @brief Default constructor.
         */
        Logger() {
            info("Logger created.");
        }

        std::mutex         mtx_;                   /**< Logging mutex. */
        std::atomic<Level> level_{ Level::trace }; /**< Logging level. */
    };
} // namespace rw::util
