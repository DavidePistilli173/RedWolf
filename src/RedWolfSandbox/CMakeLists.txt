add_executable(RedWolfSandbox)

target_sources(RedWolfSandbox PRIVATE
        sandbox_2_d.cpp
        sandbox_app.cpp
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

target_link_libraries(RedWolfSandbox PRIVATE RedWolf)