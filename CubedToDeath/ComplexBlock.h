#pragma once
#include "SimpleBlock.h"
#include <iostream>
#include "Chunk.h"
#include "Player.h"
#include "Models.h"
#include <algorithm>

#define xyz(n) n.x, n.y, n.z

class ComplexBlock : public SimpleBlock
{
public:
	ComplexBlock(glm::vec3 position, Chunk* parent_chunk)
	{
		SetFlag(OPAQUE, false);
		SetFlag(COMPLEX, true);
		SetFlag(POWERABLE, false);
		this->position = position;
		this->parent_chunk = parent_chunk;
		parent_chunk->complex_block_count++;
	}
	Direction direction;
	glm::ivec3 position;
	//glm::ivec3 parent_position;
	Chunk* parent_chunk;
	bool model_changed = false;

	virtual ~ComplexBlock();
	virtual void CreateModel(std::vector<Vertex>&, int world_x, int world_y, int world_z) = 0;
	virtual int GetNumberOfTriangles() = 0;
	virtual bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info) = 0;
	virtual void OnTick() { /*std::cout << "Default on tick\n";*/ };
	//virtual void RecalculatePowerLevel() {};
	virtual void OnPlayerClick() { std::cout << "Default on click\n"; }
	virtual void OnDestroy() { std::cout << "Default on destroy\n"; }
	virtual int GetBlockSpecificMetaSize() { return 1; }
	virtual void OnLoadFinalization() { }
	virtual void OnNeighbourDestroyed(glm::ivec3 neighbour_position) { std::cout << "Default ond\n"; if(neighbour_position == position - glm::ivec3(0, 1, 0)) SelfDestruct(); }
	virtual unsigned char GetPowerTowards(Direction direction) { return 0; }
	virtual void WriteBlockSpecificMeta(char*& save_data_pointer)
	{
		save_data_pointer[0] = (char)direction;
		save_data_pointer++;
	}
	virtual void SelfDestruct()
	{
		std::cout << "Default SelfDestruct\n";
		parent_chunk->ReplaceBlock(position.x, position.y, position.z, SimpleBlock::CreateNew(blk_id::air_id));
	}
	void RecalculateNeightboursPowerLevel(bool ignore_redstone = false)
	{
		glm::ivec3 local_position(position.x - 16 * parent_chunk->chunk_x, position.y, position.z - 16 * parent_chunk->chunk_z);

		for (int i = 1; i <= 32; i*=2)
		{
			glm::ivec3 neightbour_position = local_position + GetOffset((Direction)i);
			Chunk* chunk;
			const auto block = parent_chunk->GetBlockInArea(neightbour_position.x, neightbour_position.y, neightbour_position.z, chunk);
			if(block != nullptr)
				block->RecalculatePowerLevel(neightbour_position, chunk, ignore_redstone);
		}
	}
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
		Torch(glm::ivec3 position, Chunk* parent_chunk, char*& save_data_pointer) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::torch_id;
			direction = (SimpleBlock::Direction)save_data_pointer[0];
			parent_position = position + SimpleBlock::GetOffset(direction);
			save_data_pointer++;
			//std::cout << "t: " << parent_position.x << " " << parent_position.y << " " << parent_position.z << "\n";
		}
		int GetNumberOfTriangles() override
		{
			return 8;
		}
		void OnPlayerClick() override
		{
			std::cout << "TorchClick\n";
		}
		void OnNeighbourDestroyed(glm::ivec3 neighbour_position) override
		{
			if (neighbour_position == parent_position) SelfDestruct();
		}
		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info) override;
		void CreateModel(std::vector<Vertex>&vertices, int world_x, int world_y, int world_z) override;
		~Torch() override
		{
			std::cout << "Unloading torch\n";
		}
	protected:
		glm::ivec3 parent_position;
	};

	class RedstoneTorch : public Torch
	{
	public:
		RedstoneTorch(glm::ivec3 position, glm::ivec3 parent_position, Chunk* parent_chunk) : Torch(position, parent_position, parent_chunk)
		{
			id = blk_id::redstone_torch_id;
		}
		RedstoneTorch(glm::ivec3 position, Chunk* parent_chunk, char*& save_data_pointer) : Torch(position, parent_chunk, save_data_pointer)
		{
			//std::cout << "r: " << parent_position.x << " " << parent_position.y << " " << parent_position.z << "\n";
			id = blk_id::redstone_torch_id;
		}
		void CreateModel(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z) override;
		void OnTick() override
		{
			glm::ivec3 parent_local_position(parent_position.x - 16 * parent_chunk->chunk_x, parent_position.y, parent_position.z-16 * parent_chunk->chunk_z);
			//std::cout << "PLP: " << parent_local_position.x << " " << parent_local_position.y << " " << parent_local_position.z << std::endl;
			unsigned char parent_power = parent_chunk->GetBlockInArea(parent_local_position)->power_level;
			//std::cout << "PPL: " << (int)parent_power << std::endl;
			if ((parent_power-0.5)*(power_level-0.5) > 0)
			{
				ticks_left_to_toggle--;
				std::cout << "Ticks left to toggle " << (int)ticks_left_to_toggle << "\n";
				if (ticks_left_to_toggle < 0)
				{
					power_level = parent_power > 0 ? 0 : 16;
					model_changed = true;
					ticks_left_to_toggle = ticks_to_toggle;
					std::cout << "Toggled!";
				}

				RecalculateNeightboursPowerLevel(true);
				//std::cout << "TPL: " << (int)power_level << std::endl;
			}
			else
			{
				ticks_left_to_toggle = ticks_to_toggle;
			}
		}
		unsigned char GetPowerTowards(Direction direction) override
		{
			if (power_level == 0)
				return 0;
			return ((direction==this->direction) ? 0 : 16);
		}
	private:
		char ticks_left_to_toggle = 0;
		const char ticks_to_toggle = 2;
	};


	class Switch: public ComplexBlock
	{
	public:
		Switch(glm::ivec3 position, glm::ivec3 parent_position, Chunk* parent_chunk) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::switch_id;
			//this->parent_position = parent_position;
			direction = SimpleBlock::GetDirection(parent_position - position);
			this->parent_position = parent_position;
		}
		Switch(glm::ivec3 position, Chunk* parent_chunk, char*& save_data_pointer) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::switch_id;
			direction = (SimpleBlock::Direction)save_data_pointer[0];
			parent_position = position + SimpleBlock::GetOffset(direction);
			turned_on = (bool)save_data_pointer[1];
			save_data_pointer+=2;
		}
		int GetBlockSpecificMetaSize() override { return 2; }
		void WriteBlockSpecificMeta(char*& save_data_pointer) override
		{
			save_data_pointer[0] = (char)direction;
			save_data_pointer[1] = (char)turned_on;
			save_data_pointer+=2;
		}
		int GetNumberOfTriangles() override
		{
			return 10;
		}
		void OnPlayerClick() override
		{
			std::cout << "SwitchClick\n";
			parent_chunk->save_needed = true;

			turned_on = !turned_on;
			parent_chunk->RecalculateComplexVbo();
		}
		void OnNeighbourDestroyed(glm::ivec3 neighbour_position) override
		{
			if (neighbour_position == parent_position) SelfDestruct();
		}
		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info) override;
		void CreateModel(std::vector<Vertex>&, int world_x, int world_y, int world_z) override;
		~Switch() override
		{
			std::cout << "Unloading switch\n";
		}

	private:
		bool turned_on = false;
		const float width_base = 5.0f / 16.0f;
		const float height_base = 0.2;

		const float width_button = 2.0f / 16.0f;
		const float height_button = 0.3;
		glm::ivec3 parent_position;
	};

	class RedstoneBlock: public ComplexBlock
	{
	public:
		RedstoneBlock(glm::ivec3 position, Chunk* parent_chunk) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::redstone_block_id;
		}
		RedstoneBlock(glm::ivec3 position, Chunk* parent_chunk, char*& save_data_pointer) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::redstone_block_id;
		}
		int GetBlockSpecificMetaSize() override { return 0; }
		unsigned char GetPowerTowards(Direction direction) override { return 16; }

		int GetNumberOfTriangles() override
		{
			return 12;
		}
		void WriteBlockSpecificMeta(char*& save_data_pointer)override{}
		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info) override;
		void CreateModel(std::vector<Vertex>&, int world_x, int world_y, int world_z) override;
		void OnNeighbourDestroyed(glm::ivec3 neighbour_position) override
		{
			RecalculateNeightboursPowerLevel(true);
		}
		void OnDestroy() override
		{
			RecalculateNeightboursPowerLevel(true);
		}
	private:
	};


	class Redstone : public ComplexBlock
	{
	public:
		Redstone(glm::ivec3 position, Chunk* parent_chunk) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::redstone_id;
			SetFlag(POWERABLE,true);
			CheckLines();
			RecalculatePowerLevel(glm::ivec3(position.x - 16 * parent_chunk->chunk_x, position.y, position.z - 16 * parent_chunk->chunk_z), parent_chunk);

		}
		Redstone(glm::ivec3 position, Chunk* parent_chunk, char*& save_data_pointer) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::redstone_id;
			SetFlag(POWERABLE, true);
		}
		int GetBlockSpecificMetaSize() override { return 0; }
		void WriteBlockSpecificMeta(char*& save_data_pointer) override{}
		int GetNumberOfTriangles() override
		{
			return 2 * ((north_line!=nullptr) + (south_line!=nullptr) + (west_line!=nullptr) + (east_line!=nullptr) + 1);
		}
		void OnPlayerClick() override
		{
			std::cout << "RedstoneClick\n";
		}
		void OnLoadFinalization() override
		{ 
			CheckLines();
		}
		void OnTick() override
		{
			//power_level = 0;
		}
		void PropagetePower(unsigned char initial_power)
		{
			if (initial_power <= power_level || initial_power <= 0)
				return;


			power_level = initial_power;
			const auto lowered_power = power_level - 1;

			//std::cout << "Power of "<< (int)power_level <<" propagated...\n";

			if (north_line)
				north_line->PropagetePower(lowered_power);
			if (south_line)
				south_line->PropagetePower(lowered_power);
			if (west_line)
				west_line->PropagetePower(lowered_power);
			if (east_line)
				east_line->PropagetePower(lowered_power);
		}
		void PropagetePower()
		{
			RecalculatePowerLevel(glm::ivec3(position.x - 16*parent_chunk->chunk_x, position.y, position.z - 16*parent_chunk->chunk_z), parent_chunk);
			if (north_line && north_line->power_level > 1)
				power_level = std::max(power_level, (unsigned char)(north_line->power_level - 1));
			if (south_line && south_line->power_level > 1)
				power_level = std::max(power_level, (unsigned char)(south_line->power_level - 1));
			if (west_line && west_line->power_level > 1)
				power_level = std::max(power_level, (unsigned char)(west_line->power_level - 1));
			if (east_line && east_line->power_level > 1)
				power_level = std::max(power_level, (unsigned char)(east_line->power_level - 1));

			//std::cout << "Propagation started at " << (int)power_level << "\n";

			if (power_level <= 1)
				return;

			const auto lowered_power = power_level - 1;

			if (north_line)
				north_line->PropagetePower(lowered_power);
			if (south_line)
				south_line->PropagetePower(lowered_power);
			if (west_line)
				west_line->PropagetePower(lowered_power);
			if (east_line)
				east_line->PropagetePower(lowered_power);
		}
		unsigned char GetPowerTowards(Direction direction) override
		{
			if (power_level <= 0)
				return 0;

			if ((direction == NORTH || direction == SOUTH) && (north_line || south_line))
				return std::max(power_level - 1, 0);
			if ((direction == EAST|| direction == WEST) && (west_line || east_line))
				return std::max(power_level - 1, 0);
			if(direction == BOTTOM)
				return std::max(power_level - 1, 0);
			return 0;
		}

		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info) override;
		void CreateModel(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z) override;
		void OnDestroy() override
		{
			std::cout << "Redstone on destroy\n";

			if (north_line)
			{
				north_line->south_line = nullptr;
				if (north_line->parent_chunk != parent_chunk)
					north_line->parent_chunk->RecalculateComplexVbo();
			}
			if (south_line)
			{
				south_line->north_line = nullptr;
				if (south_line->parent_chunk != parent_chunk)
					south_line->parent_chunk->RecalculateComplexVbo();
			}
			if (west_line)
			{
				west_line->east_line = nullptr;
				if (west_line->parent_chunk != parent_chunk)
					west_line->parent_chunk->RecalculateComplexVbo();
			}
			if (east_line)
			{
				east_line->west_line = nullptr;
				if (east_line->parent_chunk != parent_chunk)
					east_line->parent_chunk->RecalculateComplexVbo();
			}
		}
		~Redstone() override
		{
			std::cout << "Unloading redstone\n";
		}
		unsigned char stable_power_level = 0;
	private:
		void CheckLines()
		{
			SimpleBlock* block;
			//Chunk* chunk = parent_chunk;

			//north line
			int local_x = position.x - 16 * parent_chunk->chunk_x, local_y = position.y, local_z = position.z - parent_chunk->chunk_z * 16 + 1;
			if ((block = parent_chunk->GetBlockInArea(local_x, local_y, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				north_line = (Redstone*)block;
				north_line->south_line = this;
				const auto chunk = north_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y + 1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				north_line = (Redstone*)block;
				north_line->south_line = this;
				const auto chunk = north_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y - 1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				north_line = (Redstone*)block;
				north_line->south_line = this;
				const auto chunk = north_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}

			//south line
			local_x = position.x - parent_chunk->chunk_x * 16, local_y = position.y, local_z = position.z - parent_chunk->chunk_z * 16 - 1;
			if ((block = parent_chunk->GetBlockInArea(local_x, local_y, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				south_line =(Redstone*)block;
				south_line->north_line = this;
				const auto chunk = south_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y+1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				south_line = (Redstone*)block;
				south_line->north_line = this;
				const auto chunk = south_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y-1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				south_line = (Redstone*)block;
				const auto chunk = south_line->parent_chunk;
				south_line->north_line = this;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}

			//west line
			local_x = position.x - parent_chunk->chunk_x * 16 + 1, local_y = position.y, local_z = position.z - parent_chunk->chunk_z * 16;
			if ((block = parent_chunk->GetBlockInArea(local_x, local_y, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				west_line = (Redstone*)block;
				west_line->east_line = this;
				const auto chunk = west_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y+1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				west_line = (Redstone*)block;
				west_line->east_line = this;
				const auto chunk = west_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y-1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				west_line = (Redstone*)block;
				west_line->east_line = this;
				const auto chunk = west_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}

			//east line
			local_x = position.x - parent_chunk->chunk_x * 16 - 1, local_y = position.y, local_z = position.z - parent_chunk->chunk_z * 16;
			if ((block = parent_chunk->GetBlockInArea(local_x, local_y, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				east_line = (Redstone*)block;
				east_line->west_line = this;
				const auto chunk = east_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y+1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				east_line = (Redstone*)block;
				east_line->west_line = this;
				const auto chunk = east_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
			else if ((block = parent_chunk->GetBlockInArea(local_x, local_y-1, local_z)) != nullptr && block->id == blk_id::redstone_id)
			{
				east_line = (Redstone*)block;
				east_line->west_line = this;
				const auto chunk = east_line->parent_chunk;
				if (chunk != parent_chunk)
					chunk->RecalculateComplexVbo();
			}
		}
		Redstone *north_line = nullptr, *south_line = nullptr, *west_line = nullptr, *east_line = nullptr;
	};


	class Door : public ComplexBlock
	{
	public:
		Door(glm::ivec3 position, glm::ivec3 parent_position, Chunk* parent_chunk, bool top = false) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::door_id;
			//this->parent_position = parent_position;
			//this->parent_chunk = parent_chunk;
			direction = SimpleBlock::GetDirection(glm::ivec3(Player::position) - position);
			if (direction == BOTTOM || direction == TOP)
				direction = NORTH;
			//this->other = other;
			this->top = top;

			if (!top)
			{
				other = new Door(glm::ivec3(position.x, position.y + 1, position.z), position, parent_chunk, true);
				other->direction = direction;
				parent_chunk->ReplaceBlock(position.x, position.y + 1, position.z, other);
				other->other = this;
			}
		}
		Door(glm::ivec3 position, Chunk* parent_chunk, char*& save_data_pointer) : ComplexBlock(position, parent_chunk)
		{
			id = blk_id::door_id;

			direction = (Direction)save_data_pointer[0];
			opened = (char)save_data_pointer[1];
			save_data_pointer+=2;

			top = false;
			other = new Door(glm::ivec3(position.x, position.y + 1, position.z), position, parent_chunk, true);
			other->direction = direction;
			other->opened = opened;
			parent_chunk->ReplaceBlock(position.x, position.y + 1, position.z, other);
			other->other = this;
		}


		int GetBlockSpecificMetaSize() override
		{
			if (!top)
				return 2;
			else
				return 0;
		}
		void WriteBlockSpecificMeta(char*& save_data_pointer) override
		{
			if (!top)
			{
				save_data_pointer[0] = (char)direction;
				save_data_pointer[1] = (char)opened;
				save_data_pointer+=2;
			}
		}
		int GetNumberOfTriangles() override
		{
			return 12;
		}
		void OnPlayerClick() override
		{
			std::cout << "DoorClick\n";
			parent_chunk->save_needed = true;
			opened = !opened;

			other->opened = opened;
			parent_chunk->RecalculateComplexVbo();
		}

		void OnDestroy() override
		{
			std::cout << "Door Destroy\n";
			if (other != nullptr)
			{
				other->other = nullptr;
				other->SelfDestruct();
			}
			else
			{
				parent_chunk->RecalculateComplexVbo();
			}
		}

		bool CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info) override;
		void CreateModel(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z) override;
		~Door() override
		{
			std::cout << "Unloading door\n";
		}

		Door* other;
	private:
		const float depth = 2.0f / 16.0f;
		bool top = true;
		bool opened = false;
	};
};