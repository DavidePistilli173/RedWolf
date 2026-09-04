add_library(redwolf STATIC)

find_package(Vulkan REQUIRED COMPONENTS glslc)

target_sources(redwolf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/entry_point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events/events.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/random.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_command_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_fence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_framebuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_renderpass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_semaphore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backend/vulkan_swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/profiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/user_data.cpp)

target_include_directories(redwolf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Custom linux preparation.
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_libraries(redwolf PUBLIC vulkan xkbcommon wayland-client)

    function(add_wayland_protocol_target TARGET_NAME SPEC NAME)
        set(PROTO_XML /usr/share/wayland-protocols/${SPEC})
        set(HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/platform/${NAME}.h)
        set(SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/platform/${NAME}.c)

        add_custom_command(
            OUTPUT ${HEADER_FILE}
            COMMAND wayland-scanner client-header ${PROTO_XML} ${HEADER_FILE}
            DEPENDS ${PROTO_XML}
            COMMENT "Generating ${NAME}.h"
            VERBATIM
        )

        add_custom_command(
            OUTPUT ${SOURCE_FILE}
            COMMAND wayland-scanner private-code ${PROTO_XML} ${SOURCE_FILE}
            DEPENDS ${PROTO_XML}
            COMMENT "Generating ${NAME}.c"
            VERBATIM
        )

        target_sources(redwolf PRIVATE
            ${SOURCE_FILE}
            ${HEADER_FILE}
        )
    endfunction()

    add_wayland_protocol_target(xdg_shell "stable/xdg-shell/xdg-shell.xml" xdg-shell)

    target_sources(redwolf PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/platform_linux.cpp
    )
endif()

# Shader compilation
make_directory(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/shaders/)

set_property(GLOBAL PROPERTY RW_SHADER_OUTPUTS "")
function(compile_shader SHADER_NAME SHADER_STAGE)
    set(INPUT_FILE ${PROJECT_SOURCE_DIR}/data/shaders/${SHADER_NAME}.${SHADER_STAGE}.glsl)
    set(OUTPUT_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/shaders/${SHADER_NAME}.spv)

    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND Vulkan::glslc -fshader-stage=${SHADER_STAGE} ${INPUT_FILE} -o ${OUTPUT_FILE}
        DEPENDS ${INPUT_FILE}
        COMMENT "Compiling ${INPUT_FILE}"
        VERBATIM
    )
    set_property(GLOBAL APPEND PROPERTY RW_SHADER_OUTPUTS ${OUTPUT_FILE})
endfunction()
#compile_shader("builtin.object_shader" "vert")

get_property(RW_ALL_SHADER_OUTPUTS GLOBAL PROPERTY RW_SHADER_OUTPUTS)
add_custom_target(redwolf_shaders ALL DEPENDS ${RW_ALL_SHADER_OUTPUTS})
add_dependencies(redwolf redwolf_shaders)

# Profiling
if(RW_ENABLE_PROFILING)
    target_link_libraries(redwolf PUBLIC TracyClient)
endif()
