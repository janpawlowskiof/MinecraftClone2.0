#pragma once
#include "glm/glm.hpp"
#include "TextureInfo.h"

//	world_x, world_y, world_z,		texture_x, texture_y,		normal_x, normal_y, normal_z,		texture_id,		overlay_id,			colorization.rgb	//

struct Vertex 
{
	Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 norm, glm::vec3 tan, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 col)
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
};