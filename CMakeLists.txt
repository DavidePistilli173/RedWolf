cmake_minimum_required(VERSION "3.12")

project("RedWolfEngine" VERSION "0.5.0" DESCRIPTION "")

# Library variables
set(RW_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(RW_SRC_DIR "${PROJECT_SOURCE_DIR}/RedWolf")
set(RW_SANDBOX_DIR "${PROJECT_SOURCE_DIR}/RedWolfSandbox")
set(RW_TEST_DIR "${PROJECT_SOURCE_DIR}/RedWolfTest")
set(RW_BUILD_DIR "${PROJECT_SOURCE_DIR}/build")
set(RW_BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(RW_LIB_DIR "${RW_BIN_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${RW_BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${RW_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RW_BIN_DIR}")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(RW_LIB_TARGET_NAME "RedWolfd")
else()
    set(RW_LIB_TARGET_NAME "RedWolf")
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "/W4 /wd4251")
endif(MSVC)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(RW_DEBUG) # Macro for debug builds.
endif()

add_subdirectory("${RW_SRC_DIR}")
add_subdirectory("${RW_SANDBOX_DIR}")

enable_testing()
add_subdirectory("${RW_TEST_DIR}")