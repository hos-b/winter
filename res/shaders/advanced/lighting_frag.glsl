#version 330 core

out vec4 color;

in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_frag_position;

const int MAX_PLIGHTS = 3;

struct Light{
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct DirectionalLight{
    Light base;
    vec3 direction;
};

struct PointLight{
	Light base;
	vec3 position;
	float exponent;
	float linear;
	float constant;
};

struct Material{
	float specular_intensity;
	float shininess;
};

uniform sampler2D u_texture; 
uniform vec3 u_camera;
uniform int u_plight_count;
uniform DirectionalLight u_directional_light;
uniform PointLight u_plight[MAX_PLIGHTS];
uniform Material u_material;

vec4 light_by_direction(Light light, vec3 direction)
{
	vec4 ambient_color = vec4(light.color, 1.0f) * light.ambient_intensity;
    float diffuse_factor = max(dot(normalize(v_normal), normalize(direction)), 0.0f);
    vec4 diffuse_color = vec4(light.color * light.diffuse_intensity * diffuse_factor, 1.0f) ;
	vec4 specular_color = vec4(0, 0, 0, 0);
	// if no diffuse lighting, no need for specular
	if (diffuse_factor > 0.0f)
	{
		vec3 fragment_eye = normalize(u_camera - v_frag_position);
		vec3 reflected_ray = normalize(reflect(direction, normalize(v_normal)));
		float specular_factor = dot(fragment_eye, reflected_ray);
		if (specular_factor > 0)
		{
			specular_factor = pow(specular_factor, u_material.shininess);
			specular_color = vec4(light.color * u_material.specular_intensity * specular_factor, 1.0f);
		}
	}
    return (ambient_color + diffuse_color + specular_color);
}
vec4 evaluate_point_lights()
{
	vec4 sum_color = vec4(0,0,0,0);
	for (int i = 0; i < u_plight_count; i++)
	{
		vec3 direction = v_frag_position - u_plight[i].position;
		float distance = length(direction);
		direction = normalize(direction);
		vec4 col = light_by_direction(u_plight[i].base, direction);
		float attenuation  = u_plight[i].exponent * distance * distance + u_plight[i].linear * distance + u_plight[i].constant;
		sum_color += (col/attenuation);
	}
	return sum_color;
}
void main()
{
    vec4 final = light_by_direction(u_directional_light.base, u_directional_light.direction);
	final += evaluate_point_lights();
    color  = texture(u_texture, v_tex_coord) * final; // * v_col 
};