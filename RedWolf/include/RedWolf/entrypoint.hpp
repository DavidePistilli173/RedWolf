/** 
    \file entrypoint.hpp 
    \brief Contains the entry point for the engine. 
*/

#ifndef RW_ENTRYPOINT_HPP
#define RW_ENTRYPOINT_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/gl/error.hpp"
#include "RedWolf/Log.hpp"

#include <glad/glad.h>
#include <SDL2.0.9/SDL.h>

#include <memory>

/** 
    \brief Application entry point.
    The entry point of an application that uses RedWolf is handled by the engine itself.
*/
int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_VIDEO); // Initialise SDL.

    if (!rw::Log::init(rw::Log::Mode::file_only)) return -1; // Initialise the engine logger.
    /* Enable OpenGL logging in debug mode.. */
    if constexpr (rw::debug)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(rw::gl::openGLError, nullptr);
    }

    auto app{ rw::createApp() };
    app->run();

    SDL_Quit();
    return 0;
}

#endif