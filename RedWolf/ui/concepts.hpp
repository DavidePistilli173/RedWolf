#ifndef RW_UI_CONCEPTS_HPP
#define RW_UI_CONCEPTS_HPP

#include "RedWolf/ui/BaseWindow.hpp"

#include <concepts>
#include <type_traits>

namespace rw::ui
{
   template<typename T>
   concept IsDerivedFromBaseWindow = std::is_base_of_v<BaseWindow, T>;
}

#endif