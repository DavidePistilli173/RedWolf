#include "FixedQueueTest.cpp"
#include "LogTest.cpp"
#include "gl/BufferTest.cpp"
#include "gl/TextureTest.cpp"
#include "utility.hpp"

#include <RedWolf/init.hpp>
#include <RedWolf/ui/Window.hpp>
#include <SDL2_image2.0.4/SDL_image.h>
#include <SDL2_ttf2.0.15/SDL_ttf.h>
#include <gtest/gtest.h>

constexpr std::string_view win_name{ "RedWolfTests" };

int main(int argc, char* args[])
{
   rw::SubsystemsHandler core;
   rw::ui::Window        win{ win_name };

   ::testing::InitGoogleTest(&argc, args);
   int retVal{ RUN_ALL_TESTS() };

   TTF_Quit();
   IMG_Quit();
   SDL_Quit();
   return retVal;
}