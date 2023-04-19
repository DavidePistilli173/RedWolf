cmake_minimum_required(VERSION "3.12") # Minimum cmake version to support C++20
set(CMAKE_CXX_STANDARD 20) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolfTest" VERSION "0.5.0" DESCRIPTION "Tests for the RedWolf library.")

set(CMAKE_INSTALL_PREFIX "${RW_BIN_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

# Third-party libraries
set(RW_TEST_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")

# GTEST
set(GTEST_DIR "${RW_TEST_THIRD_PARTY_DIR}/googletest" CACHE PATH "gtest directory")
include(GoogleTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${RW_TEST_THIRD_PARTY_DIR}/googletest")

# Vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)

message(${RW_ROOT_DIR})
include_directories(
    ${RW_ROOT_DIR}
    "${RW_LIB_DIR}/include"
    "${Vulkan_INCLUDE_DIRS}"
)

add_executable("${PROJECT_NAME}"
    "core/test_Object.cpp"
    "dat/test_LogMsg.cpp"
    "evt/test_Event.cpp"
    "thread/test_ThreadPool.cpp"
    "time/test_DateTime.cpp"
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries("${PROJECT_NAME}" 
                      "${RW_LIB_DIR}/${RW_LIB_TARGET_NAME}.lib"
                      GTest::gtest_main
)
gtest_add_tests(TARGET "${PROJECT_NAME}" )