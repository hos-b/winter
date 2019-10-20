#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec4 v_col;
out vec2 v_tex_coord;
out vec3 v_normal;
out vec3 v_frag_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * position;
    v_col = clamp(position, 0.0f, 1.0f);
    v_tex_coord = tex_coord;
    v_normal = mat3(transpose(inverse(u_model))) * normal;
	v_frag_position = (u_model * position).xyz;	// swizzling vec4 to vec3
};