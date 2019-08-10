#version 450 core
uniform sampler2DArray texture_terrain;
in float textureID;
in vec2 tex_coords;


void main()
{             
    vec4 color = texture(texture_terrain, vec3(tex_coords, textureID));
	if(color.a < 0.5) discard;
}
