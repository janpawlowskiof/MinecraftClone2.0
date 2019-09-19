#include "BlockUpdater.h"
#include "ComplexBlock.h"

int c = 0;

void BlockUpdater::Update()
{
	chunk_map = ChunkManager::GetChunkMap();
	for (auto iterator : chunk_map)
	{
		auto chunk = iterator.second;
		if (chunk->complex_block_count <= 0)
			continue;
		//Updating every complex block
		chunk->contains_redstone = false;
		for (int y = 0; y < 127; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
				{
					auto block = chunk->blocks[y][x][z];
					if (block->id == blk_id::redstone_id)
					{
						chunk->contains_redstone = true;
						block->power_level = 0;
						///			temprary on not		vvv//
					}
				}
	}

	for (auto iterator : chunk_map)
	{
		auto chunk = iterator.second;
		if (chunk->complex_block_count <= 0)
			continue;
		if (chunk->contains_redstone)
		{
			for (int y = 0; y < 127; y++)
				for (int x = 0; x < 16; x++)
					for (int z = 0; z < 16; z++)
					{
						auto block = chunk->blocks[y][x][z];
							if (block->id == blk_id::redstone_id)
							{
								const auto redstone = (blk::Redstone*)block;
								redstone->PropagetePower();
							}
					}

		}

	}

	for (auto iterator : chunk_map)
	{
		auto chunk = iterator.second;
		if (chunk->complex_block_count <= 0)
			continue;
			for (int y = 0; y < 127; y++)
				for (int x = 0; x < 16; x++)
					for (int z = 0; z < 16; z++)
					{
						auto block = chunk->blocks[y][x][z];
						if (block->id == blk_id::redstone_id)
						{
							const auto redstone = (blk::Redstone*)block;

							if (redstone->power_level != redstone->stable_power_level)
							{
								chunk->complex_model_changed = true;
								redstone->stable_power_level = redstone->power_level;

								redstone->RecalculateNeightboursPowerLevel(true);
							}
						}

						if (block->GetFlag(SimpleBlock::COMPLEX))
						{
							auto complex = (ComplexBlock*)block;

							complex->OnTick();
							if (complex->model_changed)
								chunk->complex_model_changed = true;
							complex->model_changed = false;

						}
					}

		if (chunk->complex_model_changed)
		{
			chunk->RecalculateComplexVbo();
		}

	}

	ChunkManager::GiveThreadPermissionToUnloadBlocks(ChunkManager::BLOCK_UPDATER);
	ChunkManager::GiveThreadPermissionToUnloadChunks(ChunkManager::BLOCK_UPDATER);
}

chunk_hash_map BlockUpdater::chunk_map;
