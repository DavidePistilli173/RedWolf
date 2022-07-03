cmake_minimum_required(VERSION "3.12")

project("RedWolfEngine" VERSION "0.5.0" DESCRIPTION "")

set(RW_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(RW_SRC_DIR "${PROJECT_SOURCE_DIR}/RedWolf")
set(RW_SANDBOX_DIR "${PROJECT_SOURCE_DIR}/RedWolfSandbox")
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(BUILD_DIR "${PROJECT_SOURCE_DIR}/build")
set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(RW_LIB_DIR "${BIN_DIR}/RedWolf/")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(RW_LIB_TARGET_NAME "RedWolfd")
else()
    set(RW_LIB_TARGET_NAME "RedWolf")
endif()

#find_library(SDL_LIB "SDL2" HINTS "${VENDOR_DIR}/lib/SDL2.0.9")
#find_library(SDL_MAIN_LIB "SDL2main" HINTS "${VENDOR_DIR}/lib/SDL2.0.9")
#find_library(SDL_IMAGE_LIB "SDL2_image" HINTS "${VENDOR_DIR}/lib/SDL2_image2.0.4")
#find_library(SDL_TTF_LIB "SDL2_ttf" HINTS "${VENDOR_DIR}/lib/SDL2_ttf2.0.15")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_compile_definitions(RW_DEBUG) # Macro for dll exporting.
endif()

add_subdirectory("${RW_SRC_DIR}")
add_subdirectory("${RW_SANDBOX_DIR}")