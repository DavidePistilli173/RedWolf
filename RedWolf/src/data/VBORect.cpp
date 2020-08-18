#include "RedWolf/data/VBORect.hpp"

namespace rw::data
{
   /** \param rect Rectangle data. */
   VBORect::VBORect(const rw::Rect<float>& rect)
   {
      setRect(rect);
   }

   /**
      \param rect Coordinate rectangle.
      \param texRect Texture rectangle.
   */
   VBORect::VBORect(const rw::Rect<float>& rect, const rw::Rect<float>& texRect)
   {
      setRect(rect);
      setTexRect(texRect);
   }

   /** \return Const reference to the data array. */
   const std::array<float, VBORect::size>& VBORect::data() const
   {
      return data_;
   }

   /** \param h New height. */
   void VBORect::setH(float h)
   {
      float oldHeight{ data_[static_cast<size_t>(VtxIdx::bl_y)] - data_[static_cast<size_t>(VtxIdx::tl_y)] };
      float diff{ h - oldHeight };
      data_[static_cast<size_t>(VtxIdx::bl_y)] += diff;
      data_[static_cast<size_t>(VtxIdx::br_y)] = data_[static_cast<size_t>(VtxIdx::bl_y)];
   }

   /** \param rect New rectangle data. */
   void VBORect::setRect(const rw::Rect<float>& rect)
   {
      data_[static_cast<size_t>(VtxIdx::tl_x)] = rect.x;
      data_[static_cast<size_t>(VtxIdx::tl_y)] = rect.y;
      data_[static_cast<size_t>(VtxIdx::tr_x)] = rect.x + rect.w;
      data_[static_cast<size_t>(VtxIdx::tr_y)] = rect.y;
      data_[static_cast<size_t>(VtxIdx::br_x)] = data_[static_cast<size_t>(VtxIdx::tr_x)];
      data_[static_cast<size_t>(VtxIdx::br_y)] = rect.y + rect.h;
      data_[static_cast<size_t>(VtxIdx::bl_x)] = rect.x;
      data_[static_cast<size_t>(VtxIdx::bl_y)] = data_[static_cast<size_t>(VtxIdx::br_y)];
   }

   /** \param texH New normalised height. */
   void VBORect::setTexH(float texH)
   {
      float oldHeight{ data_[static_cast<size_t>(TexIdx::bl_y)] - data_[static_cast<size_t>(TexIdx::tl_y)] };
      float diff{ texH - oldHeight };
      data_[static_cast<size_t>(TexIdx::bl_y)] += diff;
      data_[static_cast<size_t>(TexIdx::br_y)] = data_[static_cast<size_t>(TexIdx::bl_y)];
   }

   /** \param texRect New texture data. */
   void VBORect::setTexRect(const rw::Rect<float>& texRect)
   {
      data_[static_cast<size_t>(TexIdx::tl_x)] = texRect.x;
      data_[static_cast<size_t>(TexIdx::tl_y)] = texRect.y;
      data_[static_cast<size_t>(TexIdx::tr_x)] = texRect.x + texRect.w;
      data_[static_cast<size_t>(TexIdx::tr_y)] = texRect.y;
      data_[static_cast<size_t>(TexIdx::br_x)] = data_[static_cast<size_t>(TexIdx::tr_x)];
      data_[static_cast<size_t>(TexIdx::br_y)] = texRect.y + texRect.h;
      data_[static_cast<size_t>(TexIdx::bl_x)] = texRect.x;
      data_[static_cast<size_t>(TexIdx::bl_y)] = data_[static_cast<size_t>(TexIdx::br_y)];
   }

   /** \param texW New normalised width.*/
   void VBORect::setTexW(float texW)
   {
      float oldWidth{ data_[static_cast<size_t>(TexIdx::tr_x)] - data_[static_cast<size_t>(TexIdx::tl_x)] };
      float diff{ texW - oldWidth };
      data_[static_cast<size_t>(TexIdx::tr_x)] += diff;
      data_[static_cast<size_t>(TexIdx::br_x)] = data_[static_cast<size_t>(TexIdx::tr_x)];
   }

   /** \param texX New normalised x coordinate. */
   void VBORect::setTexX(float texX)
   {
      float width{ data_[static_cast<size_t>(TexIdx::tr_x)] - data_[static_cast<size_t>(TexIdx::tl_x)] };
      data_[static_cast<size_t>(TexIdx::tl_x)] = texX;
      data_[static_cast<size_t>(TexIdx::bl_x)] = texX;
      data_[static_cast<size_t>(TexIdx::tr_x)] = texX + width;
      data_[static_cast<size_t>(TexIdx::br_x)] = data_[static_cast<size_t>(TexIdx::tr_x)];
   }

   /** \param New normalised y coordinate. */
   void VBORect::setTexY(float texY)
   {
      float height{ data_[static_cast<size_t>(TexIdx::bl_y)] - data_[static_cast<size_t>(TexIdx::tl_y)] };
      data_[static_cast<size_t>(TexIdx::tl_y)] = texY;
      data_[static_cast<size_t>(TexIdx::tr_y)] = texY;
      data_[static_cast<size_t>(TexIdx::bl_y)] = texY + height;
      data_[static_cast<size_t>(TexIdx::br_y)] = data_[static_cast<size_t>(TexIdx::bl_y)];
   }

   /** \param w New width. */
   void VBORect::setW(float w)
   {
      float oldWidth{ data_[static_cast<size_t>(VtxIdx::tr_x)] - data_[static_cast<size_t>(VtxIdx::tl_x)] };
      float diff{ w - oldWidth };
      data_[static_cast<size_t>(VtxIdx::tr_x)] += diff;
      data_[static_cast<size_t>(VtxIdx::br_x)] = data_[static_cast<size_t>(VtxIdx::tr_x)];
   }

   /** \param x New x coordinate. */
   void VBORect::setX(float x)
   {
      float width{ data_[static_cast<size_t>(VtxIdx::tr_x)] - data_[static_cast<size_t>(VtxIdx::tl_x)] };
      data_[static_cast<size_t>(VtxIdx::tl_x)] = x;
      data_[static_cast<size_t>(VtxIdx::bl_x)] = x;
      data_[static_cast<size_t>(VtxIdx::tr_x)] = x + width;
      data_[static_cast<size_t>(VtxIdx::br_x)] = data_[static_cast<size_t>(VtxIdx::tr_x)];
   }

   /** \param y New y coordinate. */
   void VBORect::setY(float y)
   {
      float height{ data_[static_cast<size_t>(VtxIdx::bl_y)] - data_[static_cast<size_t>(VtxIdx::tl_y)] };
      data_[static_cast<size_t>(VtxIdx::tl_y)] = y;
      data_[static_cast<size_t>(VtxIdx::tr_y)] = y;
      data_[static_cast<size_t>(VtxIdx::bl_y)] = y + height;
      data_[static_cast<size_t>(VtxIdx::br_y)] = data_[static_cast<size_t>(VtxIdx::br_y)];
   }

   /** \param z New z coordinate. */
   void VBORect::setZ(float z)
   {
      data_[static_cast<size_t>(VtxIdx::tl_z)] = z;
      data_[static_cast<size_t>(VtxIdx::tr_z)] = z;
      data_[static_cast<size_t>(VtxIdx::br_z)] = z;
      data_[static_cast<size_t>(VtxIdx::bl_z)] = z;
   }
} // namespace rw::data