#version 430

in vec2 texturePosOutput;
out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
}