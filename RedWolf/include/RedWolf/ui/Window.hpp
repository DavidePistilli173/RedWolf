#ifndef RW_UI_WINDOW_HPP
#define RW_UI_WINDOW_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/shapes.hpp"
#include "RedWolf/utility.hpp"

#include <SDL2.0.9/SDL.h>
#include <memory>
#include <string_view>

namespace rw::ui
{
   /** \brief Graphical window that supports OpenGL rendering. */
   class RW_API Window
   {
   public:
      /** \brief Default name for the window. */
      static constexpr std::string_view def_name{ "RedWolf Application" };
      /** \brief Default position and size of the window. */
      static constexpr rw::Rect win_rect{ 100, 100, 800, 600 };

      /** \brief Create a window with a custom name. */
      explicit Window(std::string_view name = def_name, rw::Rect<int> rect = win_rect);

      /** \brief Destructor. */
      virtual ~Window() = default;

      /** 
         \brief Run the main loop of the window. 
         \return True if the execution completed successfully, false otherwise.
      */
      virtual bool run() = 0;

   protected:
      rw::win_ptr_t win_{ rw::win_ptr_t(nullptr, SDL_DestroyWindow) }; /**< Pointer to the window. */
      SDL_GLContext ctx_{ nullptr };                                   /**< OpenGL context. */
   };
} // namespace rw::ui

#endif