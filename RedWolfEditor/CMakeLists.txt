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

add_executable("${PROJECT_NAME}"
    "${VENDOR_DIR}/src/glad.c"
    "src/editor.cpp"
)

find_library(REDWOLF_LIB "RedWolf" HINTS "${BIN_DIR}/RedWolf")
target_link_libraries("${PROJECT_NAME}" "${REDWOLF_LIB}" "${SDL_LIB}" "${SDL_MAIN_LIB}")

# Copy relevant dlls.
if (WIN32)
    # Bin dir
    file(TO_NATIVE_PATH "${BIN_DIR}" NATIVE_BIN_DIR)
    # RedWolf
    file(TO_NATIVE_PATH "${BIN_DIR}/RedWolf/" NATIVE_REDWOLF_DIR)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND copy /Y /B "${NATIVE_REDWOLF_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolfEditor"
    )
endif (WIN32)