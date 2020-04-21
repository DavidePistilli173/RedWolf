#include "utility.hpp"

#include "FixedQueueTest.cpp"
#include "gl/BufferTest.cpp"
#include "gl/TextureTest.cpp"
#include "LogTest.cpp"

#include <gtest/gtest.h>
#include <RedWolf/gl/init.hpp>
#include <SDL2_image2.0.4/SDL_image.h>
#include <SDL2_ttf2.0.15/SDL_ttf.h>

constexpr std::string_view win_name{ "RedWolfTests" };

int main(int argc, char* args[])
{
    if (!rw::Log::init(rw::Log::Mode::file_only)) return 1;

    /* Initialise SDL library, exit on failure. */
    /* Init SDL's video, audio and timer subsystems. */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        rw::Log::app_err("SDL initialisation failed:\n    {}", SDL_GetError());
        return 1;
    }
    /* Init SDL_Image for png and jpg file loading. */
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        rw::Log::app_err("SDL_image initialisation failed:\n    {}", IMG_GetError());
        return 1;
    }
    /* Init SDL_TTF. */
    if (TTF_Init() == -1)
    {
        rw::Log::app_err("SDL_ttf initialisation failed:\n    {}", TTF_GetError());
        return 1;
    }

    rw::win_ptr_t win{ rwt::setUpGLContext(win_name) };
    if (win == nullptr) return 1;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(rw::gl::openGLError, nullptr);

    rw::gl::init(); // Load OpenGL calls for RedWolf.dll

    ::testing::InitGoogleTest(&argc, args);
    int retVal{ RUN_ALL_TESTS() };

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return retVal;
}