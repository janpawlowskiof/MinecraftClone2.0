#pragma once
#include "SimpleBlock.h"
#include <iostream>

#define xyz(n) n.x, n.y, n.z

class ComplexBlock : public SimpleBlock
{
public:
	ComplexBlock(glm::vec3 position)
	{
		SetFlag(OPAQUE, false);
		SetFlag(COMPLEX, true);
		this->position = position;
	}
	Direction direction;
	glm::ivec3 parent_position;
	glm::ivec3 position;

	virtual ~ComplexBlock();
	virtual float* CreateModel(float* target, int world_x, int world_y, int world_z) = 0;
	virtual int GetNumberOfTriangles() = 0;
	virtual bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info) = 0;
	//virtual void OnPlayerClick() = 0;
};

namespace blk
{
	class Torch : public ComplexBlock
	{
	public:
		Torch(glm::ivec3 position, glm::ivec3 parent_position) : ComplexBlock(position)
		{
			id = blk_id::torch_id;
			this->parent_position = parent_position;

			direction = SimpleBlock::GetDirection(parent_position - position);
		}

		int GetNumberOfTriangles() override
		{
			return 8;
		}

		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info) override
		{
			return false;
		}


		float* CreateModel(float* target, int world_x, int world_y, int world_z) override;
		~Torch() override
		{
			std::cout << "Deleting torch\n";
		}
	};
	/*
	class Switch : public ComplexBlock
	{
	public:
		Direction direction = BOTTOM;

		Switch() { id = blk_id::switch_id; }

		int GetNumberOfTriangles() override
		{
			return 20;
		}


		float* CreateModel(float* target, int world_x, int world_y, int world_z) override
		{
			const float base_width = 1.0f / 16.0f;
			const float button_width = 1.0f / 16.0f;
			const float vertices[] = {
			//base
			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f + base_width, 0 , (1) , 0, 0, 1, tex_id::torch_side,
			world_x + 1.0f, world_y + 0.0f, world_z + 0.5f + base_width, (1) , (1) , 0, 0, 1,tex_id::torch_side,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f + base_width, (1) , 0 , 0, 0, 1,tex_id::torch_side,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f + base_width, 0 , (1) , 0, 0, 1,tex_id::torch_side,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f + base_width, (1) , 0 , 0, 0, 1,tex_id::torch_side,
			world_x + 0.0f, world_y + 1.0f, world_z + 0.5f + base_width, 0 , 0 , 0, 0, 1,tex_id::torch_side,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f - base_width, 0 , (1) , 0, 0, -1,tex_id::torch_side,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f - base_width, (1) , 0 , 0, 0, -1,tex_id::torch_side,
			world_x + 1.0f, world_y + 0.0f, world_z + 0.5f - base_width, (1) , (1) , 0, 0, -1,tex_id::torch_side,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f - base_width, 0 , (1) , 0, 0, -1,tex_id::torch_side,
			world_x + 0.0f, world_y + 1.0f, world_z + 0.5f - base_width, 0 , 0 , 0, 0, -1,tex_id::torch_side,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f - base_width, (1) , 0 , 0, 0, -1,tex_id::torch_side,

			world_x + 0.5f + base_width, world_y + 0.0f, world_z + 0.0f, 0 , (1) , 1, 0, 0,tex_id::torch_side,
			world_x + 0.5f + base_width, world_y + 1.0f, world_z + 1.0f, (1) , 0 , 1, 0, 0,tex_id::torch_side,
			world_x + 0.5f + base_width, world_y + 0.0f, world_z + 1.0f, (1) , (1) , 1, 0, 0,tex_id::torch_side,

			world_x + 0.5f + base_width, world_y + 0.0f, world_z + 0.0f, 0 , (1) , 1, 0, 0,tex_id::torch_side,
			world_x + 0.5f + base_width, world_y + 1.0f, world_z + 0.0f, 0 , 0 , 1, 0, 0,tex_id::torch_side,
			world_x + 0.5f + base_width, world_y + 1.0f, world_z + 1.0f, (1) , 0 , 1, 0, 0,tex_id::torch_side,

			world_x + 0.5f - base_width, world_y + 0.0f, world_z + 0.0f, 0 , (1) , -1, 0, 0,tex_id::torch_side,
			world_x + 0.5f - base_width, world_y + 0.0f, world_z + 1.0f, (1) , (1) , -1, 0, 0,tex_id::torch_side,
			world_x + 0.5f - base_width, world_y + 1.0f, 1 + world_z, (1) , 0 , -1, 0, 0,tex_id::torch_side,

			world_x + 0.5f - base_width, world_y + 0.0f, world_z + 0.0f, 0 , (1) , -1, 0, 0,tex_id::torch_side,
			world_x + 0.5f - base_width, world_y + 1.0f, world_z + 1.0f, (1) , 0 , -1, 0, 0,tex_id::torch_side,
			world_x + 0.5f - base_width, world_y + 1.0f, world_z + 0.0f, 0 , 0 , -1, 0, 0,tex_id::torch_side,
			};
			std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
			return target + sizeof(vertices) / sizeof(float);
		}
	};
	*/
};