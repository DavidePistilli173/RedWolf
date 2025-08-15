add_library(RedWolf STATIC)

target_sources(RedWolf PRIVATE
        engine/app.cpp
        engine/entry_point.cpp
        evt/application_event.cpp
        evt/event.cpp
        evt/key_event.cpp
        evt/mouse_event.cpp
        layers/layer.cpp
        layers/layer_stack.cpp
        layers/debug_layer.cpp
        pl/gl/imgui_impl_opengl3.cpp
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
)

target_compile_options(RedWolf PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wshadow
)

target_link_libraries(RedWolf PUBLIC
        vendor
)