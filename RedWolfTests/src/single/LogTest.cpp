#include <RedWolf/Log.hpp>

enum class Error
{
    init = 1
};

int main()
{
    if (!rw::Log::init()) return static_cast<int>(Error::init);

    rw::Log::core_info("Core information.");
    rw::Log::core_warn("Core warning.");
    rw::Log::core_err("Core error.");
    rw::Log::app_info("App information");
    rw::Log::app_warn("App warning.");
    rw::Log::app_err("App error.");

    int param1{ -5 };
    rw::Log::core_info("Core information with param1={}.", param1);
    rw::Log::core_warn("Core warning with param1={}.", param1);
    rw::Log::core_err("Core error with param1={}.", param1);
    rw::Log::app_info("App information with param1={}.", param1);
    rw::Log::app_warn("App warning with param1={}.", param1);
    rw::Log::app_err("App error with param1={}.", param1);

    double param2{ 3.14 };
    rw::Log::core_info("Core information with:\n    - param1={0}\n    - param2={1}", param1, param2);
    rw::Log::core_warn("Core warning with:\n    - param1={0}\n    - param2={1}", param1, param2);
    rw::Log::core_err("Core error with:\n    - param1={0}\n    - param2={1}", param1, param2);
    rw::Log::app_info("App information with:\n    - param1={0}\n    - param2={1}", param1, param2);
    rw::Log::app_warn("App warning with:\n    - param1={0}\n    - param2={1}", param1, param2);
    rw::Log::app_err("App error with:\n    - param1={0}\n    - param2={1}", param1, param2);

    return 0;
}