cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project("redwolf" VERSION 0.8.0 LANGUAGES C CXX)

option(RW_BUILD_PROFILER "Build the profiler application" OFF)
option(RW_BUILD_TESTS "Build tests" ON)
option(RW_ENABLE_LOGGING "Enable logging" ON)
option(RW_ENABLE_PROFILING "Enable profiling" OFF)
option(RW_ENABLE_VULKAN_DEBUG "Enable Vulkan validation layers." ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(RW_ENABLE_LOGGING)
    add_compile_definitions(RW_ENABLE_LOGGING)
endif()

if(RW_ENABLE_PROFILING)
    add_compile_definitions(RW_ENABLE_PROFILING)
endif()

if (RW_ENABLE_VULKAN_DEBUG)
    add_compile_definitions(RW_ENABLE_VULKAN_DEBUG)
endif()

add_subdirectory(src)

if(RW_BUILD_TESTS)
    add_subdirectory(test)
endif()
