#shader vertex
#version 330 core
layout(location = 0) in vec4 position;

uniform mat4 u_model_view_projection;
out vec4 v_col;
void main()
{
    gl_Position = u_model_view_projection * position;
    // v_col.x = distance(gl_Position, vec4(1.0f,-1.0f, 0.0f, 1.0f))-1.0f;
    // v_col.y = distance(gl_Position, vec4(1.0f, 1.0f, 0.0f, 1.0f))-1.0f;
    // v_col.z = distance(gl_Position, vec4(-1.0f,1.0f, 0.0f, 1.0f))-1.0f;
    // v_col.w = 1.0f;
    v_col = clamp(position, 0.0f, 1.0f);
};

#shader fragment
#version 330 core

out vec4 color;
in vec4 v_col;

void main()
{
    color = v_col;
};