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