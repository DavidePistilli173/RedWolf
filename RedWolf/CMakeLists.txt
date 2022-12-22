cmake_minimum_required(VERSION "3.12") # Minimum cmake version to support C++20
set(CMAKE_CXX_STANDARD 20) # Target C++ standard.
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enable the target standard.

project("RedWolf" VERSION "0.5.0" DESCRIPTION "Graphics and utility library for desktop applications.")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RW_BIN_DIR}/${PROJECT_NAME}")
set(CMAKE_INSTALL_PREFIX "${RW_BIN_DIR}/${PROJECT_NAME}/${CMAKE_BUILD_TYPE}")

configure_file("${PROJECT_SOURCE_DIR}/common.hpp.in" "${PROJECT_SOURCE_DIR}/common.hpp")

# Third-party libraries
set(RW_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")

# Vulkan
find_package(Vulkan REQUIRED FATAL_ERROR)

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

# Compile definitions
if (WIN32)
    add_compile_definitions(RW_DLL) # Macro for dll exporting.
endif (WIN32)

# Build the library.
include_directories(
    "${RW_ROOT_DIR}"
    "${Vulkan_INCLUDE_DIRS}"
    "${RW_GLFW_INCLUDE_DIR}"
    "${RW_GLM_INCLUDE_DIR}"
)

add_library("${RW_LIB_TARGET_NAME}" SHARED
    "RedWolfManager.cpp"
    "core/BaseApplication.cpp"
    "core/BaseGUIApplication.cpp"
    "core/BaseObject.cpp"
    "dat/LogMsg.cpp"
    "dat/SettingsNode.cpp"
    "events/EventHandler.cpp"
    "gfx/Renderer.cpp"
    "io/File.cpp"
    "lif/glfw/GlfwManager.cpp"
    "lif/glfw/Window.cpp"
    "lif/vulkan/BaseObject.cpp"
    "lif/vulkan/CommandBuffer.cpp"
    "lif/vulkan/CommandPool.cpp"
    "lif/vulkan/DebugUtilsMessenger.cpp"
    "lif/vulkan/DeviceBase.cpp"
    "lif/vulkan/FrameBuffer.cpp"
    "lif/vulkan/GraphicsDevice.cpp"
    "lif/vulkan/GraphicsPipeline.cpp"
    "lif/vulkan/ImageView.cpp"
    "lif/vulkan/Instance.cpp"
    "lif/vulkan/InstanceBase.cpp"
    "lif/vulkan/Interface.cpp"
    "lif/vulkan/PhysicalDevice.cpp"
    "lif/vulkan/PipelineLayout.cpp"
    "lif/vulkan/RenderPass.cpp"
    "lif/vulkan/ShaderModule.cpp"
    "lif/vulkan/Surface.cpp"
    "lif/vulkan/SwapChain.cpp"
    "net/BaseSocket.cpp"
    "net/UdpSocket.cpp"
    "text/string_utils.cpp"
    "thread/ThreadPool.cpp"
    "time/DateTime.cpp"
    "time/Timer.cpp"
    "ui/BaseWindow.cpp"
    "utils/Logger.cpp"
    "utils/SettingsManager.cpp"
 )
set_target_properties(${RW_LIB_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${RW_LIB_TARGET_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

if (WIN32)
    target_link_libraries("${RW_LIB_TARGET_NAME}" 
        "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/glfw3.lib"
        "Ws2_32.lib"
        "${Vulkan_LIBRARIES}"
    )
else (WIN32)
    target_link_libraries("${RW_LIB_TARGET_NAME}" 
        "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/glfw3.lib"
        "${Vulkan_LIBRARIES}"
    )
endif (WIN32)

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