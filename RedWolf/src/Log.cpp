#include "RedWolf/Log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace rw
{
   std::shared_ptr<spdlog::logger> Log::coreLogger_;
   std::shared_ptr<spdlog::logger> Log::appLogger_;
   std::shared_ptr<spdlog::logger> Log::glLogger_;

   /**
       \param mode Operating mode of the logger.
   */
   bool Log::init(Mode mode)
   {
      /* Create the file sink and set its pattern. */
      std::shared_ptr fileSink{ std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_name.data()) };
      if (fileSink == nullptr) return false;
      fileSink->set_pattern(pattern.data());

      /* Create all loggers with the file sink. */
      coreLogger_ = std::make_shared<spdlog::logger>(core_name.data(), fileSink);
      if (coreLogger_ == nullptr) return false;
      coreLogger_->set_level(spdlog::level::trace);

      appLogger_ = std::make_shared<spdlog::logger>(app_name.data(), fileSink);
      if (appLogger_ == nullptr) return false;
      appLogger_->set_level(spdlog::level::trace);

      glLogger_ = std::make_shared<spdlog::logger>(gl_name.data(), fileSink);
      if (glLogger_ == nullptr) return false;
      glLogger_->set_level(spdlog::level::trace);

      /* If console output is required, add the appropriate sink to the loggers. */
      if (mode == Mode::file_and_console)
      {
         /* Create the stdout sink and set its pattern. */
         auto stdoutSink{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>() };
         if (stdoutSink == nullptr) return false;
         stdoutSink->set_pattern(pattern.data());
         /* Add the sink to all loggers. */
         coreLogger_->sinks().push_back(stdoutSink);
         appLogger_->sinks().push_back(stdoutSink);
         glLogger_->sinks().push_back(stdoutSink);
      }

      return true;
   }
} // namespace rw