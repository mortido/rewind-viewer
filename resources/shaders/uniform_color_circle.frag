#version 330 core
in GS_OUT {
    vec2 current_point;
    vec2 start_point;
    vec2 mid_point;
    vec2 end_point;
    float radius;
    float start_angle;
    float end_angle;
} fs_in;

out vec4 frag_color;

//Zero line width mean filled circle
uniform uint line_width;
uniform vec4 color = vec4(1.0);
uniform bool is_segment = false;

void main() {
    // Circle check
    float dist = length(fs_in.current_point);
    float inner_radius = 0.0 + step(float(1), float(line_width)) * (fs_in.radius - fwidth(dist) * line_width);
    float inside = step(dist, fs_in.radius);
    float outside = step(inner_radius, dist);

    // Arc check
    float determinant_mid = (fs_in.end_point.x - fs_in.start_point.x) * (fs_in.mid_point.y - fs_in.start_point.y) - (fs_in.end_point.y - fs_in.start_point.y) * (fs_in.mid_point.x - fs_in.start_point.x);
    float determinant_pos = (fs_in.end_point.x - fs_in.start_point.x) * (fs_in.current_point.y - fs_in.start_point.y) - (fs_in.end_point.y - fs_in.start_point.y) * (fs_in.current_point.x - fs_in.start_point.x);
    float same_side = step(0.0, determinant_mid * determinant_pos);

    // Sector check
    float angle = atan(fs_in.current_point.y, fs_in.current_point.x);
    float inverted_case = step(fs_in.end_angle, fs_in.start_angle);// Also checks for start_angle == end_angle
    float after_start = step(fs_in.start_angle, angle);
    float before_end = step(angle, fs_in.end_angle);
    float valid_angle = (1.0 - inverted_case) * after_start * before_end;
    valid_angle += inverted_case * clamp(before_end + after_start, 0.0, 1.0);

    float valid = inside  * (float(is_segment) * valid_angle + (1.0 - float(is_segment)) * same_side) * outside;
    if (valid < 0.1) {
        discard;  // Requirerd for stencil check to work
    } else {
        frag_color = color;
    }
}
