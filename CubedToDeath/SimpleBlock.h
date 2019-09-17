#pragma once
#include <glm\glm.hpp>
#include <fstream>
#include "Vertex.h"
#include <vector>
//#include "HitInfo.h"

class RayHitInfo;
class Chunk;

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
		POWERABLE = 8,
	};

	SimpleBlock(unsigned char id = 0);
	~SimpleBlock();


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

	void CreateSolidModel(std::vector<Vertex> &vertices, int world_x, int world_y, int world_z, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, int x_count, int z_count);
	float* CreateFluidModel(float* target, int world_x, int world_y, int world_z, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11);
	static bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info);
	static bool CheckRayVsAABB(glm::vec3 origin, glm::vec3 direction, glm::vec3 position, glm::vec3 dimentions,RayHitInfo& hit_info);
	static bool CheckCylinderVsAABB(glm::vec3 cylinder_center, float cylinder_radius, float cylinder_height, glm::vec3 aabb_center, glm::vec3 aabb_helf_extends, glm::vec3& closest_point);
	static float ResolveCollisionVerticaly(glm::vec3 block_center, glm::vec3 hitbox_center, float hitbox_height);
	static glm::vec3 ResolveCollision(glm::vec3 block_center, glm::vec3 hitbox_center, float hitbox_radius, float hitbox_height);
	static SimpleBlock* CreateNew(int block_id, RayHitInfo hit_info);
	static SimpleBlock* CreateNew(int block_id);
	static glm::vec3 GetColor(int block_id);
	static Direction GetDirection(glm::ivec3 vec);
	//static Direction GetOposite(Direction direction);
	static glm::ivec3 GetOffset(Direction direction);
	static bool ProjectRayOnPlaneXZ(float plane_y, float& hit_x, float& hit_z, glm::vec3 origin, glm::vec3 direction);
	static bool ProjectRayOnPlaneXY(float plane_z, float& hit_x, float& hit_y, glm::vec3 origin, glm::vec3 direction);
	static bool ProjectRayOnPlaneYZ(float plane_x, float& hit_y, float& hit_z, glm::vec3 origin, glm::vec3 direction);
	static void SaveBlockToFile(SimpleBlock* block, std::ofstream& save_file);
	static char* SaveBlockToFile(SimpleBlock* block, char* data);
	static SimpleBlock* LoadBlockFromFile(glm::ivec3 position, Chunk* parent_chunk, char*& data);
	void RecalculatePowerLevel(glm::ivec3 local_position, Chunk* parent_chunk);
};

namespace blk_id
{
	enum 
	{
		air_id,
		dirt_id,
		stone_id,
		cobblestone_id,
		wood_id,
		grass_id,
		leaves_id,
		planks_id,
		gold_ore_id,
		iron_block_id,
		torch_id,
		redstone_torch_id,
		switch_id,
		redstone_id,
		redstone_block_id,
		water_id,
		door_id,
	};
}

namespace tex_id
{
	enum
	{
		dirt,
		dirt_n,
		stone,
		stone_n,
		cobblestone,
		cobblestone_n,
		wood_side,
		wood_side_n,
		wood_side_s,
		wood_top,
		wood_top_n,
		wood_top_s,
		planks,
		planks_n,
		planks_s,
		gold_ore,
		gold_ore_n,
		gold_ore_s,
		iron_block,
		iron_block_n,
		iron_block_s,
		grass_side,
		grass_side_n,
		grass_side_overlay,
		grass_side_overlay_n,
		grass_top,
		grass_top_n,
		leaves,
		leaves_n,
		torch,
		torch_n,
		torch_s,
		redstone_torch_on,
		redstone_torch_on_n,
		redstone_torch_on_s,
		redstone_torch_off,
		redstone_torch_off_n,
		redstone,
		redstone_dot,
		redstone_block,
		redstone_block_n,
		redstone_block_s,
		lever_base,
		lever_base_top,
		water,
		door_top,
		door_top_n,
		door_bottom,
		door_bottom_n,
	};
}
#define m_unit (1.0f)
inline bool between(float n, float a, float b) { return (n >= a && n <= b)||(n >= b && n <= a); }