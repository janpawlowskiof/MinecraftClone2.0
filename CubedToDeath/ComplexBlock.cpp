#include "ComplexBlock.h"
#include <glm\ext\matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "HitInfo.h"

using namespace blk;

ComplexBlock::~ComplexBlock()
{
}

bool blk::Torch::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info)
{
	float width = 1.0 / 16.0;
	float hit_x, hit_y, hit_z;
	if (direction.x > 0 && ProjectRayOnPlaneYZ(block_x + 0.5 - width, hit_y, hit_z, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_z, block_z + 0.5 - width, block_z + 0.5 + width))
	{
		hit_info.Update(block_x, block_y, block_z, block_x - 1, block_y, block_z, glm::length(origin - glm::vec3(block_x + 0.5 - width, hit_y, hit_z)));
		return true;
	}
	if (direction.x < 0 && ProjectRayOnPlaneYZ(block_x + 0.5 + width, hit_y, hit_z, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_z, block_z + 0.5 - width, block_z + 0.5 + width))
	{
		hit_info.Update(block_x, block_y, block_z, block_x + 1, block_y, block_z, glm::length(origin - glm::vec3(block_x + 0.5 + width, hit_y, hit_z)));
		return true;
	}
	if (direction.z > 0 && ProjectRayOnPlaneXY(block_z + 0.5 - width, hit_x, hit_y, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_x, block_x + 0.5 - width, block_x + 0.5 + width))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y, block_z - 1, glm::length(origin - glm::vec3(hit_x, hit_y, block_z + 0.5 - width)));
		return true;
	}
	if (direction.z < 0 && ProjectRayOnPlaneXY(block_z + 0.5 + width, hit_x, hit_y, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_x, block_x + 0.5 - width, block_x + 0.5 + width))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y, block_z + 1, glm::length(origin - glm::vec3(hit_x, hit_y, block_z + 0.5 + width)));
		return true;
	}
	if (direction.y > 0 && ProjectRayOnPlaneXZ(block_y + 1, hit_x, hit_z, origin, direction) && between(hit_z, block_z + 0.5 - width, block_z + 0.5 + width) && between(hit_x, block_x + 0.5 - width, block_x + 0.5 + width))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y - 1, block_z, glm::length(origin - glm::vec3(hit_x, block_y + 0.5 - width, hit_z)));
		return true;
	}
	if (direction.y < 0 && ProjectRayOnPlaneXZ(block_y, hit_x, hit_z, origin, direction) && between(hit_z, block_z + 0.5 - width, block_z + 0.5 + width) && between(hit_x, block_x + 0.5 - width, block_x + 0.5 + width))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y + 1, block_z, glm::length(origin - glm::vec3(hit_x, block_y + 0.5 + width, hit_z)));
		return true;
	}
	return false;
}
void blk::Torch::CreateModel(std::vector<Vertex> &vertices, int world_x, int world_y, int world_z)
{
	const float width = 1.0f / 8.0f;

	TextureInfo texture_info;
	texture_info.texture_id = tex_id::torch;
	texture_info.normal_id = tex_id::torch_n;
	texture_info.specular_id = tex_id::torch_s;

	glm::mat4 rotate = glm::mat4(1);
	switch (direction)
	{
	case BOTTOM:
		//transformation = glm::rot
		break;
	case NORTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case SOUTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		break;
	case WEST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, 1));
		break;
	case EAST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, -1));
		break;
	}
	CreateNorthFace(vertices, 0, 0, 0.5 + width / 2, texture_info);
	CreateSouthFace(vertices, 0, 0, 0.5 - width / 2, texture_info);
	CreateWestFace(vertices, 0.5 + width / 2, 0, 0, texture_info);
	CreateEastFace(vertices, 0.5 - width / 2, 0, 0, texture_info);
	auto iterator = vertices.end();
	for (int i = 0; i < 3 * GetNumberOfTriangles(); i++)
	{
		iterator--;
		iterator->Translate(glm::vec3(-0.5, -0.5, -0.5));
		iterator->Transform(rotate);
		iterator->Translate(glm::vec3(0.5, 0.5, 0.5));
		iterator->Translate(glm::vec3(world_x, world_y, world_z));
	}
}

bool blk::Switch::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info)
{
	switch (this->direction)
	{
	case BOTTOM:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 0.5 - width_base / 2, block_y, block_z + 0.5 - width_base / 2), glm::vec3(width_base, height_base, width_base), hit_info);
	case SOUTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 0.5 - width_base / 2, block_y + 0.5 - width_base / 2, block_z), glm::vec3(width_base, width_base, height_base), hit_info);
	case NORTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 0.5 - width_base / 2, block_y + 0.5 - width_base / 2, block_z + 1 - height_base), glm::vec3(width_base, width_base, height_base), hit_info);
	case WEST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y + 0.5 - width_base / 2, block_z + 0.5 - width_base / 2), glm::vec3(height_base, width_base, width_base), hit_info);
	case EAST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 1 - height_base, block_y + 0.5 - width_base / 2, block_z + 0.5 - width_base / 2), glm::vec3(height_base, width_base, width_base), hit_info);
	}
	return false;
}


