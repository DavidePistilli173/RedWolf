#include "utility.hpp"

#include <RedWolf/gl/Shader.hpp>
#include <RedWolf/gl/error.hpp>
#include <glm0.9.9.7/glm.hpp>
#include <glm0.9.9.7/gtc/matrix_transform.hpp>
#include <glm0.9.9.7/gtc/type_ptr.hpp>
#include <gtest/gtest.h>
#include <string_view>

constexpr std::string_view non_existing_shader{ "nonexisting.glsl" };
constexpr std::string_view vertex_shader_1{ "plainVertex.glsl" };
constexpr std::string_view fragment_shader_1{ "plainFragment.glsl" };
constexpr std::string_view fragment_shader_2{ "highlightFragment.glsl" };

TEST(GLShader, Default)
{
   rw::gl::Shader shader;
   shader.use();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   EXPECT_THROW(shader.setUniform(0, 0), std::exception);
}

TEST(GLShader, FailLoading)
{
   EXPECT_THROW(rw::gl::Shader(std::make_pair(non_existing_shader, non_existing_shader)), std::exception);
   EXPECT_THROW(rw::gl::Shader(std::make_pair(vertex_shader_1, non_existing_shader)), std::exception);
   EXPECT_THROW(rw::gl::Shader(std::make_pair(non_existing_shader, fragment_shader_1)), std::exception);
}

TEST(GLShader, ExampleShaders)
{
   std::string pathV1{ rwt::data_folder };
   pathV1 += vertex_shader_1;
   std::string pathF1{ rwt::data_folder };
   pathF1 += fragment_shader_1;
   rw::gl::Shader shader1{ std::make_pair(pathV1, pathF1) };
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   shader1.use();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);

   glm::mat4 mat = glm::mat4(1.0f);
   shader1.setUniform(0, glm::value_ptr(mat));
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   shader1.setUniform(1, 1);
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);

   std::string pathF2{ rwt::data_folder };
   pathF2 += fragment_shader_2;
   rw::gl::Shader shader2{ std::make_pair(pathV1, pathF2) };
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   shader2.use();
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);

   shader2.setUniform(0, glm::value_ptr(mat));
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
   shader2.setUniform(1, 1);
   ASSERT_EQ(rw::gl::Messages::checkOut(rw::gl::ErrLvl::error).size(), 0);
}