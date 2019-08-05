#version 450 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 a_tex_coords;

out vec2 tex_coords;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, -0.1, 1.0);
	tex_coords = a_tex_coords;
}  