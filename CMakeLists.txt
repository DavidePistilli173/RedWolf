cmake_minimum_required(VERSION "3.8")

project("RedWolfEngine" VERSION "0.0.3" DESCRIPTION "")

set(REDWOLF_SOURCE_DIR "${PROJECT_SOURCE_DIR}/RedWolf")
set(REDWOLFEDITOR_SOURCE_DIR "${PROJECT_SOURCE_DIR}/RedWolfSandbox")
set(REDWOLFTESTS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/RedWolfTests")
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(OBJ_DIR "${PROJECT_SOURCE_DIR}/obj")
set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")

find_library(SDL_LIB "SDL2" HINTS "${VENDOR_DIR}/lib/SDL2.0.9")
find_library(SDL_MAIN_LIB "SDL2main" HINTS "${VENDOR_DIR}/lib/SDL2.0.9")
find_library(SDL_IMAGE_LIB "SDL2_image" HINTS "${VENDOR_DIR}/lib/SDL2_image2.0.4")
find_library(SDL_TTF_LIB "SDL2_ttf" HINTS "${VENDOR_DIR}/lib/SDL2_ttf2.0.15")

#Set-Up GTest
# Download and unpack googletest at configure time
configure_file("CMakeLists.txt.in" "googletest-download/CMakeLists.txt")

execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
                 EXCLUDE_FROM_ALL
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

enable_testing()

add_compile_definitions(RW_DEBUG) # Macro for debug configurations.

add_subdirectory("${REDWOLF_SOURCE_DIR}")
add_subdirectory("${REDWOLFEDITOR_SOURCE_DIR}")
add_subdirectory("${REDWOLFTESTS_SOURCE_DIR}")