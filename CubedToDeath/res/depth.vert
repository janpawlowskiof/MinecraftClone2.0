#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 3) in float aTextureID;

uniform mat4 transform_matrix;
out vec2 tex_coords;
out float textureID;

void main()
{
    gl_Position = transform_matrix * vec4(aPos, 1.0);
	tex_coords = aTex;
	textureID = aTextureID;
}
