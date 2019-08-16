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

bool blk::Torch::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info)
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
float* blk::Torch::CreateModel(float* target, int world_x, int world_y, int world_z)
{
	const float width = 1.0f / 16.0f;

	glm::vec3 p000 = glm::vec3(0.5 - width, 0, 0.5 - width);
	glm::vec3 p001 = glm::vec3(0.5 - width, 0, 0.5 + width);
	glm::vec3 p010 = glm::vec3(0.5 - width, 1, 0.5 - width);
	glm::vec3 p011 = glm::vec3(0.5 - width, 1, 0.5 + width);
	glm::vec3 p100 = glm::vec3(0.5 + width, 0, 0.5 - width);
	glm::vec3 p101 = glm::vec3(0.5 + width, 0, 0.5 + width);
	glm::vec3 p110 = glm::vec3(0.5 + width, 1, 0.5 - width);
	glm::vec3 p111 = glm::vec3(0.5 + width, 1, 0.5 + width);

	glm::vec3 n001 = glm::vec3(0, 0, 1);
	glm::vec3 n010 = glm::vec3(0, 1, 0);
	glm::vec3 n100 = glm::vec3(1, 0, 0);

	glm::mat4 translate = glm::mat4(1);
	glm::mat4 rotate = glm::mat4(1);
	switch (direction)
	{
	case BOTTOM:
		//transformation = glm::rot
		break;
	case NORTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		break;
	case SOUTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case WEST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, -1));
		break;
	case EAST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, 1));
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
	const float left_bound = 0.5f - tex_unit;
	const float right_bound = 0.5f + tex_unit;

	const float vertices[] = {
		xyz(p001), left_bound , (1) , xyz(n001), tex_id::torch_side,
		xyz(p101), right_bound , (1) , xyz(n001),tex_id::torch_side,
		xyz(p111),right_bound , 0 , xyz(n001),tex_id::torch_side,

		xyz(p001), left_bound, (1) , xyz(n001),tex_id::torch_side,
		xyz(p111), right_bound , 0 , xyz(n001),tex_id::torch_side,
		xyz(p011), left_bound, 0 , xyz(n001),tex_id::torch_side,

		xyz(p000), left_bound, (1) , xyz(-n001),tex_id::torch_side,
		xyz(p110), right_bound , 0 , xyz(-n001),tex_id::torch_side,
		xyz(p100), right_bound , (1) , xyz(-n001),tex_id::torch_side,

		xyz(p000), left_bound, (1) , xyz(-n001),tex_id::torch_side,
		xyz(p010), left_bound, 0 , xyz(-n001),tex_id::torch_side,
		xyz(p110) ,right_bound, 0 , xyz(-n001),tex_id::torch_side,

		xyz(p100), left_bound, (1) , xyz(n100),tex_id::torch_side,
		xyz(p111), right_bound , 0 ,xyz(n100),tex_id::torch_side,
		xyz(p101), right_bound , (1) , xyz(n100),tex_id::torch_side,

		xyz(p100), left_bound, (1) , xyz(n100),tex_id::torch_side,
		xyz(p110), left_bound, 0 ,	xyz(n100),tex_id::torch_side,
		xyz(p111), right_bound , 0 ,xyz(n100),tex_id::torch_side,

		xyz(p011), right_bound , 0 , xyz(-n100),tex_id::torch_side,
		xyz(p000), left_bound, 1 ,xyz(-n100),tex_id::torch_side,
		xyz(p001), right_bound , 1 , xyz(-n100),tex_id::torch_side,

		xyz(p010), right_bound , 0 , xyz(-n100),tex_id::torch_side,
		xyz(p000), left_bound, (1) , xyz(-n100),tex_id::torch_side,
		xyz(p011), left_bound, 0 , xyz(-n100),tex_id::torch_side,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

bool blk::Switch::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info)
{
	switch (this->direction)
	{
	case BOTTOM:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 0.5 - width_base / 2, block_y, block_z + 0.5 - width_base / 2), glm::vec3(width_base, height_base, width_base), hit_info);
	case SOUTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 0.5 - width_base / 2, block_y + 0.5 - width_base/2, block_z), glm::vec3(width_base, width_base, height_base), hit_info);
	case NORTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 0.5 - width_base / 2, block_y + 0.5 - width_base / 2, block_z + 1 - height_base), glm::vec3(width_base, width_base, height_base), hit_info);
	case WEST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y + 0.5 - width_base / 2, block_z + 0.5 - width_base / 2), glm::vec3(height_base, width_base, width_base), hit_info);
	case EAST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 1 - height_base, block_y + 0.5 - width_base / 2, block_z + 0.5 - width_base / 2), glm::vec3(height_base, width_base, width_base), hit_info);
	}
	return false;
}


