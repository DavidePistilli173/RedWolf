# GLFW
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# GLAD
add_subdirectory(glad)

# IMGUI
add_library(imgui STATIC)
target_sources(imgui PRIVATE
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
        ${PROJECT_SOURCE_DIR}/src/vendor
)

# Vendor Library
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
        glad
        imgui
)