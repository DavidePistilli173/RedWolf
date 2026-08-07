if(RW_ENABLE_PROFILING)
    set(TRACY_ENABLE ON)
    set(TRACY_STATIC ON)
    add_subdirectory(tracy)

    if(RW_BUILD_PROFILER)
        add_subdirectory(tracy/profiler)
    endif()
endif()
