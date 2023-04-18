#ifndef RW_CORE_CONCEPTS_HPP
#define RW_CORE_CONCEPTS_HPP

#include "RedWolf/common.hpp"

#include <type_traits>

namespace rw::core
{
   /**
    * @brief Concept for classes that must be derived from a specific class.
    * @tparam T Class to check.
    * @tparam U Base class.
    */
   template<typename T, typename U>
   concept IsDerivedFrom = std::is_base_of_v<U, T>;
} // namespace rw::core

#endif