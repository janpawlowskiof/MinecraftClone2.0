#version 450 core
out vec4 frag;

in vec2 tex_coords;
in vec3 normal;
in vec3 frag_pos;
in vec4 light_space_frag;

uniform sampler2D texture_terrain;
uniform sampler2D shadow_map;

uniform vec3 view_pos;

//uniform vec3 light_dir;
//uniform vec3 light_color;

float CalculateShadow()
{
	vec3 projection_coords = light_space_frag.xyz/light_space_frag.w;
	projection_coords = projection_coords * 0.5 + 0.5;
	float closestDepth = texture(shadow_map, projection_coords.xy).r;
	float currentDepth = projection_coords.z;

	if(closestDepth + 0.001 < currentDepth)
		return 1.0f;
	else
		return 0.0f;
}

void main()
{
	vec3 light_dir = normalize(vec3(0.3, 0.9, 0.55));
	vec3 light_color = vec3(0.95, 0.9, 0.9);
	float ambient_strength = 0.5;
    vec3 ambient = ambient_strength * light_color;

	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	float specular_strength = 0.2;
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal); 
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 4);
	vec3 specular = specular_strength * spec * light_color;  

	vec4 color = texture(texture_terrain, tex_coords);
	if(color.a < 0.5) discard;
	float shadow = CalculateShadow();
    vec3 result = (ambient + (diffuse + specular) * (1.0f - shadow)) * color.rgb;
	frag = vec4(result, 1.0);
}
