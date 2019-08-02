#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_terrain;

void main()
{
	vec3 light_dir = normalize(vec3(0.3, 0.7, 0.5));
	vec4 color = texture(texture_terrain, TexCoords);
	if(color.a < 0.5) discard;
    //FragColor = (max(dot(Normal, light_dir), 0.0) * vec4(1,1,1,1) + vec4(0.2, 0.2, 0.2, 1)) * mix(vec4(abs(Normal.x), abs(Normal.y), abs(Normal.z), 1), texture2D(texture_terrain, TexCoords), 0.5);
    FragColor = (max(dot(Normal, light_dir), 0.0) * vec4(1,1,1,1) + vec4(0.4, 0.4, 0.4, 1)) * texture2D(texture_terrain, TexCoords);
}
