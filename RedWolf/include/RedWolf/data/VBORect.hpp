#ifndef RW_DATA_VBORECT_HPP
#define RW_DATA_VBORECT_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/shapes.hpp"

#include <array>

namespace rw::data
{
   /** \brief Data structure containing data for a rectangle's VBO. */
   class RW_API VBORect
   {
   public:
      /** \brief Indexes of the vertex data inside the data structure. */
      enum class VtxIdx
      {
         tl_x = 0,  // Top-left, x
         tl_y = 1,  // Top-left, y
         tl_z = 2,  // Top-left, z
         tr_x = 5,  // Top-right, x
         tr_y = 6,  // Top-right, y
         tr_z = 7,  // Top-right, z
         br_x = 10, // Bottom-right, x
         br_y = 11, // Bottom-right, y
         br_z = 12, // Bottom-right, z
         bl_x = 15, // Bottom-left, x
         bl_y = 16, // Bottom-left, y
         bl_z = 17  // Bottom-left, z
      };

      /** \brief Indexes of the texture data inside the data structure. */
      enum class TexIdx
      {
         tl_x = 3,  // Top-left, x
         tl_y = 4,  // Top-left, y
         tr_x = 8,  // Top-right, x
         tr_y = 9,  // Top-right, y
         br_x = 13, // Bottom-right, x
         br_y = 14, // Bottom-right, y
         bl_x = 18, // Bottom-left, x
         bl_y = 19  // Bottom-left, y
      };

      /** \brief Size of the VBO. */
      static constexpr size_t size{ 20 };

      /** \brief Default constructor. */
      VBORect() = default;

      /** \brief Construct a VBORect from an existing rectangle. */
      explicit VBORect(const rw::Rect<float>& rect);

      /** \brief Construct a VBORect from existing coordinate and texture rectangles. */
      VBORect(const rw::Rect<float>& rect, const rw::Rect<float>& texRect);

      /** \brief Get the data stored inside the structure. */
      const std::array<float, size>& data() const;

      /** \brief Set the height of the rectangle. */
      void setH(float h);

      /** \brief Set new rectangle data. */
      void setRect(const rw::Rect<float>& rect);

      /** \brief Set the height of the texture. */
      void setTexH(float texH);

      /** \brief Set the new texture data. */
      void setTexRect(const rw::Rect<float>& texRect);

      /** \brief Set the width of the texture. */
      void setTexW(float texW);

      /** \brief Set the x coordinate of the texture's top-left corner. */
      void setTexX(float texX);

      /** \brief Set the y coordinate of the texture's top-left corner. */
      void setTexY(float texY);

      /** \brief Set the width of the rectangle. */
      void setW(float w);

      /** \brief Set the x coordinate of the rectangle's top-left corner. */
      void setX(float x);

      /** \brief Set the y coordinate of the rectangle's top-left corner. */
      void setY(float y);

      /** \brief Set the z coordinate of the rectangle's top-left corner. */
      void setZ(float z);

   private:
      /** \brief Actual data. */
      std::array<float, size> data_{
         // Top-left corner
         0.0F,
         0.0F,
         0.0F,
         0.0F,
         0.0F,
         // Top-right corner
         0.0F,
         0.0F,
         0.0F,
         1.0F,
         0.0F,
         // Bottom-right corner
         0.0F,
         0.0F,
         0.0F,
         1.0F,
         1.0F,
         // Bottom-left corner
         0.0F,
         0.0F,
         0.0F,
         0.0F,
         1.0F
      };
   };
} // namespace rw::data

#endif