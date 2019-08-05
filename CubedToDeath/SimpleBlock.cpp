#include "SimpleBlock.h"
#include <iostream>
#include "HitInfo.h"
#include "ComplexBlock.h"

SimpleBlock::SimpleBlock(unsigned char id)
{
	this->id = id;

	switch (id)
	{
	case blk_id::air_id:
		SetFlag(OPAQUE, false);
		break;
	case blk_id::water_id:
		SetFlag(OPAQUE, false);
		SetFlag(FLUID, true);
		break;
	default:
		SetFlag(OPAQUE, true);
	}
}


SimpleBlock::~SimpleBlock()
{
}

//											vertex format										//
//	world_x, world_y, world_z,		texture_x, texture_y,		normal_x, normal_y, normal_z	//

//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateNorthFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, texture_x *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y *m_unit, 0, 0, 1,

		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, texture_x *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y *m_unit, 0, 0, 1,
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, texture_x *m_unit, texture_y *m_unit, 0, 0, 1 };
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateSouthFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 0, 0, -1,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1) *m_unit, texture_y*m_unit, 0, 0, -1,
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, 0, 0, -1,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, 0, 0, -1,
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x *m_unit, texture_y*m_unit, 0, 0, -1,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1) *m_unit, texture_y*m_unit, 0, 0, -1,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateTopFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, 1, 0,
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (texture_x)*m_unit, (texture_y + 1)*m_unit, 0, 1, 0,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 0, 1, 0,

		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, 1, 0,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 0, 1, 0,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 0, 1, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateBottomFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, -1, 0,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 0, -1, 0,
		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (texture_x)*m_unit, (texture_y + 1)*m_unit, 0, -1, 0,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, -1, 0,
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 0, -1, 0,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, 0, -1, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateWestFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	const float vertices[] = {
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 1, 0, 0,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,

		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, texture_y*m_unit, 1, 0, 0,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 1, 0, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateEastFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
		world_x + 0.0f, world_y + 1.0f, 1 + world_z, (texture_x + 1) *m_unit, texture_y*m_unit, -1, 0, 0,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y*m_unit, -1, 0, 0,
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x *m_unit, texture_y*m_unit, -1, 0, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

float* SimpleBlock::CreateModel(float* target, int world_x, int world_y, int world_z)
{
	if (face_visible == 0)
		return target;

	int side_x = 0, side_y = 0, top_x = 0, top_y = 0, bottom_x = 0, bottom_y = 0;
	switch (id)
	{
	case blk_id::dirt_id:
		side_x = top_x = bottom_x = 2;
		side_y = top_y = bottom_y = 0;
		break;
	case blk_id::stone_id:
		side_x = top_x = bottom_x = 1;
		side_y = top_y = bottom_y = 0;
		break;
	case blk_id::wood_id:
		side_x = 4;
		side_y = 1;
		top_x = bottom_x = 5;
		top_y = bottom_y = 1;
		break;
	case blk_id::grass_id:
		side_x = 3;
		side_y = 0;
		top_x = bottom_x = 8;
		top_y = bottom_y = 2;
		break;
	case blk_id::leaves_id:
		top_x = bottom_x = side_x = 5;
		top_y = bottom_y = side_y = 3;
		break;
	case blk_id::water_id:
		top_x = 13;
		top_y = 12;
		if (GetFaceVisible(TOP))
			return CreateTopFace(target, world_x, world_y, world_z, top_x, top_y);
		else
			return target;
		break;
	case blk_id::air_id:
	default:
		return target;
	}
	if (GetFaceVisible(NORTH))
	{
		target = CreateNorthFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(SOUTH))
	{
		target = CreateSouthFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(WEST))
	{
		target = CreateWestFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(EAST))
	{
		target = CreateEastFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(TOP))
	{
		target = CreateTopFace(target, world_x, world_y, world_z, top_x, top_y);
	}
	if (GetFaceVisible(BOTTOM))
	{
		target = CreateBottomFace(target, world_x, world_y, world_z, bottom_x, bottom_y);
	}
	return target;
}

bool SimpleBlock::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info)
{
	//east - west check
	if (origin.x <= block_x)
	{
		if(glm::dot(direction, glm::vec3(1, 0, 0)) == 0)
			return false;

		float hit_y = direction.y / direction.x * (block_x - origin.x) + origin.y;
		float hit_z = direction.z / direction.x * (block_x - origin.x) + origin.z;

		if (hit_y >= block_y && hit_y <= block_y + 1 && hit_z >= block_z && hit_z <= block_z + 1)
		{
			hit_info.Update(block_x, block_y, block_z, block_x - 1, block_y, block_z, glm::length(origin - glm::vec3(block_x, hit_y, hit_z)));
			return true;
		}
	}
	else if (origin.x >= block_x + 1)
	{
		if (glm::dot(direction, glm::vec3(-1, 0, 0)) == 0)
			return false;

		float hit_y = direction.y / direction.x * (block_x + 1 - origin.x) + origin.y;
		float hit_z = direction.z / direction.x * (block_x + 1 - origin.x) + origin.z;

		if (hit_y >= block_y && hit_y <= block_y + 1 && hit_z >= block_z && hit_z <= block_z + 1)
		{
			hit_info.Update(block_x, block_y, block_z, block_x + 1, block_y, block_z, glm::length(origin - glm::vec3(block_x + 1, hit_y, hit_z)));
			return true;
		}
	}
	if (origin.z <= block_z)
	{
		if (glm::dot(direction, glm::vec3(0, 0, 1)) == 0)
			return false;

		float hit_x = direction.x / direction.z * (block_z - origin.z) + origin.x;
		float hit_y = direction.y / direction.z * (block_z - origin.z) + origin.y;

		if (hit_y >= block_y && hit_y <= block_y + 1 && hit_x >= block_x && hit_x <= block_x + 1)
		{
			hit_info.Update(block_x, block_y, block_z, block_x, block_y, block_z - 1, glm::length(origin - glm::vec3(hit_x, hit_y, block_z)));
			return true;
		}
	}
	else if (origin.z >= block_z + 1)
	{
		if (glm::dot(direction, glm::vec3(0, 0, -1)) == 0)
			return false;

		float hit_x = direction.x / direction.z * (block_z + 1 - origin.z) + origin.x;
		float hit_y = direction.y / direction.z * (block_z + 1 - origin.z) + origin.y;

		if (hit_y >= block_y && hit_y <= block_y + 1 && hit_x >= block_x && hit_x <= block_x + 1)
		{
			hit_info.Update(block_x, block_y, block_z, block_x, block_y, block_z + 1, glm::length(origin - glm::vec3(hit_x, hit_y, block_z + 1)));
			return true;
		}
	}
	if (origin.y <= block_y)
	{
		if (glm::dot(direction, glm::vec3(0, 1, 0)) == 0)
			return false;

		float hit_x = direction.x / direction.y * (block_y - origin.y) + origin.x;
		float hit_z = direction.z / direction.y * (block_y - origin.y) + origin.z;

		if (hit_z >= block_z && hit_z <= block_z + 1 && hit_x >= block_x && hit_x <= block_x + 1)
		{
			hit_info.Update(block_x, block_y, block_z, block_x, block_y - 1, block_z, glm::length(origin - glm::vec3(hit_x, block_y, hit_z)));
			return true;
		}
	}
	else if (origin.y >= block_y + 1)
	{
		if (glm::dot(direction, glm::vec3(0, -1, 0)) == 0)
			return false;

		float hit_x = direction.x / direction.y * (block_y + 1 - origin.y) + origin.x;
		float hit_z = direction.z / direction.y * (block_y + 1 - origin.y) + origin.z;

		if (hit_z >= block_z && hit_z <= block_z + 1 && hit_x >= block_x && hit_x <= block_x + 1)
		{
			hit_info.Update(block_x, block_y, block_z, block_x, block_y + 1, block_z, glm::length(origin - glm::vec3(hit_x, block_y + 1, hit_z)));
			return true;
		}
	}
	return false;
}

SimpleBlock* SimpleBlock::CreateNew(int block_id)
{
	switch (block_id)
	{
	case blk_id::torch_id:
		return new blk::Torch();
	
	default:
		return new SimpleBlock(block_id);
	}
}
