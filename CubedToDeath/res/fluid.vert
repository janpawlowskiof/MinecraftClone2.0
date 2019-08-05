#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    gl_Position = projection * view * vec4(aPos - vec3(0, 0.3 + 0.15 * sin(time + 0.2 * aPos.x + aPos.z * 0.5), 0), 1.0);
	tex_coords = aTex;
	normal = aNorm;
	frag_pos = aPos;
}
