cmake_minimum_required(VERSION "3.8") # Minimum cmake version to support C++17
set(CMAKE_CXX_STANDARD 17) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolf" VERSION "0.0.3" DESCRIPTION "Library for 2D applications.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}/${PROJECT_NAME}")

add_compile_definitions(RW_EXPORTS) # Macro for dll exporting.

# Build the library.
include_directories(
    "include"
    "${VENDOR_DIR}/include"
)

add_library("${PROJECT_NAME}" SHARED
    "${VENDOR_DIR}/src/glad.c"
    "src/gl/error.cpp"
    "src/gl/Texture.cpp"
    "src/Log.cpp"
    "src/init.cpp"
    "src/ui/Window.cpp"  
    "src/gl/Buffer.cpp" 
    "src/io/Text.cpp"
    "src/io/Parser.cpp" 
    "src/algorithms.cpp")

target_link_libraries("${PROJECT_NAME}"
    "${SDL_LIB}"
    "${SDL_IMAGE_LIB}"
    "${SDL_TTF_LIB}"
)

# Copy relevant dlls to binary folder.
if (WIN32)
    # Bin dir
    file(TO_NATIVE_PATH "${BIN_DIR}" NATIVE_BIN_DIR)
    # SDL
    file(TO_NATIVE_PATH "${VENDOR_DIR}/lib/SDL2.0.9" NATIVE_SDL_DIR)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND copy /Y /B "${NATIVE_SDL_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolf"
    )
    # SDL image
    file(TO_NATIVE_PATH "${VENDOR_DIR}/lib/SDL2_image2.0.4" NATIVE_SDL_IMAGE_DIR)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND copy /Y /B "${NATIVE_SDL_IMAGE_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolf"
    )
    # SDL TTF
    file(TO_NATIVE_PATH "${VENDOR_DIR}/lib/SDL2_ttf2.0.15" NATIVE_SDL_TTF_DIR)
    add_custom_command(
        TARGET "${PROJECT_NAME}" POST_BUILD
        COMMAND copy /Y /B "${NATIVE_SDL_TTF_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolf"
    )
endif (WIN32)