module;

#include "RedWolf/common.hpp"

export module rw.core.concepts;

import <type_traits>;

export namespace rw::core
{
   /**
    * @brief Concept for classes that must be derived from a specific class.
    * @tparam T Class to check.
    * @tparam U Base class.
    */
   template<typename T, typename U>
   concept IsDerivedFrom = std::is_base_of_v<U, T>;
} // namespace rw::core