#pragma once
#include <glm/glm.hpp>
#include "SimpleBlock.h"
//#include "Chunk.h"
//#include "SimpleBlock.h"

class Chunk;
class SimpleBlock;

struct HitInfo
{
	int hit_x = 0;
	int hit_y = 0;
	int hit_z = 0;

	int place_x = 0;
	int place_y = 0;
	int place_z = 0;

	float distance = 100000;
	Chunk* hit_chunk;
	Chunk* place_chunk;
	SimpleBlock* hit_block;
	SimpleBlock* place_block;

	void Update(int hit_x, int hit_y, int hit_z, int place_x, int place_y, int place_z, float distance)
	{
		this->hit_x = hit_x;
		this->hit_y = hit_y;
		this->hit_z = hit_z;

		this->place_x = place_x;
		this->place_y = place_y;
		this->place_z = place_z;

		this->distance = distance;

	}

	void Update(Chunk* hit_chunk, SimpleBlock* hit_block, Chunk* place_chunk, SimpleBlock* place_block)
	{
		this->hit_chunk = hit_chunk;
		this->place_chunk = place_chunk;
		this->hit_block = hit_block;
		this->place_block = place_block;
	}

};

