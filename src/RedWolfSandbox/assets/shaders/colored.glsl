#type vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;

uniform mat4 u_view_projection;
uniform mat4 u_transform;

out vec3 v_position;
out vec4 v_color;

void main() {
    v_position = in_position;
    v_color = in_color;
    gl_Position = u_view_projection * u_transform * vec4(in_position, 1.0);
}

#type fragment
#version 330 core

in vec3 v_position;
in vec4 v_color;

uniform vec4 u_color;

layout(location = 0) out vec4 color;

void main() {
    color = u_color * v_color * 2;
}