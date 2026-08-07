add_library(redwolf STATIC)

target_sources(redwolf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/entry_point.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/profiler.cpp)

target_include_directories(redwolf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_libraries(redwolf PUBLIC wayland-client)

    function(add_wayland_protocol_target TARGET_NAME SPEC NAME)
        add_custom_target(wayland_${TARGET_NAME}_header COMMAND wayland-scanner client-header /usr/share/wayland-protocols/${SPEC} ${CMAKE_CURRENT_SOURCE_DIR}/platform/${NAME}.h)
        add_dependencies(redwolf wayland_${TARGET_NAME}_header)
        add_custom_target(wayland_${TARGET_NAME}_source COMMAND wayland-scanner private-code /usr/share/wayland-protocols/${SPEC} ${CMAKE_CURRENT_SOURCE_DIR}/platform/${NAME}.c)
        add_dependencies(redwolf wayland_${TARGET_NAME}_source)
    endfunction()
    add_wayland_protocol_target(xdg_shell "stable/xdg-shell/xdg-shell.xml" xdg-shell)

    target_sources(redwolf PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/platform_linux.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/xdg-shell.c
    )
endif()

if(RW_ENABLE_PROFILING)
    target_link_libraries(redwolf PUBLIC TracyClient)
endif()
