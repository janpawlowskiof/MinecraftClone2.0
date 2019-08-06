#version 450 core

uniform sampler2D depthMap;
//in vec2 TexCoords;
//out vec4 FragColor;


void main()
{             
    //FragColor = texture(depthMap, TexCoords);
    //FragColor = vec4(vec3(LinearizeDepth(depthValue) / 150), 1.0); // perspective
    //FragColor = vec4(vec3(depthValue), 1.0); // orthographic
	//FragColor = vec4(1,0,0,1);
}
