#ifndef RW_SHAPES_HPP
#define RW_SHAPES_HPP

namespace rw
{
   /** \brief Rectangle. */
   template<typename T>
   struct Rect
   {
      /** \brief Default constructor. */
      constexpr Rect() = default;

      /** \brief Construct a rectangle by specifying its position and size. */
      constexpr Rect(T pX, T pY, T pW, T pH) :
         x{ pX }, y{ pY }, w{ pW }, h{ pH } {};

      T x{}; /**< X coordinate of the top-left corner. */
      T y{}; /**< Y coordinate of the top-left corner. */
      T w{}; /**< Width. */
      T h{}; /**< Height. */
   };
} // namespace rw

#endif