#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;
layout (location = 3) in float aTextureID;
layout (location = 4) in float aOverlayID;
layout (location = 5) in vec3 aColor;

out vec2 tex_coords;
out vec3 normal;
out vec3 frag_pos;
out vec4 light_space_close_frag;
out vec4 light_space_far_frag;
out float textureID;
out vec4 view_space;
out float overlayID;
out vec3 overlay_colorization;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_close_matrix;
uniform mat4 light_space_far_matrix;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
	tex_coords = aTex;
	normal = aNorm;
	frag_pos = aPos;
	overlay_colorization = aColor;
	overlayID = aOverlayID;
	light_space_close_frag = light_space_close_matrix * vec4(aPos, 1.0);
	light_space_far_frag = light_space_far_matrix * vec4(aPos, 1.0);
	textureID = aTextureID;
	view_space = view * vec4(aPos, 1.0);
}
