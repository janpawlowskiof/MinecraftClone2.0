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
	return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y, block_z), glm::vec3(1,1,1), hit_info);
	/*float hit_x, hit_y, hit_z;
	if (direction.x > 0 && ProjectRayOnPlaneYZ(block_x, hit_y, hit_z, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_z, block_z, block_z + 1))
	{
		hit_info.Update(block_x, block_y, block_z, block_x - 1, block_y, block_z, glm::length(origin - glm::vec3(block_x, hit_y, hit_z)));
		return true;
	}
	if (direction.x < 0 && ProjectRayOnPlaneYZ(block_x + 1, hit_y, hit_z, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_z, block_z, block_z + 1))
	{
		hit_info.Update(block_x, block_y, block_z, block_x + 1, block_y, block_z, glm::length(origin - glm::vec3(block_x + 1, hit_y, hit_z)));
		return true;
	}
	if (direction.z > 0 && ProjectRayOnPlaneXY(block_z, hit_x, hit_y, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_x, block_x, block_x + 1))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y, block_z - 1, glm::length(origin - glm::vec3(hit_x, hit_y, block_z)));
		return true;
	}
	if (direction.z < 0 && ProjectRayOnPlaneXY(block_z + 1, hit_x, hit_y, origin, direction) && between(hit_y, block_y, block_y + 1) && between(hit_x, block_x, block_x + 1))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y, block_z + 1, glm::length(origin - glm::vec3(hit_x, hit_y, block_z + 1)));
		return true;
	}
	if (direction.y > 0 && ProjectRayOnPlaneXZ(block_y, hit_x, hit_z, origin, direction) && between(hit_z, block_z, block_z + 1) && between(hit_x, block_x, block_x + 1))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y - 1, block_z, glm::length(origin - glm::vec3(hit_x, block_y, hit_z)));
		return true;
	}
	if (direction.y < 0 && ProjectRayOnPlaneXZ(block_y + 1, hit_x, hit_z, origin, direction) && between(hit_z, block_z, block_z + 1) && between(hit_x, block_x, block_x + 1))
	{
		hit_info.Update(block_x, block_y, block_z, block_x, block_y + 1, block_z, glm::length(origin - glm::vec3(hit_x, block_y + 1, hit_z)));
		return true;
	}
	return false;*/
}

bool SimpleBlock::CheckRayVsAABB(glm::vec3 origin, glm::vec3 direction, glm::vec3 position, glm::vec3 dimentions, HitInfo& hit_info)
{
	float hit_x, hit_y, hit_z;
	if (direction.x > 0 && ProjectRayOnPlaneYZ(position.x, hit_y, hit_z, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_z, position.z, position.z + dimentions.z))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x) - 1, floor(position.y), floor(position.z), glm::length(origin - glm::vec3(position.x, hit_y, hit_z)));
		return true;
	}
	if (direction.x < 0 && ProjectRayOnPlaneYZ(position.x + dimentions.x, hit_y, hit_z, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_z, position.z, position.z + dimentions.z))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x) + 1, floor(position.y), floor(position.z), glm::length(origin - glm::vec3(position.x + dimentions.x, hit_y, hit_z)));
		return true;
	}
	if (direction.z > 0 && ProjectRayOnPlaneXY(position.z, hit_x, hit_y, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y), floor(position.z) - 1, glm::length(origin - glm::vec3(hit_x, hit_y, position.z)));
		return true;
	}
	if (direction.z < 0 && ProjectRayOnPlaneXY(position.z + dimentions.z, hit_x, hit_y, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y), floor(position.z) + 1, glm::length(origin - glm::vec3(hit_x, hit_y, position.z + dimentions.z)));
		return true;
	}
	if (direction.y > 0 && ProjectRayOnPlaneXZ(position.y, hit_x, hit_z, origin, direction) && between(hit_z, position.z, position.z + dimentions.z) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y) - 1, floor(position.z), glm::length(origin - glm::vec3(hit_x, position.y, hit_z)));
		return true;
	}
	if (direction.y < 0 && ProjectRayOnPlaneXZ(position.y + dimentions.y, hit_x, hit_z, origin, direction) && between(hit_z, position.z, position.z + dimentions.z) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y) + 1, floor(position.z), glm::length(origin - glm::vec3(hit_x, position.y + dimentions.y, hit_z)));
		return true;
	}
	return false;
}

SimpleBlock* SimpleBlock::CreateNew(int block_id, HitInfo hit_info = HitInfo())
{
	switch (block_id)
	{
	case blk_id::torch_id:
		return new blk::Torch(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), hit_info.place_chunk);
	case blk_id::switch_id:
		return new blk::Switch(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), hit_info.place_chunk);
	case blk_id::door_id:
		return new blk::Door(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), hit_info.place_chunk);
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

	return NORTH;
}

bool SimpleBlock::ProjectRayOnPlaneXZ(float plane_y, float& hit_x, float& hit_z, glm::vec3 origin, glm::vec3 direction)
{
		if (glm::dot(direction, glm::vec3(0, 1, 0)) == 0)
			return false;

		hit_x = direction.x / direction.y * (plane_y - origin.y) + origin.x;
		hit_z = direction.z / direction.y * (plane_y - origin.y) + origin.z;
		return true;
}

bool SimpleBlock::ProjectRayOnPlaneXY(float plane_z, float& hit_x, float& hit_y, glm::vec3 origin, glm::vec3 direction)
{
	if (glm::dot(direction, glm::vec3(0, 0, 1)) == 0)
		return false;

	hit_x = direction.x / direction.z * (plane_z - origin.z) + origin.x;
	hit_y = direction.y / direction.z * (plane_z - origin.z) + origin.y;
	return true;
}

bool SimpleBlock::ProjectRayOnPlaneYZ(float plane_x, float& hit_y, float& hit_z, glm::vec3 origin, glm::vec3 direction)
{
	if (glm::dot(direction, glm::vec3(1, 0, 0)) == 0)
		return false;

	hit_y = direction.y / direction.x * (plane_x - origin.x) + origin.y;
	hit_z = direction.z / direction.x * (plane_x - origin.x) + origin.z;

	return true;
}
