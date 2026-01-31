add_executable(RedWolfSandbox)

target_sources(RedWolfSandbox PRIVATE
        FILE_SET CXX_MODULES FILES
        sandbox_2_d.cppm
        sandbox_app.cppm
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