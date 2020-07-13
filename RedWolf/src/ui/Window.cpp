#include "RedWolf/ui/Window.hpp"

#include "RedWolf/Log.hpp"
#include "RedWolf/gl/error.hpp"

#include <glad/glad.h>

namespace rw::ui
{
   /**
      \param name Name of the window.
      \param rect Position and size of the window.
   */
   Window::Window(std::string_view name, rw::Rect<int> rect)
   {
      win_ = rw::win_ptr_t(
         SDL_CreateWindow(name.data(), rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_OPENGL),
         SDL_DestroyWindow);
      if (win_ == nullptr)
      {
         Log::core_err("Failed to create window {1}.", name);
         throw std::exception{};
      }

      ctx_ = SDL_GL_CreateContext(win_.get());
      gladLoadGLLoader(SDL_GL_GetProcAddress);

      if constexpr (rw::debug)
      {
         glEnable(GL_DEBUG_OUTPUT);
         glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
         glDebugMessageCallback(rw::gl::openGLError, nullptr);
      }
   }
} // namespace rw::ui