void blk::Switch::CreateModel(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z)
{
	const float w = 16.0f / 128.0f;

	TextureInfo texture_info;
	texture_info.texture_id = tex_id::lever_base;

	glm::mat4 rotate = glm::mat4(1);
	switch (direction)
	{
	case BOTTOM:
		break;
	case NORTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case SOUTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		break;
	case WEST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, 1));
		break;
	case EAST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, -1));
		break;
	}
	CreateNorthFace(vertices, 0, 0, 1-w, texture_info);
	CreateSouthFace(vertices, 0, 0, w, texture_info);
	CreateWestFace(vertices, 1-w, 0, 0, texture_info);
	CreateEastFace(vertices, w, 0, 0, texture_info);

	texture_info.texture_id = tex_id::lever_base_top;
	CreateTopFace(vertices, 0, 29.0/128.0, 0, texture_info);

	auto iterator = vertices.end();
	for (int i = 0; i < 3 * GetNumberOfTriangles(); i++)
	{
		iterator--;
		iterator->Translate(glm::vec3(-0.5, -0.5, -0.5));
		iterator->Transform(rotate);
		iterator->Translate(glm::vec3(0.5, 0.5, 0.5));
		iterator->Translate(glm::vec3(world_x, world_y, world_z));
	}
}

bool blk::Door::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info)
{
	Direction current_direction = NORTH;
	if (opened)
	{
		if (this->direction == NORTH)
			current_direction = EAST;
		else if (this->direction == EAST)
			current_direction = SOUTH;
		else if (this->direction == SOUTH)
			current_direction = WEST;
		else if (this->direction == WEST)
			current_direction = NORTH;
	}
	else
	{
		current_direction = this->direction;
	}

	switch (current_direction)
	{
	case SOUTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y, block_z + depth / 2), glm::vec3(1, 1, depth), hit_info);
	case NORTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y, block_z + 1 - depth / 2), glm::vec3(1, 1, depth), hit_info);
	case WEST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + depth / 2, block_y, block_z), glm::vec3(depth, 1, 1), hit_info);
	case EAST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 1 - depth / 2, block_y, block_z), glm::vec3(depth, 1, 1), hit_info);
	}
	return false;
}

