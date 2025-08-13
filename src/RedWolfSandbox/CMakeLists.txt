add_executable(RedWolfSandbox)

target_sources(RedWolfSandbox PRIVATE
        sandbox_app.cpp
)

target_compile_options(RedWolfSandbox PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wshadow
)

target_link_libraries(RedWolfSandbox PRIVATE RedWolf)