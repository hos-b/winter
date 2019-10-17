#shader vertex
#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec4 v_col;
out vec2 v_tex_coord;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * position;
    v_col = clamp(position, 0.0f, 1.0f);
    v_tex_coord = tex_coord;
    v_normal = mat3(transpose(inverse(u_model))) * normal;
};

#shader fragment
#version 330 core

out vec4 color;
in vec2 v_tex_coord; 
in vec4 v_col;
in vec3 v_normal;

struct DirectionalLight{
    vec3 color;
    float ambient_intensity;
    vec3 direction;
    float diffuse_intensity;
};

uniform sampler2D u_texture; 
uniform DirectionalLight u_directional_light;
void main()
{
    vec4 ambient_color = vec4(u_directional_light.color, 1.0f) * u_directional_light.ambient_intensity;
    ambient_color.w = 0.5f;
    float diffuse_factor = max(dot(normalize(v_normal), u_directional_light.direction), 0);
    vec4 diffuse_color = vec4(u_directional_light.color, 1.0f) * u_directional_light.diffuse_intensity * diffuse_factor;
    diffuse_color.w = 0.5f;
    color  = texture(u_texture, v_tex_coord) * (ambient_color + diffuse_color); // * v_col 
};