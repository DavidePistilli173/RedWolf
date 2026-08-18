add_library(redwolf STATIC)

target_sources(redwolf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/entry_point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events/events.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input/input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_pool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/profiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/user_data.cpp)

target_include_directories(redwolf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..) 

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

if (RW_ENABLE_VULKAN)
    target_sources(redwolf PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backends/vulkan/vulkan_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backends/vulkan/vulkan_device_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/backends/vulkan/vulkan_renderer.cpp)
endif()

if(RW_ENABLE_PROFILING)
    target_link_libraries(redwolf PUBLIC TracyClient)
endif()
