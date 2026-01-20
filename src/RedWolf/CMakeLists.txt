add_library(RedWolf STATIC)

set(RW_GFX_API "RW_GFX_API_OPENGL" CACHE STRING "Rendering API for RedWolf")

target_sources(RedWolf PUBLIC
        FILE_SET CXX_MODULES FILES
        common/common.cppm
        core/animated.cppm
        core/asset_library.cppm
        core/core.cppm
        engine/app.cppm
        engine/camera_controller.cppm
        engine/engine.cppm
        engine/entry_point.cppm
        evt/application_event.cppm
        evt/event.cppm
        evt/evt.cppm
        evt/key_event.cppm
        evt/mouse_event.cppm
        gfx/buffer_layout.cppm
        gfx/camera.cppm
        gfx/common.cppm
        gfx/context.cppm
        gfx/font.cppm
        gfx/framebuffer.cppm
        gfx/framebuffer_descriptor.cppm
        gfx/gfx.cppm
        gfx/index_buffer.cppm
        gfx/quad.cppm
        gfx/renderer_2_d.cppm
        gfx/renderer_api.cppm
        gfx/renderer_interface_2_d.cppm
        gfx/shader.cppm
        gfx/shader_data.cppm
        gfx/texture_2d.cppm
        gfx/vertex_array.cppm
        gfx/vertex_buffer.cppm
        input/input.cppm
        input/keyboard.cppm
        input/mouse.cppm
        input/polling.cppm
        layers/debug_layer.cppm
        layers/layer.cppm
        layers/layer_stack.cppm
        layers/layers.cppm
        math/geometry.cppm
        math/math.cppm
        thread/thread.cppm
        thread/thread_pool.cppm
        time/date_time.cppm
        time/profiler.cppm
        time/profiler_data.cppm
        time/time.cppm
        ui/common.cppm
        ui/ui.cppm
        ui/window.cppm
        util/log_msg.cppm
        util/logger.cppm
        util/util.cppm
        redwolf.cppm
)

target_sources(RedWolf PRIVATE
        layers/debug_layer.cpp
)

if (RW_GFX_API STREQUAL "RW_GFX_API_OPENGL")
    target_sources(RedWolf PRIVATE
            gfx/api/gl/context.cpp
            gfx/api/gl/framebuffer.cpp
            gfx/api/gl/index_buffer.cpp
            gfx/api/gl/renderer_api.cpp
            gfx/api/gl/shader.cpp
            gfx/api/gl/texture_2d.cpp
            gfx/api/gl/vertex_array.cpp
            gfx/api/gl/vertex_buffer.cpp
    )
else ()
    message(FATAL_ERROR "Unsupported RW_GFX_API: ${RW_GFX_API}")
endif ()

target_include_directories(RedWolf PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_definitions(RedWolf PUBLIC
        GLFW_INCLUDE_NONE
        ${RW_GFX_API}
)

if (MSVC)
    target_compile_options(RedWolf PRIVATE
            /W3
            /WX
    )
else ()
    target_compile_options(RedWolf PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wconversion
            -Wshadow
    )
endif ()


target_link_libraries(RedWolf PUBLIC
        vendor
)

# Copy shaders to bin folder
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/shaders/base_2d.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/base_2d.glsl COPYONLY)