void blk::Door::CreateModel(std::vector<Vertex>&vertices, int world_x, int world_y, int world_z)
{
	glm::vec3 p000 = glm::vec3(0, 0, 0);
	glm::vec3 p001 = glm::vec3(0, 0, depth);
	glm::vec3 p010 = glm::vec3(0, 1, 0);
	glm::vec3 p011 = glm::vec3(0, 1, depth);
	glm::vec3 p100 = glm::vec3(1, 0, 0);
	glm::vec3 p101 = glm::vec3(1, 0, depth);
	glm::vec3 p110 = glm::vec3(1, 1, 0);
	glm::vec3 p111 = glm::vec3(1, 1, depth);

	glm::vec3 n001 = glm::vec3(0, 0, 1);
	glm::vec3 n010 = glm::vec3(0, 1, 0);
	glm::vec3 n100 = glm::vec3(1, 0, 0);

	glm::mat4 translate = glm::mat4(1);
	glm::mat4 rotate = glm::mat4(1);

	Direction current_direction = NORTH;
	if (opened)
	{
		if (direction == NORTH)
			current_direction = EAST;
		else if (direction == EAST)
			current_direction = SOUTH;
		else if (direction == SOUTH)
			current_direction = WEST;
		else if (direction == WEST)
			current_direction = NORTH;
	}
	else
	{
		current_direction = direction;
	}

	switch (current_direction)
	{
	case NORTH:
		rotate = glm::rotate(glm::radians(180.0f), glm::vec3(0, 1.0f, 0.0f));
		break;
	case SOUTH:
		//rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case WEST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 1.0f, 0.0f));
		break;
	case EAST:
		rotate = glm::rotate(glm::radians(270.0f), glm::vec3(0, 1, 0));
		break;
	}
	n001 = rotate * glm::vec4(n001, 1);
	n010 = rotate * glm::vec4(n010, 1);
	n100 = rotate * glm::vec4(n100, 1);

	translate = glm::translate(translate, glm::vec3(world_x, world_y, world_z));
	rotate = glm::translate(glm::vec3(0.5f, 0.5f, 0.5f)) * rotate * glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));

	p000 = translate * rotate * glm::vec4(p000, 1);
	p001 = translate * rotate * glm::vec4(p001, 1);
	p010 = translate * rotate * glm::vec4(p010, 1);
	p011 = translate * rotate * glm::vec4(p011, 1);
	p100 = translate * rotate * glm::vec4(p100, 1);
	p101 = translate * rotate * glm::vec4(p101, 1);
	p110 = translate * rotate * glm::vec4(p110, 1);
	p111 = translate * rotate * glm::vec4(p111, 1);

	const float tex_unit = 1.0f / 16.0f;
	const float side_width = 1 * tex_unit;

	int tex = top ? tex_id::door_top : tex_id::door_bottom;

	/*const float vertices[] = {
		xyz(p001), 0 + side_width, 1 - side_width, xyz(n001), tex,
		xyz(p101), 1 - side_width, 1 - side_width, xyz(n001),tex,
		xyz(p111), 1 - side_width, 0 + side_width, xyz(n001),tex,

		xyz(p001), 0 + side_width, 1 - side_width, xyz(n001),tex,
		xyz(p111), 1 - side_width, 0 + side_width, xyz(n001),tex,
		xyz(p011), 0 + side_width, 0 + side_width, xyz(n001),tex,

		xyz(p000), 0 + side_width, 1 - side_width, xyz(-n001),tex,
		xyz(p110), 1 - side_width, 0 + side_width , xyz(-n001),tex,
		xyz(p100), 1 - side_width, 1 - side_width, xyz(-n001),tex,

		xyz(p000), 0 + side_width, 1 - side_width, xyz(-n001),tex,
		xyz(p010), 0 + side_width, 0 + side_width, xyz(-n001),tex,
		xyz(p110), 1 - side_width, 0 + side_width, xyz(-n001),tex,

		xyz(p100), 0, 2 * side_width, xyz(n100),tex,
		xyz(p111), 1, 0, xyz(n100),tex,
		xyz(p101), 0, 0, xyz(n100),tex,

		xyz(p100), 0, 2 * side_width, xyz(n100),tex,
		xyz(p110), 1, 2 * side_width, xyz(n100),tex,
		xyz(p111), 1, 0, xyz(n100),tex,

		xyz(p011), 1, 0, xyz(-n100),tex,
		xyz(p000), 0, 2 * side_width, xyz(-n100),tex,
		xyz(p001), 0, 0, xyz(-n100),tex,

		xyz(p010), 1, 2 * side_width, xyz(-n100),tex,
		xyz(p000), 0, 2 * side_width, xyz(-n100),tex,
		xyz(p011), 1, 0, xyz(-n100),tex,

		xyz(p010), 0, 2 * side_width, xyz(n010),tex,
		xyz(p011), 0, 0, xyz(n010),tex,
		xyz(p111), 1, 0, xyz(n010),tex,

		xyz(p010), 0, 2 * side_width, xyz(n010),tex,
		xyz(p111), 1, 0, xyz(n010),tex,
		xyz(p110), 1, 2 * side_width, xyz(n010),tex,

		xyz(p000), 0, 2 * side_width, xyz(-n010),tex,
		xyz(p101), 1, 0, xyz(-n010),tex,
		xyz(p001), 0, 0, xyz(-n010),tex,

		xyz(p000), 0, 2 * side_width, xyz(-n010),tex,
		xyz(p100), 1, 2 * side_width, xyz(-n010),tex,
		xyz(p101), 1, 0, xyz(-n010),tex,
	};*/
	//std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	//return target + sizeof(vertices) / sizeof(float);
}

bool blk::Redstone::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info)
{

	return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y ,block_z), glm::vec3(1, 0.1, 1), hit_info);
}

void blk::Redstone::CreateModel(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z)
{
	auto color = glm::vec3(power_level, 0, 0);
	TextureInfo texture_info;
	texture_info.texture_id = tex_id::redstone_dot;
	CreateTopFace(vertices, world_x, world_y + 0.02, world_z, texture_info, TextureInfo(), color, color, color, color);
	texture_info.texture_id = tex_id::redstone;
	auto iterator = vertices.end();
	if (south_line)
	{
		CreateTopFace(vertices, world_x, world_y + 0.01, world_z, texture_info, TextureInfo(), color, color, color, color);
	}
	if (north_line)
	{
		CreateTopFace(vertices, -0.5, 0.01, -0.5, texture_info, TextureInfo(), color, color, color, color);
		auto iterator = vertices.end();
		for (int i = 0; i < 6; i++)
		{
			iterator--;
			iterator->Transform(glm::rotate(glm::radians(180.0f), v_up));
			iterator->Translate(glm::vec3(world_x + 0.5, world_y, world_z + 0.5));
		}
	}
	if (west_line)
	{
		CreateTopFace(vertices, -0.5, 0.01, -0.5, texture_info, TextureInfo(), color, color, color, color);
		auto iterator = vertices.end();
		for (int i = 0; i < 6; i++)
		{
			iterator--;
			iterator->Transform(glm::rotate(glm::radians(90.0f), v_up));
			iterator->Translate(glm::vec3(world_x + 0.5, world_y, world_z + 0.5));
		}
	}
	if (east_line)
	{
		CreateTopFace(vertices, -0.5, 0.01, -0.5, texture_info, TextureInfo(), color, color, color, color);
		auto iterator = vertices.end();
		for (int i = 0; i < 6; i++)
		{
			iterator--;
			iterator->Transform(glm::rotate(glm::radians(270.0f), v_up));
			iterator->Translate(glm::vec3(world_x + 0.5, world_y, world_z + 0.5));
		}
	}

}
