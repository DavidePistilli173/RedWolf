cmake_minimum_required(VERSION "3.8") # Minimum cmake version to support C++17
set(CMAKE_CXX_STANDARD 17) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolf" VERSION "0.0.1" DESCRIPTION "Engine for 2D applications.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")

add_compile_definitions(RW_EXPORTS) # Macro for dll exporting.
add_compile_definitions(RW_DEBUG) # Macro for debug configurations.

# Build the library.
include_directories(
    "include"
    "${VENDOR_DIR}/include"
)

add_library("${PROJECT_NAME}" SHARED
    "${VENDOR_DIR}/src/glad.c"
    "src/App.cpp"
    "src/Log.cpp"
)