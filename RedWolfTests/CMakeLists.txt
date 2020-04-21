cmake_minimum_required(VERSION "3.8") # Minimum cmake version to support C++17
set(CMAKE_CXX_STANDARD 17) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolfTests" VERSION "0.0.2" DESCRIPTION "Unit tests for RedWolf.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")

include_directories(
    "include"
    "${REDWOLF_SOURCE_DIR}/include"
    "${VENDOR_DIR}/include"
)

find_library(REDWOLF_LIB "RedWolf" HINTS "${BIN_DIR}/RedWolf")

add_executable("${PROJECT_NAME}"
    "${VENDOR_DIR}/src/glad.c"
    "src/google_tests.cpp"
)
target_link_libraries("${PROJECT_NAME}" 
    "${REDWOLF_LIB}" "${SDL_LIB}" "${SDL_MAIN_LIB}" "${SDL_IMAGE_LIB}" "${SDL_TTF_LIB}" gtest
)
add_test(NAME "${PROJECT_NAME}" COMMAND "${PROJECT_NAME}")

# Copy relevant dlls.
if (WIN32)
    # Bin dir
    file(TO_NATIVE_PATH "${BIN_DIR}" NATIVE_BIN_DIR)
    # RedWolf
    file(TO_NATIVE_PATH "${BIN_DIR}/RedWolf/" NATIVE_REDWOLF_DIR)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND copy /Y /B "${NATIVE_REDWOLF_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolfTests"
    )
endif (WIN32)

# Copy test data.
if (WIN32)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/data" NATIVE_DATA_DIR)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND xcopy "${NATIVE_DATA_DIR}" "${NATIVE_BIN_DIR}\\RedWolfTests\\data" /I /C /K /E /R /Y
    )
endif (WIN32)