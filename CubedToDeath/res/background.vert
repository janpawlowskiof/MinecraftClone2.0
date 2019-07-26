#version 450 core
layout (location = 0) in vec2 vertex; // <vec2 pos, vec2 tex>

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, -0.1, 1.0);
}  