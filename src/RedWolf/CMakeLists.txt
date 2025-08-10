add_library(RedWolf STATIC)

target_sources(RedWolf PRIVATE
        core/app.cpp
        core/entry_point.cpp
)

target_include_directories(RedWolf PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(RedWolf PUBLIC)