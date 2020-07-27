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