cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project("redwolf" VERSION 0.8.0 LANGUAGES C CXX)

option(RW_BUILD_TESTS "Build tests" ON)
option(RW_ENABLE_LOGS "Enable logging" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(RW_ENABLE_LOGS)
    add_compile_definitions(RW_ENABLE_LOGS)
endif()

add_subdirectory(src)

if(RW_BUILD_TESTS)
    add_subdirectory(test)
endif()
