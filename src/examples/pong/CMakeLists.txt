add_executable(Pong)

target_sources(Pong PRIVATE
        FILE_SET CXX_MODULES FILES
        "pong_app.cppm"
        "pong_main_layer.cppm"
)

target_link_libraries(Pong PRIVATE RedWolf)

# Copy shaders to bin folder
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures)
configure_file(${PROJECT_SOURCE_DIR}/src/examples/pong/assets/textures/ball.png ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures/ball.png COPYONLY)