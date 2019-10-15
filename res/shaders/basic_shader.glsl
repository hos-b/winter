#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

out vec4 v_col;
out vec2 v_tex_coord;

uniform mat4 u_model_view_projection;

void main()
{
    gl_Position = u_model_view_projection * position;
    v_col = clamp(position, 0.0f, 1.0f);
    v_tex_coord = tex_coord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_tex_coord;
in vec4 v_col;

uniform sampler2D u_texture;

void main()
{
    vec4 tex_color = texture(u_texture, v_tex_coord) * v_col;
    color = tex_color;
};