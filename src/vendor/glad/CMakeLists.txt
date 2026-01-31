add_library(glad STATIC)

target_sources(glad PRIVATE
        src/glad.c
)

target_include_directories(glad PUBLIC
        ${PROJECT_SOURCE_DIR}/src/vendor/glad/include
)

target_link_libraries(glad)