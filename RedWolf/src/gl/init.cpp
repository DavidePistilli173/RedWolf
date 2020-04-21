#include "RedWolf/gl/init.hpp"

#include <glad/glad.h>
#include <SDL2.0.9/SDL.h>

namespace rw::gl
{
    void RW_API init()
    {
        gladLoadGLLoader(SDL_GL_GetProcAddress);
    }
}