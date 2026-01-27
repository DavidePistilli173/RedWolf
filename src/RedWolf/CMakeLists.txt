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
        gfx/text.cppm
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

# Copy assets to bin folder
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets)

# Copy shaders to bin folder
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/shaders)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/shaders/quad_2d.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/shaders/quad_2d.glsl COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/shaders/text_2d.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/shaders/text_2d.glsl COPYONLY)

# Copy fonts to bin folder
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbi.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbi.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbl.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbl.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbmo.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbmo.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbmr.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbmr.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbso.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbso.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbsr.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbsr.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbtl.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbtl.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbto.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbto.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunbx.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunbx.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunci.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunci.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunit.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunit.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunobi.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunobi.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunobx.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunobx.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunorm.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunorm.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunoti.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunoti.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunrm.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunrm.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunsi.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunsi.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunsl.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunsl.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunso.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunso.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunss.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunss.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunssdc.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunssdc.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunsx.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunsx.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmuntb.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmuntb.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunti.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunti.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmuntt.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmuntt.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmuntx.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmuntx.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunui.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunui.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunvi.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunvi.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/cmunvt.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/cmunvt.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/cmu/LICENSE.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/cmu/LICENSE.txt COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/LICENSE)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/LICENSE/OFL.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/LICENSE/OFL.txt COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/LICENSE/OFL_FAQ.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/LICENSE/OFL_FAQ.txt COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/courier_prime.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/courier_prime.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/courier_prime_bold.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/courier_prime_bold.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/courier_prime_bold_italic.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/courier_prime_bold_italic.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/courier_prime_italic.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/courier_prime_italic.ttf COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/courier_prime/README.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/courier_prime/README.txt COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/zector)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/zector/License.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/zector/License.txt COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/RedWolf/gfx/fonts/zector/Zector.ttf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine_assets/fonts/zector/Zector.ttf COPYONLY)
