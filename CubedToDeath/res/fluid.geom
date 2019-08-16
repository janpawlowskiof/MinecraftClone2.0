#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

struct vData
{
	vec2 tex_coords;
	vec3 normal;
	vec3 frag_pos;
	float textureID;
};

in vData vertex[];
out vData frag; 


void main()
{
	vec3 u = vertex[0].frag_pos - vertex[2].frag_pos;
	vec3 v = vertex[1].frag_pos - vertex[2].frag_pos;
	vec3 normal = normalize(cross(u, v));

    for(int index = 0; index < 3; index++)
	{
		gl_Position = gl_in[index].gl_Position;
		frag.tex_coords = vertex[index].tex_coords;
		frag.frag_pos = vertex[index].frag_pos;
		frag.textureID= vertex[index].textureID;
		//frag.normal = vec3(vertex[index].frag_pos.y - floor(vertex[index].frag_pos.y));
		frag.normal = normal;
		EmitVertex();
	}
	EndPrimitive();
}
