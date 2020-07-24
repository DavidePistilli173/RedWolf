#ifndef REDWOLFTESTS_UTILITY_HPP
#define REDWOLFTESTS_UTILITY_HPP

#include <RedWolf/utility.hpp>
#include <SDL2.0.9/SDL.h>
#include <cstdio>
#include <glad/glad.h>
#include <memory>
#include <string_view>

namespace rwt
{
   /********** CONSTANTS **********/
   constexpr rw::Size<int>    win_size{ 640, 480 };
   constexpr std::string_view data_folder{ "data/" };
} // namespace rwt

#endif