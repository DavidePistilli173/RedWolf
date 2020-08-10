#include <RedWolf/gl/Buffer.hpp>
#include <RedWolf/gl/VAO.hpp>
#include <RedWolf/gl/error.hpp>
#include <glad/glad.h>
#include <gtest/gtest.h>

namespace gl_vao_test
{
   constexpr std::array<float, rw::gl::vao_sizes[0]> vbo0{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[1]> vbo1{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[2]> vbo2{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[3]> vbo3{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[4]> vbo4{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[5]> vbo5{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[6]> vbo6{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[7]> vbo7{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[8]> vbo8{ 1.0f };
   constexpr std::array<float, rw::gl::vao_sizes[9]> vbo9{ 1.0f };

   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[0]>::ebo_size> ebo0{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[1]>::ebo_size> ebo1{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[2]>::ebo_size> ebo2{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[3]>::ebo_size> ebo3{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[4]>::ebo_size> ebo4{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[5]>::ebo_size> ebo5{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[6]>::ebo_size> ebo6{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[7]>::ebo_size> ebo7{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[8]>::ebo_size> ebo8{ 1U };
   constexpr std::array<unsigned int, rw::gl::VAO<rw::gl::vao_sizes[9]>::ebo_size> ebo9{ 1U };

   template<size_t S>
   void testVAO(
      rw::gl::VAO<S>&                                           vao,
      const std::array<float, S>&                               vbo,
      const std::array<unsigned int, rw::gl::VAO<S>::ebo_size>& ebo)
   {
      vao.bind();
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      vao.setEBO(ebo);
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      vao.bind();
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      rw::gl::Buffer eboBuff(ebo, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
      vao.setEBO(std::move(eboBuff));
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      vao.bind();
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      vao.setVBO(vbo);
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      vao.bind();
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      rw::gl::Buffer vboBuff(vbo, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
      vao.setVBO(std::move(vboBuff));
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
      vao.bind();
      ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   }
} // namespace gl_vao_test

TEST(GLVAO, DefaultConstructor)
{
   using namespace gl_vao_test;

   rw::gl::VAO<rw::gl::vao_sizes[0]> vao0;
   testVAO<rw::gl::vao_sizes[0]>(vao0, vbo0, ebo0);

   rw::gl::VAO<rw::gl::vao_sizes[1]> vao1;
   testVAO<rw::gl::vao_sizes[1]>(vao1, vbo1, ebo1);

   rw::gl::VAO<rw::gl::vao_sizes[2]> vao2;
   testVAO<rw::gl::vao_sizes[2]>(vao2, vbo2, ebo2);

   rw::gl::VAO<rw::gl::vao_sizes[3]> vao3;
   testVAO<rw::gl::vao_sizes[3]>(vao3, vbo3, ebo3);

   rw::gl::VAO<rw::gl::vao_sizes[4]> vao4;
   testVAO<rw::gl::vao_sizes[4]>(vao4, vbo4, ebo4);

   rw::gl::VAO<rw::gl::vao_sizes[5]> vao5;
   testVAO<rw::gl::vao_sizes[5]>(vao5, vbo5, ebo5);

   rw::gl::VAO<rw::gl::vao_sizes[6]> vao6;
   testVAO<rw::gl::vao_sizes[6]>(vao6, vbo6, ebo6);

   rw::gl::VAO<rw::gl::vao_sizes[7]> vao7;
   testVAO<rw::gl::vao_sizes[7]>(vao7, vbo7, ebo7);

   rw::gl::VAO<rw::gl::vao_sizes[8]> vao8;
   testVAO<rw::gl::vao_sizes[8]>(vao8, vbo8, ebo8);

   rw::gl::VAO<rw::gl::vao_sizes[9]> vao9;
   testVAO<rw::gl::vao_sizes[9]>(vao9, vbo9, ebo9);

   // Move constructor and assignment operator.
   rw::gl::VAO<rw::gl::vao_sizes[5]> move_constructor_vao{ std::move(vao5) };
   testVAO(move_constructor_vao, vbo5, ebo5);
   rw::gl::VAO<rw::gl::vao_sizes[5]> move_op_vao = std::move(move_constructor_vao);
   testVAO(move_op_vao, vbo5, ebo5);
}

TEST(GLVAO, CustomUsage)
{
   using namespace gl_vao_test;

   // Static draw usage.
   rw::gl::VAO<rw::gl::vao_sizes[0]> static_vao0{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[0]>(static_vao0, vbo0, ebo0);

   rw::gl::VAO<rw::gl::vao_sizes[1]> static_vao1{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[1]>(static_vao1, vbo1, ebo1);

   rw::gl::VAO<rw::gl::vao_sizes[2]> static_vao2{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[2]>(static_vao2, vbo2, ebo2);

   rw::gl::VAO<rw::gl::vao_sizes[3]> static_vao3{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[3]>(static_vao3, vbo3, ebo3);

   rw::gl::VAO<rw::gl::vao_sizes[4]> static_vao4{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[4]>(static_vao4, vbo4, ebo4);

   rw::gl::VAO<rw::gl::vao_sizes[5]> static_vao5{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[5]>(static_vao5, vbo5, ebo5);

   rw::gl::VAO<rw::gl::vao_sizes[6]> static_vao6{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[6]>(static_vao6, vbo6, ebo6);

   rw::gl::VAO<rw::gl::vao_sizes[7]> static_vao7{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[7]>(static_vao7, vbo7, ebo7);

   rw::gl::VAO<rw::gl::vao_sizes[8]> static_vao8{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[8]>(static_vao8, vbo8, ebo8);

   rw::gl::VAO<rw::gl::vao_sizes[9]> static_vao9{ GL_STATIC_DRAW };
   testVAO<rw::gl::vao_sizes[9]>(static_vao9, vbo9, ebo9);

   // Move constructor and assignment operator.
   rw::gl::VAO<rw::gl::vao_sizes[5]> static_move_constructor_vao{ std::move(static_vao5) };
   testVAO(static_move_constructor_vao, vbo5, ebo5);
   rw::gl::VAO<rw::gl::vao_sizes[5]> static_move_op_vao = std::move(static_move_constructor_vao);
   testVAO(static_move_op_vao, vbo5, ebo5);

   // Dynamic draw usage.
   rw::gl::VAO<rw::gl::vao_sizes[0]> dynamic_vao0{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[0]>(dynamic_vao0, vbo0, ebo0);

   rw::gl::VAO<rw::gl::vao_sizes[1]> dynamic_vao1{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[1]>(dynamic_vao1, vbo1, ebo1);

   rw::gl::VAO<rw::gl::vao_sizes[2]> dynamic_vao2{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[2]>(dynamic_vao2, vbo2, ebo2);

   rw::gl::VAO<rw::gl::vao_sizes[3]> dynamic_vao3{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[3]>(dynamic_vao3, vbo3, ebo3);

   rw::gl::VAO<rw::gl::vao_sizes[4]> dynamic_vao4{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[4]>(dynamic_vao4, vbo4, ebo4);

   rw::gl::VAO<rw::gl::vao_sizes[5]> dynamic_vao5{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[5]>(dynamic_vao5, vbo5, ebo5);

   rw::gl::VAO<rw::gl::vao_sizes[6]> dynamic_vao6{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[6]>(dynamic_vao6, vbo6, ebo6);

   rw::gl::VAO<rw::gl::vao_sizes[7]> dynamic_vao7{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[7]>(dynamic_vao7, vbo7, ebo7);

   rw::gl::VAO<rw::gl::vao_sizes[8]> dynamic_vao8{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[8]>(dynamic_vao8, vbo8, ebo8);

   rw::gl::VAO<rw::gl::vao_sizes[9]> dynamic_vao9{ GL_DYNAMIC_DRAW };
   testVAO<rw::gl::vao_sizes[9]>(dynamic_vao9, vbo9, ebo9);

   // Move constructor and assignment operator.
   rw::gl::VAO<rw::gl::vao_sizes[5]> dynamic_move_constructor_vao{ std::move(dynamic_vao5) };
   testVAO(dynamic_move_constructor_vao, vbo5, ebo5);
   rw::gl::VAO<rw::gl::vao_sizes[5]> dynamic_move_op_vao = std::move(dynamic_move_constructor_vao);
   testVAO(dynamic_move_op_vao, vbo5, ebo5);

   // Stream draw usage.
   rw::gl::VAO<rw::gl::vao_sizes[0]> stream_vao0{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[0]>(stream_vao0, vbo0, ebo0);

   rw::gl::VAO<rw::gl::vao_sizes[1]> stream_vao1{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[1]>(stream_vao1, vbo1, ebo1);

   rw::gl::VAO<rw::gl::vao_sizes[2]> stream_vao2{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[2]>(stream_vao2, vbo2, ebo2);

   rw::gl::VAO<rw::gl::vao_sizes[3]> stream_vao3{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[3]>(stream_vao3, vbo3, ebo3);

   rw::gl::VAO<rw::gl::vao_sizes[4]> stream_vao4{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[4]>(stream_vao4, vbo4, ebo4);

   rw::gl::VAO<rw::gl::vao_sizes[5]> stream_vao5{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[5]>(stream_vao5, vbo5, ebo5);

   rw::gl::VAO<rw::gl::vao_sizes[6]> stream_vao6{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[6]>(stream_vao6, vbo6, ebo6);

   rw::gl::VAO<rw::gl::vao_sizes[7]> stream_vao7{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[7]>(stream_vao7, vbo7, ebo7);

   rw::gl::VAO<rw::gl::vao_sizes[8]> stream_vao8{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[8]>(stream_vao8, vbo8, ebo8);

   rw::gl::VAO<rw::gl::vao_sizes[9]> stream_vao9{ GL_STREAM_DRAW };
   testVAO<rw::gl::vao_sizes[9]>(stream_vao9, vbo9, ebo9);

   // Move constructor and assignment operator.
   rw::gl::VAO<rw::gl::vao_sizes[5]> stream_move_constructor_vao{ std::move(stream_vao5) };
   testVAO(stream_move_constructor_vao, vbo5, ebo5);
   rw::gl::VAO<rw::gl::vao_sizes[5]> stream_move_op_vao = std::move(stream_move_constructor_vao);
   testVAO(stream_move_op_vao, vbo5, ebo5);
}