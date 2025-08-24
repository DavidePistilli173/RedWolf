add_library(RedWolf STATIC
        gfx/camera.cpp
        gfx/camera.hpp)

set(RW_GFX_API "RW_GFX_API_OPENGL" CACHE STRING "Rendering API for RedWolf")

target_sources(RedWolf PRIVATE
        engine/app.cpp
        engine/entry_point.cpp
        evt/application_event.cpp
        evt/event.cpp
        evt/key_event.cpp
        evt/mouse_event.cpp
        gfx/api/gl/gl_context.cpp
        gfx/api/gl/gl_index_buffer.cpp
        gfx/api/gl/gl_renderer_api.cpp
        gfx/api/gl/gl_shader.cpp
        gfx/api/gl/gl_vertex_array.cpp
        gfx/api/gl/gl_vertex_buffer.cpp
        gfx/buffer_layout.cpp
        gfx/camera.cpp
        gfx/renderer_interface.cpp
        gfx/renderer.cpp
        input/polling.cpp
        layers/layer.cpp
        layers/layer_stack.cpp
        layers/debug_layer.cpp
        math/math.cpp
        thread/thread_pool.cpp
        time/date_time.cpp
        ui/window.cpp
        util/log_msg.cpp
        util/logger.cpp
)

target_include_directories(RedWolf PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_definitions(RedWolf PUBLIC
        GLFW_INCLUDE_NONE
        ${RW_GFX_API}
)

if (MSVC)
    target_compile_options(RedWolf PRIVATE
            /W4
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