float* blk::Switch::CreateModel(float* target, int world_x, int world_y, int world_z)
{
	glm::vec3 p000 = glm::vec3(0.5 - width_base/2.0, 0, 0.5 - width_base/2.0);
	glm::vec3 p001 = glm::vec3(0.5 - width_base/2.0, 0, 0.5 + width_base/2.0);
	glm::vec3 p010 = glm::vec3(0.5 - width_base/2.0, height_base, 0.5 - width_base/2.0);
	glm::vec3 p011 = glm::vec3(0.5 - width_base/2.0, height_base, 0.5 + width_base/2.0);
	glm::vec3 p100 = glm::vec3(0.5 + width_base/2.0, 0, 0.5 - width_base/2.0);
	glm::vec3 p101 = glm::vec3(0.5 + width_base/2.0, 0, 0.5 + width_base/2.0);
	glm::vec3 p110 = glm::vec3(0.5 + width_base/2.0, height_base, 0.5 - width_base/2.0);
	glm::vec3 p111 = glm::vec3(0.5 + width_base/2.0, height_base, 0.5 + width_base/2.0);

	glm::vec3 b000 = glm::vec3(0.5 - width_button/2.0/2.0, 0, 0.5 - width_button/2.0);
	glm::vec3 b001 = glm::vec3(0.5 - width_button/2.0, 0, 0.5 + width_button/2.0);
	glm::vec3 b010 = glm::vec3(0.5 - width_button/2.0, height_button, 0.5 - width_button/2.0);
	glm::vec3 b011 = glm::vec3(0.5 - width_button/2.0, height_button, 0.5 + width_button/2.0);
	glm::vec3 b100 = glm::vec3(0.5 + width_button/2.0, 0, 0.5 - width_button/2.0);
	glm::vec3 b101 = glm::vec3(0.5 + width_button/2.0, 0, 0.5 + width_button/2.0);
	glm::vec3 b110 = glm::vec3(0.5 + width_button/2.0, height_button, 0.5 - width_button/2.0);
	glm::vec3 b111 = glm::vec3(0.5 + width_button/2.0, height_button, 0.5 + width_button/2.0);

	glm::vec3 n001 = glm::vec3(0, 0, 1);
	glm::vec3 n010 = glm::vec3(0, 1, 0);
	glm::vec3 n100 = glm::vec3(1, 0, 0);

	glm::mat4 translate = glm::mat4(1);
	glm::mat4 rotate = glm::mat4(1);
	switch (direction)
	{
	case BOTTOM:
		//transformation = glm::rot
		break;
	case NORTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		break;
	case SOUTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case WEST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, -1));
		break;
	case EAST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, 1));
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

	b000 = translate * rotate * glm::vec4(b000, 1);
	b001 = translate * rotate * glm::vec4(b001, 1);
	b010 = translate * rotate * glm::vec4(b010, 1);
	b011 = translate * rotate * glm::vec4(b011, 1);
	b100 = translate * rotate * glm::vec4(b100, 1);
	b101 = translate * rotate * glm::vec4(b101, 1);
	b110 = translate * rotate * glm::vec4(b110, 1);
	b111 = translate * rotate * glm::vec4(b111, 1);

	const float tex_unit = 1.0f / 16.0f;
	const float left_bound = 0.5f - 5 * tex_unit;
	const float right_bound = 0.5f + 5 * tex_unit;

	const float vertices[] = {
		xyz(p001), left_bound , (1) , xyz(n001), tex_id::stone,
		xyz(p101), right_bound , (1) , xyz(n001),tex_id::stone,
		xyz(p111),right_bound , 0 , xyz(n001),tex_id::stone,

		xyz(p001), left_bound, (1) , xyz(n001),tex_id::stone,
		xyz(p111), right_bound , 0 , xyz(n001),tex_id::stone,
		xyz(p011), left_bound, 0 , xyz(n001),tex_id::stone,

		xyz(p000), left_bound, (1) , xyz(-n001),tex_id::stone,
		xyz(p110), right_bound , 0 , xyz(-n001),tex_id::stone,
		xyz(p100), right_bound , (1) , xyz(-n001),tex_id::stone,

		xyz(p000), left_bound, (1) , xyz(-n001),tex_id::stone,
		xyz(p010), left_bound, 0 , xyz(-n001),tex_id::stone,
		xyz(p110) ,right_bound, 0 , xyz(-n001),tex_id::stone,

		xyz(p100), left_bound, (1) , xyz(n100),tex_id::stone,
		xyz(p111), right_bound , 0 ,xyz(n100),tex_id::stone,
		xyz(p101), right_bound , (1) , xyz(n100),tex_id::stone,

		xyz(p100), left_bound, (1) , xyz(n100),tex_id::stone,
		xyz(p110), left_bound, 0 ,	xyz(n100),tex_id::stone,
		xyz(p111), right_bound , 0 ,xyz(n100),tex_id::stone,

		xyz(p011), right_bound , 0 , xyz(-n100),tex_id::stone,
		xyz(p000), left_bound, 1 ,xyz(-n100),tex_id::stone,
		xyz(p001), right_bound , 1 , xyz(-n100),tex_id::stone,

		xyz(p010), right_bound , 0 , xyz(-n100),tex_id::stone,
		xyz(p000), left_bound, (1) , xyz(-n100),tex_id::stone,
		xyz(p011), left_bound, 0 , xyz(-n100),tex_id::stone,

		xyz(p010), right_bound , 0 , xyz(n010),tex_id::stone,
		xyz(p011), right_bound , 1 , xyz(n010),tex_id::stone,
		xyz(p111), left_bound, 1 ,xyz(n010),tex_id::stone,

		xyz(p010), right_bound , 0 , xyz(n010),tex_id::stone,
		xyz(p111), left_bound, 0 , xyz(n010),tex_id::stone,
		xyz(p110), left_bound, (1) , xyz(n010),tex_id::stone,



		//						BUTTON							//
		xyz(b001), left_bound , (1) , xyz(n001), turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b101), right_bound , (1) , xyz(n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b111),right_bound , 0 , xyz(n001),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b001), left_bound, (1) , xyz(n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b111), right_bound , 0 , xyz(n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b011), left_bound, 0 , xyz(n001),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b000), left_bound, (1) , xyz(-n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b110), right_bound , 0 , xyz(-n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b100), right_bound , (1) , xyz(-n001),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b000), left_bound, (1) , xyz(-n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b010), left_bound, 0 , xyz(-n001),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b110) ,right_bound, 0 , xyz(-n001),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b100), left_bound, (1) , xyz(n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b111), right_bound , 0 ,xyz(n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b101), right_bound , (1) , xyz(n100),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b100), left_bound, (1) , xyz(n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b110), left_bound, 0 ,	xyz(n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b111), right_bound , 0 ,xyz(n100),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b011), right_bound , 0 , xyz(-n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b000), left_bound, 1 ,xyz(-n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b001), right_bound , 1 , xyz(-n100),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b010), right_bound , 0 , xyz(-n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b000), left_bound, (1) , xyz(-n100),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b011), left_bound, 0 , xyz(-n100),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b010), right_bound , 0 , xyz(n010),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b011), right_bound , 1 , xyz(n010),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b111), left_bound, 1 ,xyz(n010),turned_on ? tex_id::wood_side : tex_id::dirt,

		xyz(b010), right_bound , 0 , xyz(n010),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b111), left_bound, 0 , xyz(n010),turned_on ? tex_id::wood_side : tex_id::dirt,
		xyz(b110), left_bound, (1) , xyz(n010),turned_on ? tex_id::wood_side : tex_id::dirt,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

bool blk::Door::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info)
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
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y, block_z + depth/2), glm::vec3(1, 1, depth), hit_info);
	case NORTH:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y, block_z + 1 - depth / 2), glm::vec3(1, 1, depth), hit_info);
	case WEST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + depth / 2, block_y, block_z), glm::vec3(depth, 1, 1), hit_info);
	case EAST:
		return CheckRayVsAABB(origin, direction, glm::vec3(block_x + 1 - depth / 2, block_y, block_z), glm::vec3(depth, 1, 1), hit_info);
	}
	return false;
}

float* blk::Door::CreateModel(float* target, int world_x, int world_y, int world_z)
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
	const float side_width= 1 * tex_unit;

	int tex = top ? tex_id::door_top : tex_id::door_bottom;

	const float vertices[] = {
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
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
