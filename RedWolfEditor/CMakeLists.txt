cmake_minimum_required(VERSION "3.8") # Minimum cmake version to support C++17
set(CMAKE_CXX_STANDARD 17) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolfEditor" VERSION "0.0.1" DESCRIPTION "Editor for RedWolf applications.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")

include_directories(
    "include"
    "${REDWOLF_SOURCE_DIR}/include"
    "${VENDOR_DIR}/include"
)

find_library(SDL_LIB "SDL2" HINTS "${VENDOR_DIR}/lib/SDL2.0.9")
find_library(SDL_MAIN_LIB "SDL2main" HINTS "${VENDOR_DIR}/lib/SDL2.0.9")

add_executable("${PROJECT_NAME}"
    "src/editor.cpp"
)

find_library(REDWOLF_LIB "RedWolf" HINTS "${BIN_DIR}/RedWolf")
target_link_libraries("${PROJECT_NAME}" "${REDWOLF_LIB}" "${SDL_LIB}" "${SDL_MAIN_LIB}")