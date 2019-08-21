#version 450 core
out vec4 frag_color;

struct vData
{
	vec2 tex_coords;
	vec3 normal;
	vec3 frag_pos;
	vec3 color;
	vec4 view_space;
};

in vData frag;

uniform sampler2D texture_terrain;

uniform vec3 view_pos;
uniform vec3 light_direction;
uniform float time;

//uniform vec3 light_dir;
//uniform vec3 light_color;

void main()
{
	float fog_density = 0.0020;
	float dist = length(frag.view_space);
	float fog_factor = 1.0 /exp(dist*dist * fog_density *fog_density );
	//float fog_factor = (240-dist)/(240-200);
    fog_factor = clamp( fog_factor, 0.0, 1.0 );
	//vec3 light_dir = normalize(vec3(0.3, 0.9, 0.55));
	vec3 light_color = vec3(0.95, 0.9, 0.9);
	vec3 specular_color = vec3(1,1,1);
	float ambient_strength = 0.01;
    vec3 ambient = ambient_strength * light_color;

	float diff = max(dot(frag.normal, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	int image_index = int(4 * time);
	vec2 tex_coords = vec2(frag.tex_coords.x, (frag.tex_coords.y + image_index) / 32.0);
	vec4 texture_color = texture2D(texture_terrain, tex_coords);
	texture_color.a += 0.1;
	vec4 color = texture_color * vec4(frag.color, 1);

	float specular_strength = 0.1;
	vec3 view_dir = normalize(view_pos - frag.frag_pos);
	vec3 reflect_dir = reflect(-light_direction, frag.normal); 
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 8);
	//vec3 specular = specular_strength * spec * light_color;  

    vec4 result = vec4(ambient + diffuse, 1) * color + spec * vec4(specular_color, 0.18);
	result = mix(vec4(135, 206, 235, 255)/255.0, result, fog_factor);
	frag_color = result;
	//frag_color = vec4(frag.normal, 1);
}
