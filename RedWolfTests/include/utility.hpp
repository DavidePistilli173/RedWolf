#ifndef REDWOLFTESTS_UTILITY_HPP
#define REDWOLFTESTS_UTILITY_HPP

#include <glad/glad.h>
#include <RedWolf/utility.hpp>
#include <SDL2.0.9/SDL.h>

#include <cstdio>
#include <memory>
#include <string_view>

namespace rwt
{
    /********** CONSTANTS **********/
    constexpr rw::Size<int> win_size{ 640, 480 };
    constexpr std::string_view data_folder{ "data/" };

    /********** FUNCTIONS **********/
    /* Set up a basic OpenGL window and context. */
    rw::win_ptr_t setUpGLContext(std::string_view winName)
    {
        /* Set OpenGL version. */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        /* Set core profile for OpenGL. */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        
        /* Create the window. */
        rw::win_ptr_t win{ rw::win_ptr_t(
            SDL_CreateWindow(winName.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win_size.w, win_size.h, SDL_WINDOW_OPENGL),
            SDL_DestroyWindow
        ) };
        if (win == nullptr)
        {
            std::printf("Could not create game window.\n");
            std::printf("%s", SDL_GetError());
            return win;
        }

        SDL_GLContext context{ SDL_GL_CreateContext(win.get()) };
        gladLoadGLLoader(SDL_GL_GetProcAddress);

        return win;
    }
}

#endif