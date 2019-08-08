#version 450 core
out vec4 frag;

in vec2 tex_coords;
in vec3 normal;
in vec4 light_space_close_frag;

uniform sampler2D shadow_map_close;
uniform sampler2D shadow_map_far;

uniform vec3 view_pos;
uniform vec3 input_color;
uniform vec3 light_direction;

//uniform vec3 light_dir;
//uniform vec3 light_color;

float CalculateShadow()
{
	if(light_direction.y < 0)
		return 1;

	vec3 projection_coords = light_space_close_frag.xyz/light_space_close_frag.w;
	projection_coords = projection_coords * 0.5 + 0.5;
	//float closestDepth = texture(shadow_map, projection_coords.xy).r;
	float currentDepth = projection_coords.z;
	if(projection_coords.z >= 1.0 || texture(shadow_map_close, projection_coords.xy).r >= 1.0)
        return 0;

	float result = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadow_map_close, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadow_map_close, projection_coords.xy + vec2(x, y) * texelSize).r; 
			result += currentDepth - 0.0002 > pcfDepth ? 1.0 : 0.0;
		}    
	}
	result /= 9.0;
	return result;
}

void main()
{
	vec3 light_color = vec3(0.95, 0.9, 0.9);
	float ambient_strength = 0.5;
    vec3 ambient = ambient_strength * light_color;

	float diff = max(dot(normal, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	float shadow = CalculateShadow();
    vec3 result = (ambient + diffuse * (1.0f - shadow)) * input_color;
	frag = vec4(result, 1);
}
