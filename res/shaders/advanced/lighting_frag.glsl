#version 330 core

out vec4 color;

in vec2 v_tex_coord;
in vec3 v_normal;
in vec3 v_frag_position;

const int MAX_PLIGHTS = 3;
const int MAX_SLIGHTS = 3;

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
struct SpotLight{
	PointLight base;
	vec3 direction;
	float coeff;
};

struct Material{
	float specular_intensity;
	float shininess;
};

uniform sampler2D u_texture; 
uniform vec3 u_camera;
uniform int u_plight_count;
uniform int u_slight_count;
uniform DirectionalLight u_directional_light;
uniform PointLight u_plight[MAX_PLIGHTS];
uniform SpotLight u_slight[MAX_SLIGHTS];
uniform Material u_material;

vec4 calc_light_by_direction(Light light, vec3 direction)
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
vec4 calc_point_light(PointLight plight)
{
	vec3 direction = v_frag_position - plight.position;
	float distance = length(direction);
	direction = normalize(direction);
	vec4 col = calc_light_by_direction(plight.base, direction);
	float attenuation  = plight.exponent * distance * distance + plight.linear * distance + plight.constant;
	return (col/attenuation);
}
vec4 evaluate_point_lights()
{
	vec4 sum_color = vec4(0,0,0,0);
	for (int i = 0; i < u_plight_count; i++)
	{
		sum_color += calc_point_light(u_plight[i]);
	}
	return sum_color;
}
vec4 calc_spot_light(SpotLight slight)
{
	vec3 ray_direction = normalize(v_frag_position - slight.base.position);
	float factor = dot(ray_direction, slight.direction);
	if (factor > slight.coeff)
	{
		vec4 col = calc_point_light(slight.base);
		return col * (1.0f - (1.0f - factor) * (1.0f/(1.0f-slight.coeff)));
	}
	return vec4(0 ,0 ,0 ,0);
}
vec4 evaluate_spot_lights()
{
	vec4 sum_color = vec4(0,0,0,0);
	for (int i = 0; i < u_slight_count; i++)
	{
		sum_color += calc_spot_light(u_slight[i]);
	}
	return sum_color;
}
void main()
{
    vec4 final = calc_light_by_direction(u_directional_light.base, u_directional_light.direction);
	final += evaluate_point_lights();
	final += evaluate_spot_lights();
    color  = texture(u_texture, v_tex_coord) * final; // * v_col 
};