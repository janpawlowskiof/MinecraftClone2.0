#pragma once
#include <glm\glm.hpp>
class SimpleBlock
{
public:
	unsigned char id = 255;
	unsigned char face_visible = 63;
	unsigned char flags = 254;
	unsigned char power_level = 0;
	unsigned char light_level = 0;

	enum Direction
	{
		//	z+
		NORTH = 1,
		//	z-
		SOUTH = 2,
		//	x+
		EAST = 4,
		//	x-
		WEST = 8,
		//	y+
		TOP = 16,
		//	y-
		BOTTOM = 32
	};

	enum Flag
	{
		COMPLEX = 1,
		OPAQUE = 2,
	};

	SimpleBlock(unsigned char id = 255);
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

	static bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z);

	//static float* CreateModel(
};

namespace blk_id
{
	enum 
	{
		air_id,
		dirt_id,
		stone_id,

		water_id,
		torch_id,
	};
}
//bo textura ma 16x16 bloków
#define m_unit (1.0f/16.0f)