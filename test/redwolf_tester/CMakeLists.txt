add_executable(redwolf_tester)

target_sources(redwolf_tester PRIVATE generic_module.cpp redwolf_tester.cpp)

target_link_libraries(redwolf_tester PRIVATE redwolf)