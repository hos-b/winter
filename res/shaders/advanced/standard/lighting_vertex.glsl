#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec2 v_tex_coord;
// lighting
out vec3 v_normal;
out vec3 v_frag_position;	// global position
// mvp
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
// shadows
out vec4 v_dlight_space_pos ;		// position w.r.t. light
uniform mat4 u_dlight_transform;	// projection x view
void main()
{
	// basic interpolation
    gl_Position = u_projection * u_view * u_model * position;
	// textures
    v_tex_coord = tex_coord;
	// lihgting
    v_normal = mat3(transpose(inverse(u_model))) * normal;
	v_frag_position = (u_model * position).xyz;	// swizzling vec4 to vec3
	// shadows
	v_dlight_space_pos = u_dlight_transform * u_model * position;
};