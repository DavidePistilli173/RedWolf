cmake_minimum_required(VERSION "3.12") # Minimum cmake version to support C++20
set(CMAKE_CXX_STANDARD 20) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolfSandbox" VERSION "0.5.0" DESCRIPTION "Sandbox application that uses the RedWolf library.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_INSTALL_PREFIX "${BIN_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

include_directories(
    ${RW_ROOT_DIR}
)

add_executable("${PROJECT_NAME}"
    "main.cpp"
    "MainApplication.cpp"
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
target_link_libraries("${PROJECT_NAME}" "${RW_LIB_DIR}/${RW_LIB_TARGET_NAME}.lib")
