// Basic texture shader.

#type vertex
#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_tex_coord;
layout(location = 3) in float in_tex_index;
layout(location = 4) in float in_tiling_factor;

uniform mat4 u_view_projection;

out vec2 v_tex_coord;
out vec4 v_color;
flat out float v_tex_index;
out float v_tiling_factor;

void main() {
    v_tex_coord = in_tex_coord;
    v_color = in_color;
    v_tex_index = in_tex_index;
    v_tiling_factor = in_tiling_factor;
    gl_Position = u_view_projection * vec4(in_position, 1.0);
}

#type fragment
#version 330 core

in vec2 v_tex_coord;
in vec4 v_color;
flat in float v_tex_index;
in float v_tiling_factor;

uniform sampler2D u_textures[32];

layout(location = 0) out vec4 color;

void main() {
    color = texture(u_textures[int(v_tex_index)], v_tex_coord * v_tiling_factor) * v_color;
}