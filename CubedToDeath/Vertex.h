#pragma once
#include "glm/glm.hpp"
#include "TextureInfo.h"

//	world_x, world_y, world_z,		texture_x, texture_y,		normal_x, normal_y, normal_z,		texture_id,		overlay_id,			colorization.rgb	//

#define v_up glm::vec3(0, 1, 0)
#define v_down glm::vec3(0, -1, 0)
#define v_forward glm::vec3(0, 0, 1)
#define v_backward glm::vec3(0, 0, -1)
#define v_right glm::vec3(1, 0, 0)
#define v_left glm::vec3(-1, 0, 0)

struct Vertex 
{
	Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 norm, glm::vec3 tan, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo(), glm::vec3 col = glm::vec3(-1))
	{
		position = pos;
		tex_coords = tex;
		normal = norm;
		this->texture_info = texture_info;
		this->overlay_info = overlay_info;
		color = col;
		tangent = tan;
	}
	glm::vec3 position;
	glm::vec2 tex_coords;
	glm::vec3 normal;
	glm::vec3 tangent;
	TextureInfo texture_info;
	TextureInfo overlay_info;
	glm::vec3 color;

	void Translate(glm::vec3 v)
	{
		position += v;
	}

	void Transform(glm::mat4 m)
	{
		position = glm::vec4(position, 1) * m;
		normal = glm::vec4(normal, 1) * m;
		tangent = glm::vec4(tangent, 1) * m;
	}
};