#shader vertex
#version 330
layout (location = 0) in vec3 position;

out vec3 v_tex_coords;
uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
	v_tex_coords = position;
	gl_Position = u_projection * u_view * vec4(position, 1.0);
}
#shader fragment
#version 330
in vec3 v_tex_coords;
out vec4 color;
uniform samplerCube u_skybox;
void main()
{
	color = texture(u_skybox, v_tex_coords);
}