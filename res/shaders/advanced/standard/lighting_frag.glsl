#version 330 core

out vec4 color;
// textures
in vec2 v_tex_coord;
// lighting
in vec3 v_normal;
in vec3 v_frag_position;
const int MAX_PLIGHTS = 3;
const int MAX_SLIGHTS = 3;
// lighting structs
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
// lighting material
struct Material{
	float specular_intensity;
	float shininess;
};

uniform sampler2D u_texture; 	// basic texture
uniform vec3 u_camera;			// eye position
uniform int u_plight_count;		// # of active point lights
uniform int u_slight_count;		// # of active spot lights
// lighting struct uniforms
uniform DirectionalLight u_directional_light;
uniform PointLight u_plight[MAX_PLIGHTS];
uniform SpotLight u_slight[MAX_SLIGHTS];
uniform Material u_material;
// shadows
in vec4 v_dlight_space_pos;		// position w.r.t. directional light
uniform sampler2D u_directional_shadow_map;	// directional shadow map texture
uniform int u_pcf_radius;

float calc_directional_shadow_factor(DirectionalLight light)
{
	vec3 projected_coords = v_dlight_space_pos.xyz/v_dlight_space_pos.w;
	projected_coords = (projected_coords * 0.5f) + 0.5f;
	float closest_depth = texture(u_directional_shadow_map, projected_coords.xy).r;
	float current_depth = projected_coords.z;
	// adding bias
	vec3 normal_ = normalize(v_normal);
	vec3 light_dir_ = normalize(light.direction);
	float bias = max(0.05 * (1 - dot(normal_, light_dir_)), 0.005f);
	// pcf
	float shadow = 0.0f;
	vec2 texel_size = 1.0/textureSize(u_directional_shadow_map, 0);
	for(int x = -u_pcf_radius; x <= u_pcf_radius; ++x)
	{
		for(int y = -u_pcf_radius; y <= u_pcf_radius; ++y)
		{
			float pcf_depth = texture(u_directional_shadow_map, projected_coords.xy + vec2(x, y) * texel_size).r;
			shadow += (current_depth - bias) > pcf_depth ? 1.0f : 0.0f;
		}
	}
	shadow /= pow(2 * u_pcf_radius + 1, 2);
	if(projected_coords.z > 1.0f)
	{
		shadow = 0.0f;
	}
	return shadow;
}

vec4 calc_light_by_direction(Light light, vec3 direction, float shadow_factor)
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
    return (ambient_color + (1.0f - shadow_factor) * (diffuse_color + specular_color) );
}
vec4 calc_point_light(PointLight plight)
{
	vec3 direction = v_frag_position - plight.position;
	float distance = length(direction);
	direction = normalize(direction);
	vec4 col = calc_light_by_direction(plight.base, direction, 0.0f);
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
	float shadow_factor = calc_directional_shadow_factor(u_directional_light);
    vec4 final = calc_light_by_direction(u_directional_light.base, u_directional_light.direction, shadow_factor);
	final += evaluate_point_lights();
	final += evaluate_spot_lights();
    color  = texture(u_texture, v_tex_coord) * final; // * v_col 
};