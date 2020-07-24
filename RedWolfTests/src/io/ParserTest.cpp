#include <RedWolf/io/Parser.hpp>
#include <gtest/gtest.h>
#include <string_view>

constexpr std::string_view sample_text{
   R"delimiter(#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform float coeff;
uniform mat4 CM; uniform mat4 boh;
uniform 
sampler2D 
baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
})delimiter"
};

TEST(IOParser, OneFind)
{
   rw::io::Parser parser{ sample_text };

   size_t pos{ parser.find("uniform", 0) };
   ASSERT_EQ(sample_text.substr(pos, 20), "uniform float coeff;");

   // Repeat search.
   pos = parser.find("uniform", 0);
   ASSERT_EQ(sample_text.substr(pos, 20), "uniform float coeff;");
}

TEST(IOParser, FindAll)
{
   rw::io::Parser parser{ sample_text };

   auto positions = parser.findAll("uniform");
   ASSERT_EQ(positions.size(), 4);

   // Repeat search.
   positions = parser.findAll("uniform");
   ASSERT_EQ(positions.size(), 4);
}

TEST(IOParser, tokeniseLine)
{
   rw::io::Parser parser{ sample_text };

   size_t pos{ parser.find("uniform", 74) };
   ASSERT_EQ(sample_text.substr(pos, 16), "uniform mat4 CM;");

   auto tokens = parser.tokeniseLine(
      rw::io::Parser::glsl_ufrm_delims,
      rw::io::Parser::glsl_ufrm_endln,
      pos);
   ASSERT_EQ(tokens[0], "uniform");
   ASSERT_EQ(tokens[1], "mat4");
   ASSERT_EQ(tokens[2], "CM");
}

TEST(IOParser, tokeniseAll)
{
   rw::io::Parser parser{ sample_text };

   auto uniforms = parser.findAll("uniform");
   ASSERT_EQ(uniforms.size(), 4);

   auto currentUniform = uniforms.begin();

   auto tokens = parser.tokeniseLine(
      rw::io::Parser::glsl_ufrm_delims,
      rw::io::Parser::glsl_ufrm_endln,
      *currentUniform);
   ASSERT_EQ(tokens[0], "uniform");
   ASSERT_EQ(tokens[1], "float");
   ASSERT_EQ(tokens[2], "coeff");
   ++currentUniform;

   tokens = parser.tokeniseLine(
      rw::io::Parser::glsl_ufrm_delims,
      rw::io::Parser::glsl_ufrm_endln,
      *currentUniform);
   ASSERT_EQ(tokens[0], "uniform");
   ASSERT_EQ(tokens[1], "mat4");
   ASSERT_EQ(tokens[2], "CM");
   ++currentUniform;

   tokens = parser.tokeniseLine(
      rw::io::Parser::glsl_ufrm_delims,
      rw::io::Parser::glsl_ufrm_endln,
      *currentUniform);
   ASSERT_EQ(tokens[0], "uniform");
   ASSERT_EQ(tokens[1], "mat4");
   ASSERT_EQ(tokens[2], "boh");
   ++currentUniform;

   tokens = parser.tokeniseLine(
      rw::io::Parser::glsl_ufrm_delims,
      rw::io::Parser::glsl_ufrm_endln,
      *currentUniform);
   ASSERT_EQ(tokens[0], "uniform");
   ASSERT_EQ(tokens[1], "sampler2D");
   ASSERT_EQ(tokens[2], "baseTexture");
};