#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

// incoming light matrix tranforms (projection * view)
uniform mat4 u_light_matrix[6];
// output to fragment shader
out vec4 v_frag_pos;


void main()
{
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; i++)
		{
			v_frag_pos = gl_in[i].gl_Position;
			gl_Position = u_light_matrix[face] * v_frag_pos;
			EmitVertex();
		}
		EndPrimitive();
	}
}