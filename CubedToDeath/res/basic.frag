#version 450 core
out vec4 frag;

in Vertex
{
	vec2 texture_coords;
	vec4 light_space_close_frag;
	vec4 light_space_far_frag;
	float texture_color;
	float texture_normal;
	float texture_specular;
	float overlay_color;
	float overlay_normal;
	float overlay_specular;
	vec3 position;
	vec4 view_space;
	vec3 colorization;
	mat3 TBN;
}fragment;

uniform sampler2DArray texture_terrain;
uniform sampler2D shadow_map_close;
uniform sampler2D shadow_map_far;
uniform vec3 view_pos;
uniform vec3 light_direction;
uniform bool use_colorization = false;

//uniform vec3 light_dir;
//uniform vec3 light_color;

float CalculateFarShadow()
{
	vec3 projection_coords = fragment.light_space_far_frag.xyz/fragment.light_space_far_frag.w;
	projection_coords = projection_coords * 0.5 + 0.5;
	//float closestDepth = texture(shadow_map, projection_coords.xy).r;
	float currentDepth = projection_coords.z;
	if(projection_coords.z > 1.0)
        return 0.0;

	float result = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadow_map_far, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadow_map_far, projection_coords.xy + vec2(x, y) * texelSize).r; 
			result += currentDepth - 0.001 > pcfDepth ? 1.0 : 0.0;
		}    
	}
	result /= 9.0;
	return result;
}

float CalculateShadow()
{
	if(light_direction.y < 0)
	return 1;

	vec3 projection_coords = fragment.light_space_close_frag.xyz/fragment.light_space_close_frag.w;
	projection_coords = projection_coords * 0.5 + 0.5;
	//float closestDepth = texture(shadow_map, projection_coords.xy).r;
	float currentDepth = projection_coords.z;
	if(projection_coords.z >= 1.0 || texture(shadow_map_close, projection_coords.xy).r >= 1.0)
        return CalculateFarShadow();

	float result = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadow_map_close, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadow_map_close, projection_coords.xy + vec2(x, y) * texelSize).r; 
			result += currentDepth - 0.00055 > pcfDepth ? 1.0 : 0.0;
		}    
	}
	result /= 9.0;
	return result;
}

float zNear = 0.1;
float zFar = 1000.0;

// depthSample from depthTexture.r, for instance
float linearDepth(float depthSample)
{
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
    return zLinear;
}

void main()
{
	float fog_density = 0.0020;
	float dist = length(fragment.view_space);
	float fog_factor = 1.0 /exp(dist*dist * fog_density *fog_density );
	//float fog_factor = (240-dist)/(240-200);
    fog_factor = clamp( fog_factor, 0.0, 1.0 );
	vec3 light_color = 1.0 * vec3(0.95, 0.9, 0.9);
	float ambient_strength = 0.4;
    vec3 ambient = ambient_strength * light_color;

	vec4 color = vec4(0);
	vec3 normal = vec3(0,0,1);
	vec3 specular = vec3(0);
	vec4 overlay_color;
	
	if(fragment.overlay_color >= 0 && (overlay_color = texture(texture_terrain, vec3(fragment.texture_coords, fragment.overlay_color))).a > 0.5)
	{
		color = overlay_color;
		if(fragment.colorization.r >= 0)
			color *= vec4(fragment.colorization, 1);

		if(fragment.overlay_normal >= 0)
			normal = 2.0 * texture(texture_terrain, vec3(fragment.texture_coords, fragment.overlay_normal)).rgb - 1.0;
		if(fragment.overlay_specular >= 0)
			specular = texture(texture_terrain, vec3(fragment.texture_coords, fragment.overlay_specular)).rgb;
	}
	else
	{
		if(fragment.texture_color >= 0)
			color = texture(texture_terrain, vec3(fragment.texture_coords, fragment.texture_color));
		if(fragment.texture_normal >= 0)
			normal = 2.0 * texture(texture_terrain, vec3(fragment.texture_coords, fragment.texture_normal)).rgb - 1.0;
		if(fragment.texture_specular >= 0)
			specular = texture(texture_terrain, vec3(fragment.texture_coords, fragment.texture_specular)).rgb;
	}
	//color = vec4(0.5, 0.5, 0.5, 1);

	normal = fragment.TBN * normal;
	float diff = max(dot(normal, light_direction), 0.0);
	vec3 diffuse = diff * 1.0 * light_color;
	vec3 specular_value = vec3(0);
	//float specular_strength = 0.1;
	if(specular.r > 0)
	{
		vec3 view_dir = normalize(view_pos - fragment.position);
		vec3 reflect_dir = reflect(-light_direction, normal); 
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 4);
		specular_value +=  spec * light_color * vec3(specular.r);
	}
	//specular.g = 1;
	if(specular.g > 0)
	{
		//specular.g = 1;
		vec3 view_dir = normalize(view_pos - fragment.position);
		vec3 reflect_dir = reflect(-light_direction, normal); 
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
		specular_value +=  spec * light_color * vec3(specular.g);
	}

	if(color.a < 0.5) discard;
	float shadow = CalculateShadow();
    color = vec4((ambient + diffuse * (1.0f - shadow)) * color.rgb + (1.0f - shadow) * specular_value, 1);
	// color = vec4((ambient + diffuse * (1.0f - shadow)) * color.rgb, 1);
	color = mix(vec4(135, 206, 235, 255)/255.0, color, fog_factor);
	frag = color;
	//frag = vec4(normal * 0.5 + 0.5, 1);
}
