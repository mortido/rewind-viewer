#version 330 core
layout (location = 0) in vec3 a_pos;

layout (std140) uniform MatrixBlock {
    mat4 proj_view;
};

void main() {
    gl_Position = proj_view * vec4(a_pos, 1.0);
}