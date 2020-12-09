#include <RedWolf/gl/Buffer.hpp>
#include <RedWolf/gl/VAO.hpp>
#include <RedWolf/gl/error.hpp>
#include <glad/glad.h>
#include <gtest/gtest.h>

namespace gl_vao_test
{
   template<size_t VBOSize, size_t EBOSize>
   void testVAO(
      rw::gl::VAO<VBOSize, EBOSize>&           vao,
      const std::array<float, VBOSize>&        vbo,
      const std::array<unsigned int, EBOSize>& ebo)
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

   template<size_t A, size_t B>
   struct TypeDefinitions
   {
      static constexpr size_t VBOSize{ A };
      static constexpr size_t EBOSize{ B };
   };

   using VAOTypes = ::testing::Types<
      TypeDefinitions<rw::gl::vao_sizes[0].first, rw::gl::vao_sizes[0].second>,
      TypeDefinitions<rw::gl::vao_sizes[1].first, rw::gl::vao_sizes[1].second>,
      TypeDefinitions<rw::gl::vao_sizes[2].first, rw::gl::vao_sizes[2].second>,
      TypeDefinitions<rw::gl::vao_sizes[3].first, rw::gl::vao_sizes[3].second>,
      TypeDefinitions<rw::gl::vao_sizes[4].first, rw::gl::vao_sizes[4].second>,
      TypeDefinitions<rw::gl::vao_sizes[5].first, rw::gl::vao_sizes[5].second>,
      TypeDefinitions<rw::gl::vao_sizes[6].first, rw::gl::vao_sizes[6].second>,
      TypeDefinitions<rw::gl::vao_sizes[7].first, rw::gl::vao_sizes[7].second>,
      TypeDefinitions<rw::gl::vao_sizes[8].first, rw::gl::vao_sizes[8].second>,
      TypeDefinitions<rw::gl::vao_sizes[9].first, rw::gl::vao_sizes[9].second>,
      TypeDefinitions<rw::gl::vao_sizes[10].first, rw::gl::vao_sizes[10].second>,
      TypeDefinitions<rw::gl::vao_sizes[11].first, rw::gl::vao_sizes[11].second>,
      TypeDefinitions<rw::gl::vao_sizes[12].first, rw::gl::vao_sizes[12].second>,
      TypeDefinitions<rw::gl::vao_sizes[13].first, rw::gl::vao_sizes[13].second>,
      TypeDefinitions<rw::gl::vao_sizes[14].first, rw::gl::vao_sizes[14].second>,
      TypeDefinitions<rw::gl::vao_sizes[15].first, rw::gl::vao_sizes[15].second>,
      TypeDefinitions<rw::gl::vao_sizes[16].first, rw::gl::vao_sizes[16].second>,
      TypeDefinitions<rw::gl::vao_sizes[17].first, rw::gl::vao_sizes[17].second>,
      TypeDefinitions<rw::gl::vao_sizes[18].first, rw::gl::vao_sizes[18].second>>;
} // namespace gl_vao_test

template<typename T>
class GLVAO : public ::testing::Test
{
};
TYPED_TEST_CASE_P(GLVAO);

TYPED_TEST_P(GLVAO, DefaultConstructor)
{
   using namespace gl_vao_test;

   std::array<float, TypeParam::VBOSize>               vbo{ 1.0F };
   std::array<unsigned int, TypeParam::EBOSize>        ebo{ 1U };
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> vao;
   testVAO(vao, vbo, ebo);

   // Move constructor and assignment operator.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> move_constructor_vao{ std::move(vao) };
   testVAO(move_constructor_vao, vbo, ebo);
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> move_op_vao = std::move(move_constructor_vao);
   testVAO(move_op_vao, vbo, ebo);
}

TYPED_TEST_P(GLVAO, CustomUsage)
{
   using namespace gl_vao_test;

   std::array<float, TypeParam::VBOSize>        vbo{ 1.0F };
   std::array<unsigned int, TypeParam::EBOSize> ebo{ 1U };

   // Static draw usage.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> static_vao{ GL_STATIC_DRAW };
   testVAO(static_vao, vbo, ebo);

   // Move constructor and assignment operator.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> static_move_constructor_vao{ std::move(static_vao) };
   testVAO(static_move_constructor_vao, vbo, ebo);
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> static_move_op_vao = std::move(static_move_constructor_vao);
   testVAO(static_move_op_vao, vbo, ebo);

   // Dynamic draw usage.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> dynamic_vao{ GL_DYNAMIC_DRAW };
   testVAO(dynamic_vao, vbo, ebo);

   // Move constructor and assignment operator.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> dynamic_move_constructor_vao{ std::move(dynamic_vao) };
   testVAO(dynamic_move_constructor_vao, vbo, ebo);
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> dynamic_move_op_vao = std::move(dynamic_move_constructor_vao);
   testVAO(dynamic_move_op_vao, vbo, ebo);

   // Stream draw usage.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> stream_vao{ GL_STREAM_DRAW };
   testVAO(stream_vao, vbo, ebo);

   // Move constructor and assignment operator.
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> stream_move_constructor_vao{ std::move(stream_vao) };
   testVAO(stream_move_constructor_vao, vbo, ebo);
   rw::gl::VAO<TypeParam::VBOSize, TypeParam::EBOSize> stream_move_op_vao = std::move(stream_move_constructor_vao);
   testVAO(stream_move_op_vao, vbo, ebo);
}

REGISTER_TYPED_TEST_CASE_P(GLVAO, DefaultConstructor, CustomUsage);

INSTANTIATE_TYPED_TEST_CASE_P(GLVAOTests, GLVAO, gl_vao_test::VAOTypes);