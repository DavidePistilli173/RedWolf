#include "RedWolf/init.hpp"

#include "RedWolf/Log.hpp"
#include "RedWolf/gl/error.hpp"
#include "SDL2.0.9/SDL.h"
#include "SDL2_image2.0.4/SDL_image.h"
#include "SDL2_ttf2.0.15/SDL_ttf.h"
#include "glad/glad.h"

#include <cstdio>
#include <exception>

using namespace rw;

bool SubsystemsHandler::initialised_{ false };

SubsystemsHandler::SubsystemsHandler()
{
   // Subsystems can only be initialised once.
   if (initialised_)
   {
      Log::core_err("Subsystems already initialised.");
      throw std::exception{};
   }
   initialised_ = true;

   // Initialise the logger.
   if (!rw::Log::init(rw::Log::Mode::file_only))
   {
      std::printf("Failed to initialise logger.");
      throw std::exception{};
   }

   // Initialise SDL library, throw on failure.
   // Init SDL's video, audio and timer subsystems.
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
   {
      rw::Log::core_err("SDL initialisation failed:\n    {}", SDL_GetError());
      throw std::exception{};
   }
   // Init SDL_Image for png and jpg file loading.
   if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
   {
      rw::Log::core_err("SDL_image initialisation failed:\n    {}", IMG_GetError());
      throw std::exception{};
   }
   // Init SDL_TTF.
   if (TTF_Init() == -1)
   {
      rw::Log::core_err("SDL_ttf initialisation failed:\n    {}", TTF_GetError());
      throw std::exception{};
   }

   /* Set OpenGL version. */
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, rw::gl_version.first);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, rw::gl_version.second);
   /* Set core profile for OpenGL. */
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

SubsystemsHandler::~SubsystemsHandler()
{
   TTF_Quit();
   IMG_Quit();
   SDL_Quit();
}
