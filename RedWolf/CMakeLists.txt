cmake_minimum_required(VERSION "3.12") # Minimum cmake version to support C++20
set(CMAKE_CXX_STANDARD 20) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolf" VERSION "0.5.0" DESCRIPTION "Graphics and utility library for desktop applications.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_INSTALL_PREFIX "${RW_BIN_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}")

# Third-party libraries
set(RW_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")

# GLFW
set(RW_GLFW_DIR "${RW_THIRD_PARTY_DIR}/glfw")
set(RW_GLFW_INCLUDE_DIR "${RW_THIRD_PARTY_DIR}/glfw/include")
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory("${RW_GLFW_DIR}")

# GLM
set(RW_GLM_INCLUDE_DIR "${RW_THIRD_PARTY_DIR}/glm")

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

# Build the library.
include_directories(
    "${RW_ROOT_DIR}"
    "${RW_GLFW_INCLUDE_DIR}"
    "${RW_GLM_INCLUDE_DIR}"
)

add_library("${RW_LIB_TARGET_NAME}" STATIC
    "core/BaseApplication.cpp"
    "core/BaseObject.cpp"
    "dat/LogMsg.cpp"
    "io/File.cpp"
    "thread/ThreadPool.cpp"
    "time/DateTime.cpp"
    "time/Timer.cpp"
    "utils/Logger.cpp"
    "utils/Settings.cpp"
)
set_target_properties(${RW_LIB_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${RW_LIB_TARGET_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

#target_link_libraries("${PROJECT_NAME}"
#    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/glfw3.lib"
#)

# Copy all files required by the users in the output folder.
if (WIN32)
    file(TO_NATIVE_PATH "${RW_BIN_DIR}/${PROJECT_NAME}" NATIVE_OUTPUT_DIR)

    # RedWolf
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}" NATIVE_SOURCE_DIR)
    add_custom_command(
        TARGET ${RW_LIB_TARGET_NAME} POST_BUILD
        COMMAND xcopy "${NATIVE_SOURCE_DIR}\\*.hpp" "${NATIVE_OUTPUT_DIR}\\include\\RedWolf" /f /i /s /y 
    )
    
    # GLFW
    file(TO_NATIVE_PATH "${RW_GLFW_INCLUDE_DIR}" NATIVE_GLFW_INCLUDE_DIR)
    add_custom_command(
        TARGET ${RW_LIB_TARGET_NAME} POST_BUILD
        COMMAND xcopy "${NATIVE_GLFW_INCLUDE_DIR}\\GLFW" "${NATIVE_OUTPUT_DIR}\\include\\GLFW" /f /i /s /y  
    )

    # GLM
    file(TO_NATIVE_PATH "${RW_GLM_INCLUDE_DIR}" NATIVE_GLM_INCLUDE_DIR)
    add_custom_command(
        TARGET ${RW_LIB_TARGET_NAME} POST_BUILD
        COMMAND xcopy "${NATIVE_GLM_INCLUDE_DIR}\\glm" "${NATIVE_OUTPUT_DIR}\\include\\glm" /f /i /s /y 
    )
endif (WIN32)

# Copy relevant dlls to binary folder.
#if (WIN32)
    # Bin dir
#    file(TO_NATIVE_PATH "${BIN_DIR}" NATIVE_BIN_DIR)
    # SDL
#    file(TO_NATIVE_PATH "${VENDOR_DIR}/lib/SDL2.0.9" NATIVE_SDL_DIR)
#    add_custom_command(
#        TARGET "${PROJECT_NAME}" POST_BUILD
#        COMMAND copy /Y /B "${NATIVE_SDL_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolf"
#    )
    # SDL image
#    file(TO_NATIVE_PATH "${VENDOR_DIR}/lib/SDL2_image2.0.4" NATIVE_SDL_IMAGE_DIR)
#    add_custom_command(
#        TARGET "${PROJECT_NAME}" POST_BUILD
#        COMMAND copy /Y /B "${NATIVE_SDL_IMAGE_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolf"
#    )
    # SDL TTF
#    file(TO_NATIVE_PATH "${VENDOR_DIR}/lib/SDL2_ttf2.0.15" NATIVE_SDL_TTF_DIR)
#    add_custom_command(
#        TARGET "${PROJECT_NAME}" POST_BUILD
#        COMMAND copy /Y /B "${NATIVE_SDL_TTF_DIR}\\*.dll" "${NATIVE_BIN_DIR}\\RedWolf"
#    )
#endif (WIN32)