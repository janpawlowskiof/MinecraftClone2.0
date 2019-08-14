#version 450 core
out vec4 frag;

in vec2 tex_coords;
in vec3 normal;
in vec3 frag_pos;
in float textureID;

uniform sampler2DArray texture_terrain;

uniform vec3 view_pos;

//uniform vec3 light_dir;
//uniform vec3 light_color;

void main()
{
	vec3 light_dir = normalize(vec3(0.3, 0.9, 0.55));
	vec3 light_color = vec3(0.95, 0.9, 0.9);
	float ambient_strength = 0.5;
    vec3 ambient = ambient_strength * light_color;

	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	float specular_strength = 0.5;
	vec3 view_dir = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal); 
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 4);
	vec3 specular = specular_strength * spec * light_color;  

	vec4 color = texture(texture_terrain, vec3(tex_coords, textureID));
	color.a = 0.4;
    vec4 result = vec4((ambient + diffuse + specular), 1) * color;
	frag = result;
}
