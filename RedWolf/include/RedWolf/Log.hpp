#ifndef RW_LOG_HPP
#define RW_LOG_HPP

#include "RedWolf/core.hpp"

#include <spdlog/spdlog.h>

#include <memory>

namespace rw
{
    /** \brief Class for message logging. */
    class RW_API Log
    {
    public:
        /** \brief Name of the engine logger. */
        static constexpr std::string_view core_name{ "REDWOLF" };
        /** \brief Name of the application logger. */
        static constexpr std::string_view app_name{ "APP" };
        /** \brief Formatting pattern for log messages. */
        static constexpr std::string_view pattern{ "%^[%n - %l] - %T:%e\n    %v" };

        /** 
            \brief Initialise the logger. 
            \return true if initialisation was successful, false otherwise.
        */
        static bool init();

        /** 
            \brief Log information from the engine.
            Only active in debug builds (see core.hpp).
            \param msg Information message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
        template <typename... Args>
        static void core_info(std::string_view msg, Args... args)
        {
            if constexpr (rw::debug)
            {
                coreLogger_->info(msg, args...);
            }
        }

        /**
            \brief Log a warning from the engine.
            Only active in debug builds (see core.hpp)
            \param msg Warning message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
        template <typename... Args>
        static void core_warn(std::string_view msg, Args... args)
        {
            if constexpr (rw::debug)
            {
                coreLogger_->warn(msg, args...);
            }
        }

        /**
            \brief Log an error from the engine.
            Only active in debug builds (see core.hpp)
            \param msg Error message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
        template <typename... Args>
        static void core_err(std::string_view msg, Args... args)
        {
            if constexpr (rw::debug)
            {
                coreLogger_->error(msg, args...);
            }
        }

        /**
            \brief Log information from the application.
            Only active in debug builds (see core.hpp)
            \param msg Information message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
        template <typename... Args>
        static void app_info(std::string_view msg, Args... args)
        {
            if constexpr (rw::debug)
            {
                appLogger_->info(msg, args...);
            }
        }

        /**
            \brief Log a warning from the application.
            Only active in debug builds (see core.hpp)
            \param msg Warning message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
        template <typename... Args>
        static void app_warn(std::string_view msg, Args... args)
        {
            if constexpr (rw::debug)
            {
                appLogger_->warn(msg, args...);
            }
        }

        /**
            \brief Log an error from the application.
            Only active in debug builds (see core.hpp)
            \param msg Error message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
        template <typename... Args>
        static void app_err(std::string_view msg, Args... args)
        {
            if constexpr (rw::debug)
            {
                appLogger_->error(msg, args...);
            }
        }

    private:
        /** \brief Engine logger. */
        static std::shared_ptr<spdlog::logger> coreLogger_;
        /** \brief Application logger. */
        static std::shared_ptr<spdlog::logger> appLogger_;
    };
}

#endif