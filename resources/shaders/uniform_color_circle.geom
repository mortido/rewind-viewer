#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    float radius;
    float start_angle;
    float end_angle;
} gs_in[];

out GS_OUT {
    vec2 current_point;
    vec2 start_point;
    vec2 mid_point;
    vec2 end_point;
    float radius;
    float start_angle;
    float end_angle;
} gs_out;

layout (std140) uniform MatrixBlock {
    mat4 proj_view;
};

void main() {
    float r = gs_in[0].radius;
    gs_out.start_angle = gs_in[0].start_angle;
    gs_out.end_angle = gs_in[0].end_angle;
    gs_out.radius = gs_in[0].radius;

    float normal_case = step(gs_in[0].start_angle, gs_in[0].end_angle);
    float mid_angle = 0.5 * (gs_in[0].start_angle + gs_in[0].end_angle) + (3.14159265) * (1.0 - normal_case);
    gs_out.start_point = r * vec2(cos(gs_in[0].start_angle), sin(gs_in[0].start_angle));
    gs_out.end_point = r * vec2(cos(gs_in[0].end_angle), sin(gs_in[0].end_angle));
    gs_out.mid_point = r * vec2(cos(mid_angle), sin(mid_angle));

    vec4 center_pos = gl_in[0].gl_Position;
    vec4 point;
    gs_out.current_point = vec2(-r, -r);
    point = center_pos + vec4(-r, -r, 0.0, 0.0);
    gl_Position = proj_view * point;
    EmitVertex();

    gs_out.current_point = vec2(r, -r);
    point = center_pos + vec4(r, -r, 0.0, 0.0);
    gl_Position = proj_view * point;
    EmitVertex();

    gs_out.current_point = vec2(-r, r);
    point = center_pos + vec4(-r, r, 0.0, 0.0);
    gl_Position = proj_view * point;
    EmitVertex();

    gs_out.current_point = vec2(r, r);
    point = center_pos + vec4(r, r, 0.0, 0.0);
    gl_Position = proj_view * point;
    EmitVertex();

    EndPrimitive();
}
