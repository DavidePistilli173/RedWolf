#include "RedWolf/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace rw
{
    std::shared_ptr<spdlog::logger> Log::coreLogger_;
    std::shared_ptr<spdlog::logger> Log::appLogger_;

    bool Log::init()
    {
        spdlog::set_pattern(pattern.data());

        coreLogger_ = spdlog::stdout_color_mt(core_name.data());
        coreLogger_->set_level(spdlog::level::trace);
        appLogger_ = spdlog::stdout_color_mt(app_name.data());
        appLogger_->set_level(spdlog::level::trace);

        return true;
    }
}