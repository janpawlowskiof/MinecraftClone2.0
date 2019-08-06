#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 transform_matrix;

//out vec2 TexCoords;

void main()
{
    gl_Position = transform_matrix * vec4(aPos, 1.0);
}
