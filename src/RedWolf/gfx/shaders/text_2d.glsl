// Basic text shader.

#type vertex
#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_tex_coord;

uniform mat4 u_view_projection;

out vec2 v_tex_coord;
out vec4 v_color;
flat out float v_tex_index;

void main() {
    v_tex_coord = in_tex_coord;
    v_color = in_color;
    gl_Position = u_view_projection * vec4(in_position, 1.0);
}

#type fragment
#version 460 core

in vec2 v_tex_coord;
in vec4 v_color;// Text color.

uniform sampler2D u_font_atlas;

layout(location = 0) out vec4 color;

float screen_px_range() {
    const float px_range = 2.0;// Set to distance field's pixel range.
    vec2 unit_range = vec2(px_range) / vec2(textureSize(u_font_atlas, 0));
    vec2 screen_tex_size = vec2(1.0) / fwidth(v_tex_coord);
    return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 msd = texture(u_font_atlas, v_tex_coord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screen_px_distance = screen_px_range() * (sd - 0.5);
    float opacity = clamp(screen_px_distance + 0.5, 0.0, 1.0);
    if (opacity == 0.0) {
        discard;
    }

    vec4 bg_color = vec4(0.0);
    color = mix(bg_color, v_color, opacity);
    if (color.a == 0.0) {
        discard;
    }
}