#include "utility.hpp"

#include <RedWolf/io/Text.hpp>
#include <algorithm>
#include <gtest/gtest.h>
#include <string>
#include <string_view>

// File names.
constexpr std::string_view empty_text_file{ "emptyText.txt" };
constexpr std::string_view average_text_file{ "averageText.txt" };

// File contents.
const std::string empty_text_content{ '\0' };
const std::string average_text_content{
   R"delimiter(#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
    ivec2 baseTexSize = textureSize(baseTexture, 0);
    ivec2 pixelPos = ivec2(texturePosOutput * baseTexSize);
    if (pixelPos.x <= 20 || pixelPos.x >= baseTexSize.x - 20 ||
        pixelPos.y <= 20 || pixelPos.y >= baseTexSize.y - 20)
    {
        fragmentColour.x = fragmentColour.x * 0.3 + 1.0 * 0.7;
        fragmentColour.y = fragmentColour.y * 0.3 + 0.8 * 0.7;
        fragmentColour.z = fragmentColour.z * 0.3 + 0.0 * 0.7;   
    }
}

#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
}

#version 430

layout (location = 0) in vec3 screenPos;
layout (location = 1) in vec2 texturePos;

out vec2 texturePosOutput;

uniform mat4 cameraMatrix;

void main()
{
    gl_Position = cameraMatrix * vec4(screenPos, 1.0);
    texturePosOutput = texturePos;
}

#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
    ivec2 baseTexSize = textureSize(baseTexture, 0);
    ivec2 pixelPos = ivec2(texturePosOutput * baseTexSize);
    if (pixelPos.x <= 20 || pixelPos.x >= baseTexSize.x - 20 ||
        pixelPos.y <= 20 || pixelPos.y >= baseTexSize.y - 20)
    {
        fragmentColour.x = fragmentColour.x * 0.3 + 1.0 * 0.7;
        fragmentColour.y = fragmentColour.y * 0.3 + 0.8 * 0.7;
        fragmentColour.z = fragmentColour.z * 0.3 + 0.0 * 0.7;   
    }
}

#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
}

#version 430

layout (location = 0) in vec3 screenPos;
layout (location = 1) in vec2 texturePos;

out vec2 texturePosOutput;

uniform mat4 cameraMatrix;

void main()
{
    gl_Position = cameraMatrix * vec4(screenPos, 1.0);
    texturePosOutput = texturePos;
}

#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
    ivec2 baseTexSize = textureSize(baseTexture, 0);
    ivec2 pixelPos = ivec2(texturePosOutput * baseTexSize);
    if (pixelPos.x <= 20 || pixelPos.x >= baseTexSize.x - 20 ||
        pixelPos.y <= 20 || pixelPos.y >= baseTexSize.y - 20)
    {
        fragmentColour.x = fragmentColour.x * 0.3 + 1.0 * 0.7;
        fragmentColour.y = fragmentColour.y * 0.3 + 0.8 * 0.7;
        fragmentColour.z = fragmentColour.z * 0.3 + 0.0 * 0.7;   
    }
}

#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
}

#version 430

layout (location = 0) in vec3 screenPos;
layout (location = 1) in vec2 texturePos;

out vec2 texturePosOutput;

uniform mat4 cameraMatrix;

void main()
{
    gl_Position = cameraMatrix * vec4(screenPos, 1.0);
    texturePosOutput = texturePos;
})delimiter"
};

// Load empty textfile.
TEST(IOText, LoadEmpty)
{
   std::string file{ rwt::data_folder };
   file += empty_text_file;

   std::string contents{ rw::io::Text::load(file) };
   ASSERT_EQ(contents, empty_text_content);
}

// Load an average text file.
TEST(IOText, LoadAverage)
{
   std::string file{ rwt::data_folder };
   file += average_text_file;

   std::string contents{ rw::io::Text::load(file) };
   contents.erase(std::remove(contents.begin(), contents.end(), '\r'), contents.end());
   ASSERT_EQ(contents, average_text_content);
}