add_executable(RedWolfSandbox)

target_sources(RedWolfSandbox PRIVATE
        sandbox_app.cpp
)

target_link_libraries(RedWolfSandbox PRIVATE RedWolf)