############################# ZLIB #############################
set(ZLIB_BUILD_TESTING OFF)
set(ZLIB_BUILD_STATIC ON)
set(ZLIB_BUILD_SHARED OFF)
set(ZLIB_INSTALL OFF)
set(ZLIB_INSTALL_COMPAT_DLL OFF)
add_subdirectory(zlib)
if (TARGET zlibstatic)
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
    add_library(zlib ALIAS zlibstatic) # optional: some projects look for 'zlib'

    # Satisfy legacy FindZLIB checks (used by libpng and others)
    set(ZLIB_LIBRARY ZLIB::ZLIB)
    set(ZLIB_LIBRARIES ZLIB::ZLIB)
    set(ZLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/vendor/zlib)
    set(ZLIB_FOUND TRUE)
endif ()

############################# LIBPNG #############################
set(PNG_SHARED OFF)
set(PNG_TESTS OFF)
set(SKIP_INSTALL_ALL ON)
add_subdirectory(libpng)
if (TARGET png_static)
    add_library(PNG::PNG ALIAS png_static)
    add_library(png ALIAS png_static)

    # satisfy modern and legacy consumers
    set(PNG_LIBRARY PNG::PNG)
    set(PNG_LIBRARIES PNG::PNG)
    set(PNG_PNG_LIBRARY PNG::PNG)
    set(PNG_PNG_LIBRARIES PNG::PNG)

    set(PNG_PNG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/vendor/libpng)
    set(PNG_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/vendor/libpng)
    set(PNG_FOUND TRUE)
endif ()

############################# Freetype #############################
add_library(freetype STATIC)

target_sources(freetype PRIVATE
        freetype/src/autofit/autofit.c
        freetype/src/base/ftbase.c
        freetype/src/base/ftbbox.c
        freetype/src/base/ftbdf.c
        freetype/src/base/ftbitmap.c
        freetype/src/base/ftcid.c
        freetype/src/base/ftdebug.c
        freetype/src/base/ftfstype.c
        freetype/src/base/ftgasp.c
        freetype/src/base/ftglyph.c
        freetype/src/base/ftgxval.c
        freetype/src/base/ftinit.c
        freetype/src/base/ftmm.c
        freetype/src/base/ftotval.c
        freetype/src/base/ftpatent.c
        freetype/src/base/ftpfr.c
        freetype/src/base/ftstroke.c
        freetype/src/base/ftsynth.c
        freetype/src/base/ftsystem.c
        freetype/src/base/fttype1.c
        freetype/src/base/ftwinfnt.c
        freetype/src/bdf/bdf.c
        freetype/src/bzip2/ftbzip2.c
        freetype/src/cache/ftcache.c
        freetype/src/cff/cff.c
        freetype/src/cid/type1cid.c
        freetype/src/gzip/ftgzip.c
        freetype/src/lzw/ftlzw.c
        freetype/src/pcf/pcf.c
        freetype/src/pfr/pfr.c
        freetype/src/psaux/psaux.c
        freetype/src/pshinter/pshinter.c
        freetype/src/psnames/psnames.c
        freetype/src/raster/raster.c
        freetype/src/sdf/sdf.c
        freetype/src/sfnt/sfnt.c
        freetype/src/smooth/smooth.c
        freetype/src/truetype/truetype.c
        freetype/src/type1/type1.c
        freetype/src/type42/type42.c
        freetype/src/winfonts/winfnt.c
)

target_include_directories(freetype PUBLIC
        ${PROJECT_SOURCE_DIR}/src/vendor/freetype/include
)

target_compile_definitions(freetype PRIVATE
        FT2_BUILD_LIBRARY
)

add_library(Freetype::Freetype ALIAS freetype)

set(FREETYPE_LIBRARY Freetype::Freetype)
set(FREETYPE_LIBRARIES Freetype::Freetype)
set(FREETYPE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/vendor/freetype/include)
set(FREETYPE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/vendor/freetype/include)
set(FREETYPE_FOUND TRUE)

############################# GLAD #############################
add_subdirectory(glad)

############################# GLFW #############################
set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)

############################# GLM #############################
set(GLM_BUILD_TESTS OFF)
set(GLM_ENABLE_CXX_20 ON)
add_subdirectory(glm)

############################# IMGUI #############################
add_library(imgui STATIC)
target_sources(imgui PRIVATE
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
        ${PROJECT_SOURCE_DIR}/src/vendor
        ${PROJECT_SOURCE_DIR}/src/vendor/imgui
)

target_compile_definitions(imgui PRIVATE
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_link_libraries(imgui PUBLIC
        glfw
)

############################# MSDF ATLAS GEN #############################
set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_USE_VCPKG OFF)
set(MSDF_ATLAS_USE_SKIA OFF)
add_subdirectory(msdf_atlas_gen)

############################# Vendor Library #############################
add_library(vendor STATIC)

target_sources(vendor PRIVATE
        glfw.cpp
        stb.cpp
)

target_include_directories(vendor PUBLIC
        ${PROJECT_SOURCE_DIR}/src/vendor/glfw/include
        ${PROJECT_SOURCE_DIR}/src/vendor/glm
)

target_link_libraries(vendor PUBLIC
        glad
        glfw
        glm::glm
        imgui
        msdf-atlas-gen
        PNG::PNG
        ZLIB::ZLIB
)