#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in float aTextureID;

struct vData
{
	vec2 tex_coords;
	vec3 normal;
	vec3 frag_pos;
	float textureID;
};

out vData vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
	vertex.tex_coords = aTex;
	vertex.normal = aNorm;
	vertex.frag_pos = aPos - vec3(0, 0.3 + 0.15 * sin(time + 0.46 * aPos.x + aPos.z * 1.5), 0);
	vertex.textureID = aTextureID;
    gl_Position = projection * view * vec4(vertex.frag_pos, 1.0);
}
