#ifndef RW_LOG_HPP
#define RW_LOG_HPP

#include "RedWolf/core.hpp"

#include <memory>
#include <spdlog/spdlog.h>
#include <string_view>

namespace rw
{
   /** \brief Class for message logging. */
   class RW_API Log
   {
   public:
      /** \brief Logger modes. */
      enum class Mode
      {
         file_only,       /**< Only log to file. */
         file_and_console /**< Log both to file and console. */
      };

      /** \brief Name of the log file. */
      static constexpr std::string_view file_name{ "rwelog.log" };
      /** \brief Name of the library logger. */
      static constexpr std::string_view core_name{ "REDWOLF" };
      /** \brief Name of the application logger. */
      static constexpr std::string_view app_name{ "APP" };
      /** \brief Name of the OpenGL logger. */
      static constexpr std::string_view gl_name{ "OPENGL" };
      /** \brief Formatting pattern for log messages. */
      static constexpr std::string_view pattern{ "%^[%n - %l] - %T:%e\n    %v" };

      /** 
            \brief Initialise the logger. 
            \return true if initialisation was successful, false otherwise.
        */
      static bool init(Mode mode);

      /** 
            \brief Log information from the library.
            Only active in debug builds (see core.hpp).
            \param msg Information message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
      template<typename... Args>
      static void core_info(std::string_view msg, Args... args)
      {
         if constexpr (rw::debug)
         {
            coreLogger_->info(msg, args...);
         }
      }

      /**
            \brief Log a warning from the library.
            Only active in debug builds (see core.hpp)
            \param msg Warning message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
      template<typename... Args>
      static void core_warn(std::string_view msg, Args... args)
      {
         if constexpr (rw::debug)
         {
            coreLogger_->warn(msg, args...);
         }
      }

      /**
            \brief Log an error from the library.
            Only active in debug builds (see core.hpp)
            \param msg Error message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
      template<typename... Args>
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
      template<typename... Args>
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
      template<typename... Args>
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
      template<typename... Args>
      static void app_err(std::string_view msg, Args... args)
      {
         if constexpr (rw::debug)
         {
            appLogger_->error(msg, args...);
         }
      }

      /**
            \brief Log information from the OpenGL API.
            Only active in debug builds (see core.hpp)
            \param msg Information message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
      template<typename... Args>
      static void gl_info(std::string_view msg, Args... args)
      {
         if constexpr (rw::debug)
         {
            glLogger_->info(msg, args...);
         }
      }

      /**
            \brief Log a warning from the OpenGL API.
            Only active in debug builds (see core.hpp)
            \param msg Warning message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
      template<typename... Args>
      static void gl_warn(std::string_view msg, Args... args)
      {
         if constexpr (rw::debug)
         {
            glLogger_->warn(msg, args...);
         }
      }

      /**
            \brief Log an error from the OpenGL API.
            Only active in debug builds (see core.hpp)
            \param msg Error message with spdlog formatting.
            \param args Additional parameters to be printed with msg.
        */
      template<typename... Args>
      static void gl_err(std::string_view msg, Args... args)
      {
         if constexpr (rw::debug)
         {
            glLogger_->error(msg, args...);
         }
      }

   private:
      /** \brief Engine logger. */
      static std::shared_ptr<spdlog::logger> coreLogger_;
      /** \brief Application logger. */
      static std::shared_ptr<spdlog::logger> appLogger_;
      /** \brief OpenGL logger. */
      static std::shared_ptr<spdlog::logger> glLogger_;
   };
} // namespace rw

#endif