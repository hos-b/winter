#shader vertex
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

#shader fragment
#version 330 core

out vec4 color;
in vec2 v_tex_coord; 
in vec4 v_col;
in vec3 v_normal;
in vec3 v_frag_position;

struct DirectionalLight{
    vec3 color;
    float ambient_intensity;
    vec3 direction;
    float diffuse_intensity;
};
struct Material{
	float specular_intensity;
	float shininess;
};

uniform sampler2D u_texture; 
uniform vec3 u_camera;
uniform DirectionalLight u_directional_light;
uniform Material u_material;
void main()
{
    vec4 ambient_color = vec4(u_directional_light.color, 1.0f) * u_directional_light.ambient_intensity;
    float diffuse_factor = max(dot(normalize(v_normal), u_directional_light.direction), 0);
    vec4 diffuse_color = vec4(u_directional_light.color, 1.0f) * u_directional_light.diffuse_intensity * diffuse_factor;
	vec4 specular_color = vec4(0, 0, 0, 0);
	// if no diffuse lighting, no need for specular
	if (diffuse_factor > 0.0f)
	{
		vec3 fragment_eye = normalize(u_camera - v_frag_position);
		vec3 reflected_ray = normalize(reflect(u_directional_light.direction, normalize(v_normal)));
		float specular_factor = dot(fragment_eye, reflected_ray);
		if (specular_factor > 0)
		{
			specular_factor = pow(specular_factor, u_material.shininess);
			specular_color = vec4(u_directional_light.color * u_material.specular_intensity * specular_factor, 1.0f);
		}
	}
	vec3 lighting_sum = (ambient_color + diffuse_color + specular_color).xyz;
    color  = texture(u_texture, v_tex_coord) * vec4(lighting_sum, 1.0f); // * v_col 
};