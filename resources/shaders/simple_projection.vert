#version 330 core
layout (location = 0) in uint a_proj_view_id;
layout (location = 1) in vec3 a_pos;

layout (std140) uniform MatrixBlock {
    mat4 proj_views[9];
};

void main() {
    gl_Position = proj_views[a_proj_view_id] * vec4(a_pos, 1.0);
}
