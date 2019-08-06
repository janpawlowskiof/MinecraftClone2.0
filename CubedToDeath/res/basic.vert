#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;
out vec4 light_space_frag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_matrix;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
	tex_coords = aTex;
	normal = aNorm;
	frag_pos = aPos;
	light_space_frag = light_space_matrix * vec4(aPos, 1.0);
}
