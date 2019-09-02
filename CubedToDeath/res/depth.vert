#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 texture_info;
layout (location = 5) in vec3 overlay_info;
layout (location = 6) in vec3 color;

uniform mat4 transform_matrix;
out vec2 tex_coords;
out float textureID;

void main()
{
    gl_Position = transform_matrix * vec4(position, 1.0);
	tex_coords = tex_coords;
	textureID = texture_info.x;
}
