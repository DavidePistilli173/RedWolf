#include "utility.hpp"

#include <RedWolf/gl/Texture.hpp>
#include <RedWolf/gl/error.hpp>
#include <RedWolf/utility.hpp>
#include <gtest/gtest.h>
#include <string_view>

constexpr std::string_view tt_font{ "font.ttf" };
constexpr std::string_view tt_tex_png{ "texture.png" };
constexpr std::string_view tt_tex_jpg{ "texture.jpg" };
constexpr std::string_view tt_text{ "This is a nice texture." };

constexpr int tt_font_size{ 50 };

void testTexture(rw::gl::Texture& tex);

TEST(GLTexture, Default)
{
   rw::gl::Texture tex;
   testTexture(tex);
}

TEST(GLTexture, FromFile)
{
   /* Load non-existing texture. */
   EXPECT_THROW(rw::gl::Texture{ "nonexisting texture" }, std::exception);

   /* Load png texture. */
   std::string texPngFile{ rwt::data_folder };
   texPngFile += tt_tex_png;

   rw::gl::Texture texPng;
   ASSERT_NO_THROW(texPng = rw::gl::Texture{ texPngFile });
   testTexture(texPng);

   /* Load jpg texture. */
   std::string texJpgFile{ rwt::data_folder };
   texJpgFile += tt_tex_jpg;

   rw::gl::Texture texJpg;
   ASSERT_NO_THROW(texJpg = rw::gl::Texture{ texJpgFile });
   testTexture(texJpg);
}

TEST(GLTexture, FromText)
{
   std::string fontFile{ rwt::data_folder };
   fontFile += tt_font;

   rw::font_ptr_t font{ rw::font_ptr_t(TTF_OpenFont(fontFile.c_str(), tt_font_size), TTF_CloseFont) };
   ASSERT_NE(font, nullptr);

   SDL_Color       colour{ 20, 20, 20 };
   rw::gl::Texture tex;
   ASSERT_NO_THROW(tex = rw::gl::Texture(tt_text, colour, font));
   testTexture(tex);
}

void testTexture(rw::gl::Texture& tex)
{
   tex.bind();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.setMinFilt(GL_NEAREST_MIPMAP_NEAREST);
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.bind();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.setMagFilt(GL_NEAREST);
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.bind();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.setWrapX(GL_MIRRORED_REPEAT);
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.bind();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.setWrapY(GL_MIRRORED_REPEAT);
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   tex.bind();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
}