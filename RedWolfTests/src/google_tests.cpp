#include "FixedQueueTest.cpp"
#include "LogTest.cpp"
#include "binary_searchTest.cpp"
#include "data/VBORectTest.cpp"
#include "gl/BufferTest.cpp"
#include "gl/ShaderTest.cpp"
#include "gl/SpriteTest.cpp"
#include "gl/TextureTest.cpp"
#include "gl/VAOTest.cpp"
#include "io/ParserTest.cpp"
#include "io/TextTest.cpp"
#include "utility.hpp"

#include <RedWolf/init.hpp>
#include <RedWolf/ui/Window.hpp>
#include <SDL2_image2.0.4/SDL_image.h>
#include <SDL2_ttf2.0.15/SDL_ttf.h>
#include <gtest/gtest.h>

namespace rwtests
{
   class TestWindow : public rw::ui::Window
   {
   public:
      /**< Name of the window. */
      static constexpr std::string_view win_name{ "RedWolfTests" };

      TestWindow() :
         Window(win_name)
      {
      }

      /**< Empty main loop. */
      bool run() override
      {
         return true;
      }

   private:
   };
} // namespace rwtests

int main(int argc, char* args[])
{
   rw::SubsystemsHandler core;
   rwtests::TestWindow   win{};

   ::testing::InitGoogleTest(&argc, args);
   int retVal{ RUN_ALL_TESTS() };

   return retVal;
}