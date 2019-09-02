#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 texture_info;
layout (location = 5) in vec3 overlay_info;
layout (location = 6) in vec3 color;

out Vertex
{
	vec2 texture_coords;
	vec4 light_space_close_frag;
	vec4 light_space_far_frag;
	float texture_color;
	float texture_normal;
	float texture_specular;
	float overlay_color;
	float overlay_normal;
	float overlay_specular;
	vec3 position;
	vec4 view_space;
	vec3 colorization;
	mat3 TBN;
}vertex;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space_close_matrix;
uniform mat4 light_space_far_matrix;

void main()
{
    gl_Position = projection * view * vec4(position, 1.0);
	vertex.texture_coords = texture_coords;
	//frag_pos = aPos;
	vertex.colorization = color;

	vertex.texture_color = texture_info.x;
	vertex.texture_normal = texture_info.y;
	vertex.texture_specular = texture_info.z;

	vertex.overlay_color = overlay_info.x;
	vertex.overlay_normal = overlay_info.y;
	vertex.overlay_specular = overlay_info.z;

	vertex.light_space_close_frag = light_space_close_matrix * vec4(position, 1.0);
	vertex.light_space_far_frag = light_space_far_matrix * vec4(position, 1.0);
	vertex.view_space = view * vec4(position, 1.0);
	vertex.position = position;
	vertex.TBN = mat3(tangent, cross(tangent, normal), normal);
}
