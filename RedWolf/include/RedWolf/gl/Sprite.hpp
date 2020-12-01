#ifndef RW_GL_SPRITE_HPP
#define RW_GL_SPRITE_HPP

#include "RedWolf/core.hpp"
#include "RedWolf/data/VBORect.hpp"
#include "RedWolf/gl/Texture.hpp"
#include "RedWolf/gl/VAO.hpp"
#include "RedWolf/shapes.hpp"

#include <array>

namespace rw::gl
{
   /** \brief Rectangular 2D sprite. */
   class RW_API Sprite
   {
   public:
      /** \brief Size of the VBO. */
      static constexpr size_t vbo_size{ 20 };
      /** \brief Size of the EBO. */
      static constexpr size_t ebo_size{ 6 };
      /** \brief EBO data. */
      static constexpr std::array<unsigned int, ebo_size> ebo_data{
         0U, 1U, 2U, 2U, 3U, 0U
      };

      /** \brief Default constructor. */
      explicit Sprite(const Texture& tex);

      /** \brief Draw the sprite with the currently bound shader. */
      void draw();

      /** \brief Get the height of the sprite. */
      float h() const;

      /** \brief Check whether the sprite was modified since the last draw call or not. */
      bool modified() const;

      /** \brief Set the height of the sprite. */
      void setH(float h);

      /** \brief Set the sprite's position. */
      void setPosition(float x, float y);

      /** \brief Set the sprite's position. */
      void setPosition(std::pair<float, float> position);

      /** \brief Set the new position and size of the sprite. */
      void setRect(const rw::Rect<float> rect);

      /** \brief Set the height of the texture.
          It is usually expressed between 0 and 1, where 1 means that the entire height of the source texture 
          is considered. Values outside this range rely on the texture's border policy.
      */
      void setTexH(float texH);

      /** \brief Set the width of the texture. 
          It is usually expressed between 0 and 1, where 1 means that the entire width of the source texture 
          is considered. Values outside this range rely on the texture's border policy.
      */
      void setTexW(float texW);

      /** \brief Set the x coordinate of the top-left corner of the texure. 
          It is usually expressed between 0 and 1, where 0 is the leftmost part of the texture. 
          Values outside this range rely on the texture's border policy.
      */
      void setTexX(float texX);

      /** \brief Set the y coordinate of the top-left corner of the texture. 
          It is usually expressed between 0 and 1, where 0 is the upmost part of the texture. 
          Values outside this range rely on the texture's border policy.
      */
      void setTexY(float texY);

      /** \brief Set the width of the sprite. */
      void setW(float w);

      /** \brief Set the x coordinate of the top-left corner. */
      void setX(float x);

      /** \brief Set the y coordinate of the top-left corner. */
      void setY(float y);

      /** \brief Set the z coordinate of the sprite. */
      void setZ(float z);

      /** \brief Get the height of the texture. 
          It is usually expressed between 0 and 1, where 1 means that the entire height of the source texture 
          is considered. Values outside this range rely on the texture's border policy.
      */
      float texH() const;

      /** \brief Get the width of the texture. 
          It is usually expressed between 0 and 1, where 1 means that the entire width of the source texture 
          is considered. Values outside this range rely on the texture's border policy.
      */
      float texW() const;

      /** \brief Get the x coordinate of the top-left corner of the texture. 
          It is usually expressed between 0 and 1, where 0 is the leftmost part of the texture. 
          Values outside this range rely on the texture's border policy.
      */
      float texX() const;

      /** \brief Get the y coordinate of the top-left corner of the texture. 
          It is usually expressed between 0 and 1, where 0 is the upmost part of the texture. 
          Values outside this range rely on the texture's border policy.
      */
      float texY() const;

      /** \brief Set the width of the sprite. */
      float w() const;

      /** \brief Get the x coordinate of the top-left corner of the sprite. */
      float x() const;

      /** \brief Get the y coordinate of the top-left corner of the sprite. */
      float y() const;

      /** \brief Get the z coordinate of the sprite. */
      float z() const;

   private:
      rw::data::VBORect       vbo_data_; /**< Position and size. */
      VAO<vbo_size, ebo_size> vao_;      /**< Vertex Array Object. */
      const Texture&          tex_;      /**< Texture. */
      /** \brief True if the sprite's position and/or size were modified since the last draw call. */
      bool modified_{ true };
   };
} // namespace rw::gl

#endif