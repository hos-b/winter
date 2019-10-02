#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

out vec2 v_texcoord;

void main()
{
    gl_Position = position;
    v_texcoord = tex_coord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texcoord;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
    vec4 tex_color = texture(u_texture, v_texcoord); 
    color = tex_color;
};