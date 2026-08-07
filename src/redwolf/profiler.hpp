#pragma once

#ifdef RW_ENABLE_PROFILING
    #include <tracy/Tracy.hpp>

    #define RW_PROFILE_MARK_FRAME FrameMark;
    #define RW_PROFILE_SCOPE      ZoneScoped

#else
    #define RW_PROFILE_MARK_FRAME
    #define RW_PROFILE_SCOPE

#endif