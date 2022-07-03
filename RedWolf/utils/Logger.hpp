#ifndef RW_UTILS_LOG_HPP
#define RW_UTILS_LOG_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/LogMsg.hpp"
#include "RedWolf/io/concepts.hpp"

#include <atomic>
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

namespace rw::utils
{
   /**
    * @brief Custom singleton, thread-safe logger.
    * @details The default log functions only work in debug environments (RW_DEBUG macro).
    *         When using this logger, DO NOT use stdio functions, but only iostream ones (std::ios_base::sync_with_stdio(false)).
    */
   class Logger
   {
   public:
      using LogMsg = rw::dat::LogMsg;
      using Level = rw::dat::LogMsg::Level;

      /**
       * @brief Log an error message only in debug builds.
       */
      template<rw::io::IsFormattable... Args>
      void err(std::string_view msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::error, msg, args...);
      }

      /**
       * @brief Log a fatal message only in debug builds.
       * @details Throws an std::exception.
       */
      template<rw::io::IsFormattable... Args>
      void fatal(std::string_view msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::fatal, msg, args...);
      }

      /**
       * @brief Log an information message only in debug builds.
       */
      template<rw::io::IsFormattable... Args>
      void info(std::string_view msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::info, msg, args...);
      }

      /**
       * @brief Get the logger's instance.
       * @details Creates a new logger on the first call.
       */
      [[nodiscard]] static Logger* instance();

      /**
       * @brief Get the current log level.
       * @return Current log level.
       */
      [[nodiscard]] Level level() const;

      /**
       * @brief Base message logging function, should not be used directly.
       * @tparam Args Optional argument type for the format string.
       * @param level Logging level.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments for the format string.
       */
      template<rw::io::IsFormattable... Args>
      void message_base(Level level, std::string_view msg, const Args&... args)
      {
         if (level >= level_)
         {
            rw::dat::LogMsg message{ level, std::vformat(msg, std::make_format_args(args...)) };

            std::scoped_lock lck{ mtx_ };

            switch (level)
            {
            case rw::utils::Logger::Level::trace:
               std::cout << "\033[0m" << std::format("{}", message) << "\033[0m\n";
               break;
            case rw::utils::Logger::Level::info:
               std::cout << "\033[37m" << std::format("{}", message) << "\033[0m\n";
               break;
            case rw::utils::Logger::Level::warning:
               std::cout << "\033[33m" << std::format("{}", message) << "\033[0m\n";
               break;
            case rw::utils::Logger::Level::error:
               std::cout << "\033[31m" << std::format("{}", message) << "\033[0m\n";
               break;
            case rw::utils::Logger::Level::fatal:
               std::cout << "\033[35m" << std::format("{}", message) << "\033[0m\n";
               throw std::exception("FATAL ERROR THROWN FROM LOGGER");
               break;
            }
         }
      }

      /**
       * @brief Log an error message in all build types.
       */
      template<rw::io::IsFormattable... Args>
      void relErr(std::string_view msg, const Args&... args)
      {
         message_base(Level::error, msg, args...);
      }

      /**
       * @brief Log a fatal message in all build types.
       * @details Throws an std::exception.
       */
      template<rw::io::IsFormattable... Args>
      void relFatal(std::string_view msg, const Args&... args)
      {
         message_base(Level::fatal, msg, args...);
      }

      /**
       * @brief Log an information message in all build types.
       */
      template<rw::io::IsFormattable... Args>
      void relInfo(std::string_view msg, const Args&... args)
      {
         message_base(Level::info, msg, args...);
      }

      /**
       * @brief Log a trace message in all build types.
       */
      template<rw::io::IsFormattable... Args>
      void relTrace(std::string_view msg, const Args&... args)
      {
         message_base(Level::trace, msg, args...);
      }

      /**
       * @brief Log a warning message in all build types.
       */
      template<rw::io::IsFormattable... Args>
      void relWarn(std::string_view msg, const Args&... args)
      {
         message_base(Level::warning, msg, args...);
      }

      /**
       * @brief Set the logging level for debug builds.
       * @param level New logging level for debug builds.
       */
      void setDebugLevel(Level level);

      /**
       * @brief Set the logging level for release builds.
       * @param level New logging level for release builds.
       */
      void setReleaseLevel(Level level);

      /**
       * @brief Log a trace message only in debug builds.
       */
      template<rw::io::IsFormattable... Args>
      void trace(std::string_view msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::trace, msg, args...);
      }

      /**
       * @brief Log a warning message only in debug builds.
       */
      template<rw::io::IsFormattable... Args>
      void warn(std::string_view msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::warning, msg, args...);
      }

   private:
      /**
       * @brief Default constructor.
       */
      Logger();

      static std::unique_ptr<Logger> instance_; /**< Current instance of the logger. */

      std::mutex         mtx_;                   /**< Logging mutex. */
      std::atomic<Level> level_{ Level::trace }; /**< Logging level. */
   };

   // Declaration of explicit template instantiation for common types, so they will not be compiled in user projects.
   extern template void Logger::trace(std::string_view);
   extern template void Logger::trace(std::string_view, const char* const&);
   extern template void Logger::trace(std::string_view, const std::string_view&);
   extern template void Logger::trace(std::string_view, const std::string&);
   extern template void Logger::trace(std::string_view, const char&);
   extern template void Logger::trace(std::string_view, const short&);
   extern template void Logger::trace(std::string_view, const int&);
   extern template void Logger::trace(std::string_view, const long&);
   extern template void Logger::trace(std::string_view, const long long&);
   extern template void Logger::trace(std::string_view, const unsigned char&);
   extern template void Logger::trace(std::string_view, const unsigned short&);
   extern template void Logger::trace(std::string_view, const unsigned int&);
   extern template void Logger::trace(std::string_view, const unsigned long&);
   extern template void Logger::trace(std::string_view, const unsigned long long&);
   extern template void Logger::trace(std::string_view, const float&);
   extern template void Logger::trace(std::string_view, const double&);
   extern template void Logger::trace(std::string_view, const bool&);

   extern template void Logger::info(std::string_view);
   extern template void Logger::info(std::string_view, const char* const&);
   extern template void Logger::info(std::string_view, const std::string_view&);
   extern template void Logger::info(std::string_view, const std::string&);
   extern template void Logger::info(std::string_view, const char&);
   extern template void Logger::info(std::string_view, const short&);
   extern template void Logger::info(std::string_view, const int&);
   extern template void Logger::info(std::string_view, const long&);
   extern template void Logger::info(std::string_view, const long long&);
   extern template void Logger::info(std::string_view, const unsigned char&);
   extern template void Logger::info(std::string_view, const unsigned short&);
   extern template void Logger::info(std::string_view, const unsigned int&);
   extern template void Logger::info(std::string_view, const unsigned long&);
   extern template void Logger::info(std::string_view, const unsigned long long&);
   extern template void Logger::info(std::string_view, const float&);
   extern template void Logger::info(std::string_view, const double&);
   extern template void Logger::info(std::string_view, const bool&);

   extern template void Logger::warn(std::string_view);
   extern template void Logger::warn(std::string_view, const char* const&);
   extern template void Logger::warn(std::string_view, const std::string_view&);
   extern template void Logger::warn(std::string_view, const std::string&);
   extern template void Logger::warn(std::string_view, const char&);
   extern template void Logger::warn(std::string_view, const short&);
   extern template void Logger::warn(std::string_view, const int&);
   extern template void Logger::warn(std::string_view, const long&);
   extern template void Logger::warn(std::string_view, const long long&);
   extern template void Logger::warn(std::string_view, const unsigned char&);
   extern template void Logger::warn(std::string_view, const unsigned short&);
   extern template void Logger::warn(std::string_view, const unsigned int&);
   extern template void Logger::warn(std::string_view, const unsigned long&);
   extern template void Logger::warn(std::string_view, const unsigned long long&);
   extern template void Logger::warn(std::string_view, const float&);
   extern template void Logger::warn(std::string_view, const double&);
   extern template void Logger::warn(std::string_view, const bool&);

   extern template void Logger::err(std::string_view);
   extern template void Logger::err(std::string_view, const char* const&);
   extern template void Logger::err(std::string_view, const std::string_view&);
   extern template void Logger::err(std::string_view, const std::string&);
   extern template void Logger::err(std::string_view, const char&);
   extern template void Logger::err(std::string_view, const short&);
   extern template void Logger::err(std::string_view, const int&);
   extern template void Logger::err(std::string_view, const long&);
   extern template void Logger::err(std::string_view, const long long&);
   extern template void Logger::err(std::string_view, const unsigned char&);
   extern template void Logger::err(std::string_view, const unsigned short&);
   extern template void Logger::err(std::string_view, const unsigned int&);
   extern template void Logger::err(std::string_view, const unsigned long&);
   extern template void Logger::err(std::string_view, const unsigned long long&);
   extern template void Logger::err(std::string_view, const float&);
   extern template void Logger::err(std::string_view, const double&);
   extern template void Logger::err(std::string_view, const bool&);

   extern template void Logger::fatal(std::string_view);
   extern template void Logger::fatal(std::string_view, const char* const&);
   extern template void Logger::fatal(std::string_view, const std::string_view&);
   extern template void Logger::fatal(std::string_view, const std::string&);
   extern template void Logger::fatal(std::string_view, const char&);
   extern template void Logger::fatal(std::string_view, const short&);
   extern template void Logger::fatal(std::string_view, const int&);
   extern template void Logger::fatal(std::string_view, const long&);
   extern template void Logger::fatal(std::string_view, const long long&);
   extern template void Logger::fatal(std::string_view, const unsigned char&);
   extern template void Logger::fatal(std::string_view, const unsigned short&);
   extern template void Logger::fatal(std::string_view, const unsigned int&);
   extern template void Logger::fatal(std::string_view, const unsigned long&);
   extern template void Logger::fatal(std::string_view, const unsigned long long&);
   extern template void Logger::fatal(std::string_view, const float&);
   extern template void Logger::fatal(std::string_view, const double&);
   extern template void Logger::fatal(std::string_view, const bool&);

   extern template void Logger::relTrace(std::string_view);
   extern template void Logger::relTrace(std::string_view, const char* const&);
   extern template void Logger::relTrace(std::string_view, const std::string_view&);
   extern template void Logger::relTrace(std::string_view, const std::string&);
   extern template void Logger::relTrace(std::string_view, const char&);
   extern template void Logger::relTrace(std::string_view, const short&);
   extern template void Logger::relTrace(std::string_view, const int&);
   extern template void Logger::relTrace(std::string_view, const long&);
   extern template void Logger::relTrace(std::string_view, const long long&);
   extern template void Logger::relTrace(std::string_view, const unsigned char&);
   extern template void Logger::relTrace(std::string_view, const unsigned short&);
   extern template void Logger::relTrace(std::string_view, const unsigned int&);
   extern template void Logger::relTrace(std::string_view, const unsigned long&);
   extern template void Logger::relTrace(std::string_view, const unsigned long long&);
   extern template void Logger::relTrace(std::string_view, const float&);
   extern template void Logger::relTrace(std::string_view, const double&);
   extern template void Logger::relTrace(std::string_view, const bool&);

   extern template void Logger::relInfo(std::string_view);
   extern template void Logger::relInfo(std::string_view, const char* const&);
   extern template void Logger::relInfo(std::string_view, const std::string_view&);
   extern template void Logger::relInfo(std::string_view, const std::string&);
   extern template void Logger::relInfo(std::string_view, const char&);
   extern template void Logger::relInfo(std::string_view, const short&);
   extern template void Logger::relInfo(std::string_view, const int&);
   extern template void Logger::relInfo(std::string_view, const long&);
   extern template void Logger::relInfo(std::string_view, const long long&);
   extern template void Logger::relInfo(std::string_view, const unsigned char&);
   extern template void Logger::relInfo(std::string_view, const unsigned short&);
   extern template void Logger::relInfo(std::string_view, const unsigned int&);
   extern template void Logger::relInfo(std::string_view, const unsigned long&);
   extern template void Logger::relInfo(std::string_view, const unsigned long long&);
   extern template void Logger::relInfo(std::string_view, const float&);
   extern template void Logger::relInfo(std::string_view, const double&);
   extern template void Logger::relInfo(std::string_view, const bool&);

   extern template void Logger::relWarn(std::string_view);
   extern template void Logger::relWarn(std::string_view, const char* const&);
   extern template void Logger::relWarn(std::string_view, const std::string_view&);
   extern template void Logger::relWarn(std::string_view, const std::string&);
   extern template void Logger::relWarn(std::string_view, const char&);
   extern template void Logger::relWarn(std::string_view, const short&);
   extern template void Logger::relWarn(std::string_view, const int&);
   extern template void Logger::relWarn(std::string_view, const long&);
   extern template void Logger::relWarn(std::string_view, const long long&);
   extern template void Logger::relWarn(std::string_view, const unsigned char&);
   extern template void Logger::relWarn(std::string_view, const unsigned short&);
   extern template void Logger::relWarn(std::string_view, const unsigned int&);
   extern template void Logger::relWarn(std::string_view, const unsigned long&);
   extern template void Logger::relWarn(std::string_view, const unsigned long long&);
   extern template void Logger::relWarn(std::string_view, const float&);
   extern template void Logger::relWarn(std::string_view, const double&);
   extern template void Logger::relWarn(std::string_view, const bool&);

   extern template void Logger::relErr(std::string_view);
   extern template void Logger::relErr(std::string_view, const char* const&);
   extern template void Logger::relErr(std::string_view, const std::string_view&);
   extern template void Logger::relErr(std::string_view, const std::string&);
   extern template void Logger::relErr(std::string_view, const char&);
   extern template void Logger::relErr(std::string_view, const short&);
   extern template void Logger::relErr(std::string_view, const int&);
   extern template void Logger::relErr(std::string_view, const long&);
   extern template void Logger::relErr(std::string_view, const long long&);
   extern template void Logger::relErr(std::string_view, const unsigned char&);
   extern template void Logger::relErr(std::string_view, const unsigned short&);
   extern template void Logger::relErr(std::string_view, const unsigned int&);
   extern template void Logger::relErr(std::string_view, const unsigned long&);
   extern template void Logger::relErr(std::string_view, const unsigned long long&);
   extern template void Logger::relErr(std::string_view, const float&);
   extern template void Logger::relErr(std::string_view, const double&);
   extern template void Logger::relErr(std::string_view, const bool&);

   extern template void Logger::relFatal(std::string_view);
   extern template void Logger::relFatal(std::string_view, const char* const&);
   extern template void Logger::relFatal(std::string_view, const std::string_view&);
   extern template void Logger::relFatal(std::string_view, const std::string&);
   extern template void Logger::relFatal(std::string_view, const char&);
   extern template void Logger::relFatal(std::string_view, const short&);
   extern template void Logger::relFatal(std::string_view, const int&);
   extern template void Logger::relFatal(std::string_view, const long&);
   extern template void Logger::relFatal(std::string_view, const long long&);
   extern template void Logger::relFatal(std::string_view, const unsigned char&);
   extern template void Logger::relFatal(std::string_view, const unsigned short&);
   extern template void Logger::relFatal(std::string_view, const unsigned int&);
   extern template void Logger::relFatal(std::string_view, const unsigned long&);
   extern template void Logger::relFatal(std::string_view, const unsigned long long&);
   extern template void Logger::relFatal(std::string_view, const float&);
   extern template void Logger::relFatal(std::string_view, const double&);
   extern template void Logger::relFatal(std::string_view, const bool&);
} // namespace rw::utils

#endif