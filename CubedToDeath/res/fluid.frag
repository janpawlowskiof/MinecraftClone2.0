#version 450 core
out vec4 frag_color;

struct vData
{
	vec2 tex_coords;
	vec3 normal;
	vec3 frag_pos;
	float textureID;
};

in vData frag;

uniform sampler2DArray texture_terrain;

uniform vec3 view_pos;
uniform vec3 light_direction;

//uniform vec3 light_dir;
//uniform vec3 light_color;

void main()
{
	//vec3 light_dir = normalize(vec3(0.3, 0.9, 0.55));
	vec3 light_color = vec3(0.95, 0.9, 0.9);
	vec3 specular_color = vec3(1,1,1);
	float ambient_strength = 0.01;
    vec3 ambient = ambient_strength * light_color;

	float diff = max(dot(frag.normal, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	float specular_strength = 0.2;
	vec3 view_dir = normalize(view_pos - frag.frag_pos);
	vec3 reflect_dir = reflect(-light_direction, frag.normal); 
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 8);
	//vec3 specular = specular_strength * spec * light_color;  

	//vec4 color = texture(texture_terrain, vec3(frag.tex_coords, frag.textureID));
	//vec4 color = vec4(frag.normal, 1);
	vec4 color = vec4(0,0,1, 0.75);
	//color.a = 0.4;
    vec4 result = vec4(ambient + diffuse, 1) * color + spec * vec4(specular_color, 0.18);
	frag_color = result;
	//frag_color = vec4(frag.normal, 1);
}
