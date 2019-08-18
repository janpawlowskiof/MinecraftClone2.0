#pragma once
#include <glm\glm.hpp>
#include <fstream>
//#include "HitInfo.h"

class HitInfo;

class SimpleBlock
{
public:
	unsigned char id = 0;
	unsigned char face_visible = 63;
	unsigned char flags = 252;
	unsigned char power_level = 0;
	unsigned char light_level = 0;

	enum Direction
	{
		//	z+
		NORTH = 1,
		//	z-
		SOUTH = 2,
		//	x-
		EAST = 4,
		//	x+
		WEST = 8,
		//	y+
		TOP = 16,
		//	y-
		BOTTOM = 32
	};

	enum Flag
	{
		COMPLEX = 1,
		FLUID = 2,
		OPAQUE = 4,
	};

	SimpleBlock(unsigned char id = 0);
	~SimpleBlock();


	float* CreateModel(float* target, int world_x, int world_y, int world_z);
	//returns if given face is currently visible
	bool GetFaceVisible(Direction direction)
	{
		return ((face_visible & direction) > 0);
	}
	//sets if given face should be visible or not
	void SetFaceVisible(Direction direction, bool value)
	{
		if (value) face_visible |= direction; else face_visible &= ~direction;
	}
	bool GetFlag(Flag flag)
	{
		return ((flags & flag) > 0);
	}
	void SetFlag(Flag flag, bool value)
	{
		if (value) flags |= flag; else flags &= ~flag;
	}

	static bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info);
	static bool CheckRayVsAABB(glm::vec3 origin, glm::vec3 direction, glm::vec3 position, glm::vec3 dimentions,HitInfo& hit_info);
	static SimpleBlock* CreateNew(int block_id, HitInfo hit_info);
	static SimpleBlock* CreateNew(int block_id);
	static glm::vec3 GetColor(int block_id);
	static Direction GetDirection(glm::ivec3 vec);
	static bool ProjectRayOnPlaneXZ(float plane_y, float& hit_x, float& hit_z, glm::vec3 origin, glm::vec3 direction);
	static bool ProjectRayOnPlaneXY(float plane_z, float& hit_x, float& hit_y, glm::vec3 origin, glm::vec3 direction);
	static bool ProjectRayOnPlaneYZ(float plane_x, float& hit_y, float& hit_z, glm::vec3 origin, glm::vec3 direction);
	static void SaveBlockToFile(SimpleBlock* block, std::ofstream& save_file);
	static char* SaveBlockToFile(SimpleBlock* block, char* data);
	static SimpleBlock* LoadBlockFromFile(std::ifstream& save_file);
	static SimpleBlock* LoadBlockFromFile(char*& data);
};

namespace blk_id
{
	enum 
	{
		air_id,
		dirt_id,
		stone_id,
		wood_id,
		grass_id,
		leaves_id,
		torch_id,
		switch_id,
		water_id,
		door_id,
	};
}

namespace tex_id
{
	enum
	{
		dirt,
		stone,
		wood_side,
		wood_top,
		grass_side,
		grass_top,
		leaves,
		torch_side,
		water,
		door_top,
		door_bottom,
	};
}
#define m_unit (1.0f)
inline bool between(float n, float a, float b) { return (n >= a && n <= b)||(n >= b && n <= a); }