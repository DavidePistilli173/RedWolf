#include "RedWolf/gl/Sprite.hpp"

#include <glad/glad.h>

namespace rw::gl
{
   Sprite::Sprite(const Texture& tex) :
      tex_{ tex }
   {
   }

   void Sprite::draw()
   {
      // Set the new drawing data.
      if (modified_)
      {
         vao_.setVBO(vbo_data_.data());
         modified_ = false;
      }

      // Draw the sprite.
      vao_.bind();
      tex_.bind();
      glDrawElements(GL_TRIANGLES, ebo_size, GL_UNSIGNED_INT, nullptr);
   }

   float Sprite::h() const
   {
      return vbo_data_.h();
   }

   bool Sprite::modified() const
   {
      return modified_;
   }

   /** \param h Value to be set. */
   void Sprite::setH(float h)
   {
      vbo_data_.setH(h);
      modified_ = true;
   }

   /** 
      \param x New x coordinate of the top-left corner of the sprite.
      \param y New y coordinate of the top-left corner of the sprite.
   */
   void Sprite::setPosition(float x, float y)
   {
      vbo_data_.setX(x);
      vbo_data_.setY(y);
      modified_ = true;
   }

   /** \param position New x and y coordinates of the sprite. */
   void Sprite::setPosition(std::pair<float, float> position)
   {
      vbo_data_.setX(position.first);
      vbo_data_.setY(position.second);
      modified_ = true;
   }

   /** \param rect Rectangle representing the new position and size of the sprite. */
   void Sprite::setRect(const rw::Rect<float> rect)
   {
      vbo_data_.setRect(rect);
      modified_ = true;
   }

   /** \param texH Value to be set. */
   void Sprite::setTexH(float texH)
   {
      vbo_data_.setTexH(texH);
      modified_ = true;
   }

   /** \param texW Value to be set. */
   void Sprite::setTexW(float texW)
   {
      vbo_data_.setTexW(texW);
      modified_ = true;
   }

   /** \param texX Value to be set. */
   void Sprite::setTexX(float texX)
   {
      vbo_data_.setTexX(texX);
      modified_ = true;
   }

   /** \param texY Value to be set. */
   void Sprite::setTexY(float texY)
   {
      vbo_data_.setTexY(texY);
      modified_ = true;
   }

   /** \param w Value to be set. */
   void Sprite::setW(float w)
   {
      vbo_data_.setW(w);
      modified_ = true;
   }

   /** \param x Value to be set. */
   void Sprite::setX(float x)
   {
      vbo_data_.setX(x);
      modified_ = true;
   }

   /** \param y Value to be set. */
   void Sprite::setY(float y)
   {
      vbo_data_.setY(y);
      modified_ = true;
   }

   /** \param z Value to be set. */
   void Sprite::setZ(float z)
   {
      vbo_data_.setZ(z);
      modified_ = true;
   }

   float Sprite::texH() const
   {
      return vbo_data_.texH();
   }

   float Sprite::texW() const
   {
      return vbo_data_.texW();
   }

   float Sprite::texX() const
   {
      return vbo_data_.texX();
   }

   float Sprite::texY() const
   {
      return vbo_data_.texY();
   }

   float Sprite::w() const
   {
      return vbo_data_.w();
   }

   float Sprite::x() const
   {
      return vbo_data_.x();
   }

   float Sprite::y() const
   {
      return vbo_data_.y();
   }

   float Sprite::z() const
   {
      return vbo_data_.z();
   }
} // namespace rw::gl