set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

add_library(vendor STATIC)

target_sources(vendor PRIVATE
        glfw.cpp
)

target_include_directories(vendor PUBLIC
        ${PROJECT_SOURCE_DIR}/vendor/glfw/include
)

target_link_libraries(vendor PUBLIC
        glfw
        ${GLFW_LIBRARIES}
)