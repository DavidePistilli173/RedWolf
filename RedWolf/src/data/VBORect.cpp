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

   float VBORect::h() const
   {
      return data_[static_cast<size_t>(VtxIdx::bl_y)] - data_[static_cast<size_t>(VtxIdx::tl_y)];
   }

   /** \param h New height. */
   void VBORect::setH(float h)
   {
      float diff{ h - this->h() };
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
      float diff{ texH - this->texH() };
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
      float diff{ texW - this->texW() };
      data_[static_cast<size_t>(TexIdx::tr_x)] += diff;
      data_[static_cast<size_t>(TexIdx::br_x)] = data_[static_cast<size_t>(TexIdx::tr_x)];
   }

   /** \param texX New normalised x coordinate. */
   void VBORect::setTexX(float texX)
   {
      float width{ this->texW() };
      data_[static_cast<size_t>(TexIdx::tl_x)] = texX;
      data_[static_cast<size_t>(TexIdx::bl_x)] = texX;
      data_[static_cast<size_t>(TexIdx::tr_x)] = texX + width;
      data_[static_cast<size_t>(TexIdx::br_x)] = data_[static_cast<size_t>(TexIdx::tr_x)];
   }

   /** \param New normalised y coordinate. */
   void VBORect::setTexY(float texY)
   {
      float height{ this->texH() };
      data_[static_cast<size_t>(TexIdx::tl_y)] = texY;
      data_[static_cast<size_t>(TexIdx::tr_y)] = texY;
      data_[static_cast<size_t>(TexIdx::bl_y)] = texY + height;
      data_[static_cast<size_t>(TexIdx::br_y)] = data_[static_cast<size_t>(TexIdx::bl_y)];
   }

   /** \param w New width. */
   void VBORect::setW(float w)
   {
      float diff{ w - this->w() };
      data_[static_cast<size_t>(VtxIdx::tr_x)] += diff;
      data_[static_cast<size_t>(VtxIdx::br_x)] = data_[static_cast<size_t>(VtxIdx::tr_x)];
   }

   /** \param x New x coordinate. */
   void VBORect::setX(float x)
   {
      float width{ this->w() };
      data_[static_cast<size_t>(VtxIdx::tl_x)] = x;
      data_[static_cast<size_t>(VtxIdx::bl_x)] = x;
      data_[static_cast<size_t>(VtxIdx::tr_x)] = x + width;
      data_[static_cast<size_t>(VtxIdx::br_x)] = data_[static_cast<size_t>(VtxIdx::tr_x)];
   }

   /** \param y New y coordinate. */
   void VBORect::setY(float y)
   {
      float height{ this->h() };
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

   /** \brief Get the height of the texture. */
   float VBORect::texH() const
   {
      return data_[static_cast<size_t>(TexIdx::bl_y)] - data_[static_cast<size_t>(TexIdx::tl_y)];
   }

   /** \brief Get the width of the texture. */
   float VBORect::texW() const
   {
      return data_[static_cast<size_t>(TexIdx::tr_x)] - data_[static_cast<size_t>(TexIdx::tl_x)];
   }

   /** \brief Get the x coordinate of the texture's top-left corner. */
   float VBORect::texX() const
   {
      return data_[static_cast<size_t>(TexIdx::tl_x)];
   }

   /** \brief Get the y coordinate of the texture's top-left corner. */
   float VBORect::texY() const
   {
      return data_[static_cast<size_t>(TexIdx::tl_y)];
   }

   float VBORect::w() const
   {
      return data_[static_cast<size_t>(VtxIdx::tr_x)] - data_[static_cast<size_t>(VtxIdx::tl_x)];
   }

   float VBORect::x() const
   {
      return data_[static_cast<size_t>(VtxIdx::tl_x)];
   }

   float VBORect::y() const
   {
      return data_[static_cast<size_t>(VtxIdx::tl_y)];
   }

   float VBORect::z() const
   {
      return data_[static_cast<size_t>(VtxIdx::tl_z)];
   }
} // namespace rw::data