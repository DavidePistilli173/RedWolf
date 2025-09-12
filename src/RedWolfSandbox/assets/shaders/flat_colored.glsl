// Basic texture shader.

#type vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_tex_coord;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

void main() {
    gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
}

#type fragment
#version 330 core

in vec2 v_tex_coord;

uniform vec4 u_color;

layout(location = 0) out vec4 color;

void main() {
    color = u_color;
}