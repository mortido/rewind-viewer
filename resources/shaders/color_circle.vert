#version 330 core
layout (location = 0) in uint a_proj_idx;
layout (location = 1) in uint a_color;
layout (location = 2) in vec2 a_pos;
layout (location = 3) in float a_radius;
layout (location = 4) in float a_start_angle;
layout (location = 5) in float a_end_angle;

out VS_OUT {
    uint proj_idx;
    vec4 color;
    float radius;
    float start_angle;
    float end_angle;
} vs_out;

vec4 unpack_color(uint color) {
    return vec4(
        float((color >> 16U) & 0xFFU) / 255.0, // Extract R component and normalize
        float((color >> 8U) & 0xFFU) / 255.0,  // Extract G component and normalize
        float(color & 0xFFU) / 255.0,          // Extract B component and normalize
        float((color >> 24U) & 0xFFU) / 255.0  // Extract A component and normalize
    );
}

void main() {
    gl_Position = vec4(a_pos, 0.2, 1.0);
    vs_out.proj_idx = a_proj_idx;
    vs_out.color = unpack_color(a_color);
    vs_out.radius = a_radius;
    vs_out.start_angle = a_start_angle;
    vs_out.end_angle = a_end_angle;
}