# GLAD
add_subdirectory(glad)

# GLFW
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# GLM
set(GLM_BUILD_TESTS OFF)
set(GLM_ENABLE_CXX_20 ON)
add_subdirectory(glm)

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
        ${PROJECT_SOURCE_DIR}/vendor/glm
)

target_link_libraries(vendor PUBLIC
        glad
        glfw
        ${GLFW_LIBRARIES}
        glm::glm
        imgui
)