#version 330 core
layout (location = 0) in uint a_proj_idx;
layout (location = 1) in uint a_color;
layout (location = 2) in vec2 a_pos;

out VS_OUT {
    vec4 color;
} vs_out;

layout (std140) uniform MatrixBlock {
    mat4 proj_views[9];
};

vec4 unpack_color(uint color) {
    return vec4(
        float((color >> 16U) & 0xFFU) / 255.0, // Extract R component and normalize
        float((color >> 8U) & 0xFFU) / 255.0,  // Extract G component and normalize
        float(color & 0xFFU) / 255.0,          // Extract B component and normalize
        float((color >> 24U) & 0xFFU) / 255.0  // Extract A component and normalize
    );
}


void main() {
    gl_Position = proj_views[a_proj_idx] * vec4(a_pos, 0.2, 1.0);
    vs_out.color = unpack_color(a_color);
}