#version 330

in vec4 v_frag_pos;

uniform vec3 u_light_pos;
uniform float u_far_plane;


void main()
{
	float distance = length(v_frag_pos.xyz - u_light_pos);
	// from [near, far] to [0, 1]
	distance = distance/u_far_plane;
	gl_FragDepth = distance;
}