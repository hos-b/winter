#shader vertex
#version 330

layout(location = 0) in vec4 position;

uniform mat4 u_model;
uniform mat4 u_dlight_transform; // projection x view
void main()
{
	gl_Position = u_dlight_transform * u_model * position;
};

#shader fragment
#version 330
void main(){}