#version 330 core
layout (location = 0) in uint a_proj_idx;
layout (location = 1) in vec2 a_pos;
layout (location = 2) in float a_radius;
layout (location = 3) in float a_start_angle;
layout (location = 4) in float a_end_angle;

out VS_OUT {
    uint proj_idx;
    float radius;
    float start_angle;
    float end_angle;
} vs_out;

void main() {
    gl_Position = vec4(a_pos, 0.2, 1.0);
    vs_out.proj_idx = a_proj_idx;
    vs_out.radius = a_radius;
    vs_out.start_angle = a_start_angle;
    vs_out.end_angle = a_end_angle;
}