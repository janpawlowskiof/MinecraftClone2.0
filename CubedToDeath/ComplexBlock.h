#pragma once
#include "SimpleBlock.h"
#include <iostream>
#include "Chunk.h"
#include "Player.h"

#define xyz(n) n.x, n.y, n.z

class ComplexBlock : public SimpleBlock
{
public:
	ComplexBlock(glm::vec3 position, Chunk* parent_chunk)
	{
		SetFlag(OPAQUE, false);
		SetFlag(COMPLEX, true);
		this->position = position;
		this->parent_chunk = parent_chunk;
	}
	Direction direction;
	glm::ivec3 position;
	glm::ivec3 parent_position;
	Chunk* parent_chunk;

	virtual ~ComplexBlock();
	virtual float* CreateModel(float* target, int world_x, int world_y, int world_z) = 0;
	virtual int GetNumberOfTriangles() = 0;
	virtual bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info) = 0;
	virtual void OnTick() { std::cout << "Default on tick\n"; };
	virtual void OnPlayerClick() { std::cout << "Default on click\n"; }
};

namespace blk
{
	class Torch : public ComplexBlock
	{
	public:
		Torch(glm::ivec3 position, glm::ivec3 parent_position, Chunk* parent_chunk) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::torch_id;
			this->parent_position = parent_position;

			direction = SimpleBlock::GetDirection(parent_position - position);
		}

		int GetNumberOfTriangles() override
		{
			return 8;
		}
		void OnPlayerClick() override
		{
			std::cout << "TorchClick\n";
		}

		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info) override;
		float* CreateModel(float* target, int world_x, int world_y, int world_z) override;
		~Torch() override
		{
			std::cout << "Deleting torch\n";
		}
	};

	class Switch: public ComplexBlock
	{
	public:
		Switch(glm::ivec3 position, glm::ivec3 parent_position, Chunk* parent_chunk) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::switch_id;
			this->parent_position = parent_position;
			direction = SimpleBlock::GetDirection(parent_position - position);
		}

		int GetNumberOfTriangles() override
		{
			return 20;
		}
		void OnPlayerClick() override
		{
			std::cout << "SwitchClick\n";
			turned_on = !turned_on;
			parent_chunk->RecalculateComplexVbo();
		}

		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info) override;
		float* CreateModel(float* target, int world_x, int world_y, int world_z) override;
		~Switch() override
		{
			std::cout << "Deleting switch\n";
		}

	private:
		bool turned_on = false;
		const float width_base = 5.0f / 16.0f;
		const float height_base = 0.2;

		const float width_button = 2.0f / 16.0f;
		const float height_button = 0.3;
	};

	class Door : public ComplexBlock
	{
	public:
		Door(glm::ivec3 position, glm::ivec3 parent_position, Chunk* parent_chunk, bool top = false) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::door_id;
			this->parent_position = parent_position;
			this->parent_chunk = parent_chunk;
			direction = SimpleBlock::GetDirection(glm::ivec3(Player::position) - position);
			if (direction == BOTTOM || direction == TOP)
				direction = NORTH;
			this->other = other;
			this->top = top;

			if (!top)
			{
				other = new Door(glm::ivec3(position.x, position.y + 1, position.z), position, parent_chunk, true);
				parent_chunk->ReplaceBlock(position.x, position.y + 1, position.z, other);
				other->other = this;
			}
		}

		int GetNumberOfTriangles() override
		{
			return 12;
		}
		void OnPlayerClick() override
		{
			std::cout << "DoorClick\n";
			if (direction == NORTH)
				direction = EAST;
			else if (direction == EAST)
				direction = SOUTH;
			else if (direction == SOUTH)
				direction = WEST;
			else if (direction == WEST)
				direction = NORTH;

			other->direction = direction;
			parent_chunk->RecalculateComplexVbo();
		}

		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, HitInfo& hit_info) override;
		float* CreateModel(float* target, int world_x, int world_y, int world_z) override;
		~Door() override
		{
			std::cout << "Deleting door\n";
		}

		Door* other;
	private:
		const float depth = 2.0f / 16.0f;
		bool top = true;
	};
};