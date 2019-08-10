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
	case blk_id::leaves_id:
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

//											vertex format														//
//	world_x, world_y, world_z,		texture_x, texture_y,		normal_x, normal_y, normal_z,		texture_id	//

//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateNorthFace(float* target, int world_x, int world_y, int world_z, int texture_id)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, 0 *m_unit, (0 + 1) *m_unit, 0, 0, 1, texture_id,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (0 + 1) *m_unit, (0 + 1) *m_unit, 0, 0, 1,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1) *m_unit, 0 *m_unit, 0, 0, 1,texture_id,

		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, 0 *m_unit, (0 + 1) *m_unit, 0, 0, 1,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1) *m_unit, 0 *m_unit, 0, 0, 1,texture_id,
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, 0 *m_unit, 0 *m_unit, 0, 0, 1,texture_id, };
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateSouthFace(float* target, int world_x, int world_y, int world_z, int texture_id)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0*m_unit, (0 + 1)*m_unit, 0, 0, -1,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (0 + 1) *m_unit, 0*m_unit, 0, 0, -1,texture_id,
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (0 + 1) *m_unit, (0 + 1)*m_unit, 0, 0, -1,texture_id,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 *m_unit, (0 + 1)*m_unit, 0, 0, -1,texture_id,
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 *m_unit, 0*m_unit, 0, 0, -1,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (0 + 1) *m_unit, 0*m_unit, 0, 0, -1,texture_id,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateTopFace(float* target, int world_x, int world_y, int world_z, int texture_id)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0*m_unit, (0)*m_unit, 0, 1, 0,texture_id,
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (0)*m_unit, (0 + 1)*m_unit, 0, 1, 0,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1)*m_unit, (0 + 1)*m_unit, 0, 1, 0,texture_id,

		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0*m_unit, (0)*m_unit, 0, 1, 0,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1)*m_unit, (0 + 1)*m_unit, 0, 1, 0,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (0 + 1)*m_unit, 0*m_unit, 0, 1, 0,texture_id,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateBottomFace(float* target, int world_x, int world_y, int world_z, int texture_id)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0*m_unit, (0)*m_unit, 0, -1, 0,texture_id,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (0 + 1)*m_unit, (0 + 1)*m_unit, 0, -1, 0,texture_id,
		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (0)*m_unit, (0 + 1)*m_unit, 0, -1, 0,texture_id,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0*m_unit, (0)*m_unit, 0, -1, 0,texture_id,
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (0 + 1)*m_unit, 0*m_unit, 0, -1, 0,texture_id,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (0 + 1) *m_unit, (0 + 1)*m_unit, 0, -1, 0,texture_id,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateWestFace(float* target, int world_x, int world_y, int world_z, int texture_id)
{
	const float vertices[] = {
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, 0*m_unit, (0 + 1)*m_unit, 1, 0, 0,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1)*m_unit, 0*m_unit, 1, 0, 0,texture_id,
		world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (0 + 1)*m_unit, (0 + 1)*m_unit, 1, 0, 0,texture_id,

		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, 0*m_unit, (0 + 1)*m_unit, 1, 0, 0,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, 0*m_unit, 0*m_unit, 1, 0, 0,texture_id,
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1)*m_unit, 0*m_unit, 1, 0, 0,texture_id,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateEastFace(float* target, int world_x, int world_y, int world_z, int texture_id)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 *m_unit, (0 + 1)*m_unit, -1, 0, 0,texture_id,
		world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (0 + 1) *m_unit, (0 + 1)*m_unit, -1, 0, 0,texture_id,
		world_x + 0.0f, world_y + 1.0f, 1 + world_z, (0 + 1) *m_unit, 0*m_unit, -1, 0, 0,texture_id,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 *m_unit, (0 + 1)*m_unit, -1, 0, 0,texture_id,
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1) *m_unit, 0*m_unit, -1, 0, 0,texture_id,
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 *m_unit, 0*m_unit, -1, 0, 0,texture_id,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

float* SimpleBlock::CreateModel(float* target, int world_x, int world_y, int world_z)
{
	if (face_visible == 0)
		return target;

	int side_id = 0, top_id = 0;
	switch (id)
	{
	case blk_id::dirt_id:
		side_id = top_id = tex_id::dirt;
		break;
	case blk_id::stone_id:
		side_id = top_id = tex_id::stone;
		break;
	case blk_id::wood_id:
		side_id = tex_id::wood_side;
		top_id = tex_id::wood_top;
		break;
	case blk_id::grass_id:
		side_id = tex_id::grass_side;
		top_id = tex_id::grass_top;
		break;
	case blk_id::leaves_id:
		side_id = top_id = tex_id::leaves;
		break;
	case blk_id::water_id:
		if (GetFaceVisible(TOP))
			return CreateTopFace(target, world_x, world_y, world_z, tex_id::water);
		else
			return target;
		break;
	case blk_id::air_id:
	default:
		return target;
	}
	if (GetFaceVisible(NORTH))
	{
		target = CreateNorthFace(target, world_x, world_y, world_z, side_id);
	}
	if (GetFaceVisible(SOUTH))
	{
		target = CreateSouthFace(target, world_x, world_y, world_z, side_id);
	}
	if (GetFaceVisible(WEST))
	{
		target = CreateWestFace(target, world_x, world_y, world_z, side_id);
	}
	if (GetFaceVisible(EAST))
	{
		target = CreateEastFace(target, world_x, world_y, world_z, side_id);
	}
	if (GetFaceVisible(TOP))
	{
		target = CreateTopFace(target, world_x, world_y, world_z, top_id);
	}
	if (GetFaceVisible(BOTTOM))
	{
		target = CreateBottomFace(target, world_x, world_y, world_z, top_id);
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

SimpleBlock* SimpleBlock::CreateNew(int block_id, HitInfo hit_info = HitInfo())
{
	switch (block_id)
	{
	case blk_id::torch_id:
		return new blk::Torch(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z));

	default:
		return new SimpleBlock(block_id);
	}
}

SimpleBlock* SimpleBlock::CreateNew(int block_id)
{
	return CreateNew(block_id, HitInfo());
}

glm::vec3 SimpleBlock::GetColor(int block_id)
{
	switch (block_id)
	{
	case blk_id::dirt_id:
	case blk_id::grass_id:
		return glm::vec3(120.0, 85, 58)/255.0f;
	case blk_id::stone_id:
		return glm::vec3(116, 116, 116) / 255.0f;

	case blk_id::wood_id:
		return glm::vec3(104, 83, 50) / 255.0f;
	case blk_id::leaves_id:
		return glm::vec3(80, 144, 38) / 255.0f;
	case blk_id::water_id:
		return glm::vec3(0, 0, 1);
	case blk_id::air_id:
	default:
		return glm::vec3(0,0,0);
	}
}

SimpleBlock::Direction SimpleBlock::GetDirection(glm::ivec3 vec)
{
	if (vec.x >= 1)
		return EAST;
	if (vec.x <= -1)
		return WEST;
	if (vec.z <= -1)
		return SOUTH;
	if (vec.z >= 1)
		return NORTH;
	if (vec.y >= 1)
		return TOP;
	if (vec.y <= -1)
		return BOTTOM;
}
