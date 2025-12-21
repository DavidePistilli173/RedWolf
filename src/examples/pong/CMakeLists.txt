add_executable(Pong)

target_sources(Pong PRIVATE
        "pong.cpp"
        "main_game.cpp"
)

target_link_libraries(Pong PRIVATE RedWolf)