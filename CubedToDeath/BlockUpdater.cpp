#include "BlockUpdater.h"
#include "ComplexBlock.h"

void BlockUpdater::Update()
{
	chunk_map = ChunkManager::GetChunkMap();
	bool model_changed = false;
	for (auto iterator : chunk_map)
	{
		bool chunk_contains_redstone = false;
		//Updating every complex block
		auto chunk = iterator.second;
		for (int y = 0; y < 127; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
				{
					auto block = chunk->blocks[y][x][z];
					if (block->GetFlag(SimpleBlock::COMPLEX))
					{
						auto complex =(ComplexBlock*)block;

						if (complex->id == blk_id::redstone_id)
						{
							chunk_contains_redstone = true;
							///			temprary on not		vvv//
							model_changed = true;
						}
						complex->OnTick();
						if (complex->model_changed)
							model_changed = true;
						complex->model_changed = false;

					}
				}

		if (chunk_contains_redstone)
		{
			for (int y = 0; y < 127; y++)
				for (int x = 0; x < 16; x++)
					for (int z = 0; z < 16; z++)
					{
						auto block = chunk->blocks[y][x][z];
							if (block->id == blk_id::redstone_id)
							{
								((blk::Redstone*)block)->PropagetePower();
							}
					}
		}
		if (model_changed)
			chunk->RecalculateComplexVbo();
	}

	ChunkManager::GiveThreadPermissionToUnloadBlocks(ChunkManager::BLOCK_UPDATER);
	ChunkManager::GiveThreadPermissionToUnloadChunks(ChunkManager::BLOCK_UPDATER);
}

chunk_hash_map BlockUpdater::chunk_map;
