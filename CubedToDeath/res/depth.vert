#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 4) in vec3 texture_info;
layout (location = 5) in vec3 overlay_info;

uniform mat4 transform_matrix;
out vec2 tex_coords;
out float textureID;

void main()
{
    gl_Position = transform_matrix * vec4(position, 1.0);
	tex_coords = texture_coords;
	if(texture_info.x >= 0)
		textureID = texture_info.x;
	else
		textureID = overlay_info.x;
}
