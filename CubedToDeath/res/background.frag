#version 450 core

out vec4 color;
in vec2 tex_coords;

uniform vec3 input_color;
uniform sampler2D tex;

void main()
{
	color = texture2D(tex, tex_coords);
	//color = vec4(1,0,0,1);
}