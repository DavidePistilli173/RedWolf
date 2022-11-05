#ifndef RW_UTILS_LOG_HPP
#define RW_UTILS_LOG_HPP

#include "RedWolf/common.hpp"
#include "RedWolf/dat/LogMsg.hpp"
#include "RedWolf/io/concepts.hpp"
#include "RedWolf/io/custom_formatters.hpp"

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
   class RW_API Logger
   {
   public:
      using LogMsg = rw::dat::LogMsg;
      using Level = rw::dat::LogMsg::Level;

      /**
       * @brief Utility structure for storing a message and the source code location that generated.
       */
      struct MessageWithLocation
      {
         /**
          * @brief Constructor. It is not made explicit on purpose, so that it is transparent to the user.
          * @param pMsg Text message with ftm formatting syntax.
          * @param pLoc Source code location data.
          */
         template<typename T>
         MessageWithLocation(T pMsg, std::source_location pLoc = std::source_location::current()) : txt{ pMsg }, loc{ pLoc }
         {
         }

         std::string_view     txt; /**< Text message with formatting syntax. */
         std::source_location loc; /**< Source code location data. */
      };

      /**
       * @brief Default constructor.
       */
      Logger();

      /**
       * @brief Log an error message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void err(const MessageWithLocation& msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::error, msg, args...);
      }

      /**
       * @brief Log a fatal message only in debug builds.
       * @details Throws an std::exception.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void fatal(const MessageWithLocation& msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::fatal, msg, args...);
      }

      /**
       * @brief Log an information message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void info(const MessageWithLocation& msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::info, msg, args...);
      }

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
      void message_base(Level level, const MessageWithLocation& msg, const Args&... args)
      {
         if (level >= level_)
         {
            rw::dat::LogMsg message{ level, msg.loc, std::vformat(msg.txt, std::make_format_args(args...)) };

            std::scoped_lock lck{ mtx_ };

            switch (level)
            {
            case rw::utils::Logger::Level::trace:
               std::cout << "\033[0m" << std::format("{}", message) << "\033[0m\n";
               break;
            case rw::utils::Logger::Level::info:
               std::cout << "\033[32m" << std::format("{}", message) << "\033[0m\n";
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
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relErr(const MessageWithLocation& msg, const Args&... args)
      {
         message_base(Level::error, msg, args...);
      }

      /**
       * @brief Log a fatal message in all build types.
       * @details Throws an std::exception.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relFatal(const MessageWithLocation& msg, const Args&... args)
      {
         message_base(Level::fatal, msg, args...);
      }

      /**
       * @brief Log an information message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relInfo(const MessageWithLocation& msg, const Args&... args)
      {
         message_base(Level::info, msg, args...);
      }

      /**
       * @brief Log a trace message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relTrace(const MessageWithLocation& msg, const Args&... args)
      {
         message_base(Level::trace, msg, args...);
      }

      /**
       * @brief Log a warning message in all build types.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void relWarn(const MessageWithLocation& msg, const Args&... args)
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
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void trace(const MessageWithLocation& msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::trace, msg, args...);
      }

      /**
       * @brief Log a warning message only in debug builds.
       * @param msg Message to log. Supports the std::format syntax.
       * @param args Optional arguments to insert into the message.
       */
      template<rw::io::IsFormattable... Args>
      void warn(const MessageWithLocation& msg, const Args&... args)
      {
         if constexpr (rw::debug) message_base(Level::warning, msg, args...);
      }

   private:
      std::mutex         mtx_;                   /**< Logging mutex. */
      std::atomic<Level> level_{ Level::trace }; /**< Logging level. */
   };

   // Declaration of explicit template instantiation for common types, so they will not be compiled in user projects.
   extern template RW_API void Logger::trace(const MessageWithLocation&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::trace(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::info(const MessageWithLocation&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::info(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::warn(const MessageWithLocation&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::warn(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::err(const MessageWithLocation&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::err(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::fatal(const MessageWithLocation&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::fatal(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::relTrace(const MessageWithLocation&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::relTrace(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::relInfo(const MessageWithLocation&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::relInfo(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::relWarn(const MessageWithLocation&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::relWarn(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::relErr(const MessageWithLocation&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::relErr(const MessageWithLocation&, const bool&);

   extern template RW_API void Logger::relFatal(const MessageWithLocation&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const char* const&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const std::string_view&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const std::string&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const char&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const short&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const int&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const long&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const long long&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const unsigned char&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const unsigned short&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const unsigned int&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const unsigned long&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const unsigned long long&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const float&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const double&);
   extern template RW_API void Logger::relFatal(const MessageWithLocation&, const bool&);
} // namespace rw::utils

#endif