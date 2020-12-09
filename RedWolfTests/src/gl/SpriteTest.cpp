/* Tests for rw::gl::Sprite */
#include "utility.hpp"

#include <RedWolf/gl/Sprite.hpp>
#include <RedWolf/gl/error.hpp>
#include <RedWolf/shapes.hpp>
#include <gtest/gtest.h>

namespace gl_sprite_test
{
   constexpr std::string_view tt_tex_png{ "texture.png" };

   constexpr float rounding_error{ 10e-4 };
   constexpr float z{ 5.0F };

   constexpr rw::Rect<float> rect1{ 1.0F, 2.0F, 5.0F, 10.0F };
   constexpr rw::Rect<float> rect2{ 1.0F, 2.0F, -5.0F, 10.0F };
   constexpr rw::Rect<float> rect3{ 1.0F, 2.0F, 5.0F, -10.0F };
   constexpr rw::Rect<float> rect4{ 1.0F, 2.0F, -5.0F, -10.0F };

   constexpr rw::Rect<float> texRect1{ 0.2F, 0.4F, 0.8F, 1.0F };
   constexpr rw::Rect<float> texRect2{ 0.2F, 0.4F, -0.8F, 1.0F };
   constexpr rw::Rect<float> texRect3{ 0.2F, 0.4F, 0.8F, -1.0F };
   constexpr rw::Rect<float> texRect4{ 0.2F, 0.4F, -0.8F, -1.0F };

   void testSprite(rw::gl::Sprite& sprite, const rw::Rect<float>& rect, const rw::Rect<float>& texRect)
   {
      float refX{ sprite.x() };
      float refY{ sprite.y() };
      float refZ{ sprite.z() };
      float refW{ sprite.w() };
      float refH{ sprite.h() };
      float refTexX{ sprite.texX() };
      float refTexY{ sprite.texY() };
      float refTexW{ sprite.texW() };
      float refTexH{ sprite.texH() };

      auto testState = [&]() {
         ASSERT_NEAR(sprite.x(), refX, rounding_error);
         ASSERT_NEAR(sprite.y(), refY, rounding_error);
         ASSERT_NEAR(sprite.z(), refZ, rounding_error);
         ASSERT_NEAR(sprite.w(), refW, rounding_error);
         ASSERT_NEAR(sprite.h(), refH, rounding_error);
         ASSERT_NEAR(sprite.texX(), refTexX, rounding_error);
         ASSERT_NEAR(sprite.texY(), refTexY, rounding_error);
         ASSERT_NEAR(sprite.texW(), refTexW, rounding_error);
         ASSERT_NEAR(sprite.texH(), refTexH, rounding_error);

         sprite.draw();
         ASSERT_FALSE(sprite.modified());
         ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      };

      testState();

      // Test setH().
      refH = rect.h;
      sprite.setH(rect.h);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setPosition(float, float).
      refX = rect.x;
      refY = rect.y;
      sprite.setPosition(rect.x, rect.y);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setPosition(pair).
      refX = -rect.x;
      refY = -rect.y;
      std::pair position{ -rect.x, -rect.y };
      sprite.setPosition(position);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setRect().
      refX = rect.x;
      refY = rect.y;
      refW = rect.w;
      refH = rect.h;
      sprite.setRect(rect);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setTexH().
      refTexH = texRect.h;
      sprite.setTexH(texRect.h);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setTexW().
      refTexW = texRect.w;
      sprite.setTexW(texRect.w);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setTexX().
      refTexX = texRect.x;
      sprite.setTexX(texRect.x);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setTexY().
      refTexY = texRect.y;
      sprite.setTexY(texRect.y);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setW().
      refW = rect.w;
      sprite.setW(rect.w);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setX().
      refX = rect.x;
      sprite.setX(rect.x);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setY().
      refY = rect.y;
      sprite.setY(rect.y);
      ASSERT_TRUE(sprite.modified());
      testState();

      // Test setZ().
      refZ = z;
      sprite.setZ(z);
      ASSERT_TRUE(sprite.modified());
      testState();
   }
} // namespace gl_sprite_test

TEST(GLSpriteTest, Default)
{
   using namespace gl_sprite_test;

   std::string texPngFile{ rwt::data_folder };
   texPngFile += tt_tex_png;
   rw::gl::Texture tex{ texPngFile };
   rw::gl::Sprite  sprite{ tex };
   ASSERT_TRUE(sprite.modified());

   testSprite(sprite, rect1, texRect1);
   testSprite(sprite, rect2, texRect2);
   testSprite(sprite, rect3, texRect3);
   testSprite(sprite, rect4, texRect4);
   testSprite(sprite, rect1, texRect4);
   testSprite(sprite, rect2, texRect3);
   testSprite(sprite, rect3, texRect2);
   testSprite(sprite, rect4, texRect1);
}