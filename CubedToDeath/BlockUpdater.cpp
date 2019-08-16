#include "BlockUpdater.h"
#include "ComplexBlock.h"

void BlockUpdater::Update()
{
	chunk_map = ChunkManager::GetChunkMap();

	for (auto iterator : chunk_map)
	{
		auto chunk = iterator.second;
		for (int y = 0; y < 127; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
				{
					auto block = chunk->blocks[y][x][z];
					if (block->GetFlag(SimpleBlock::COMPLEX))
					{
						((ComplexBlock*)block)->OnTick();
					}
				}
	}

	ChunkManager::GiveThreadPermissionToUnloadBlocks(ChunkManager::BLOCK_UPDATER);
	ChunkManager::GiveThreadPermissionToUnloadChunks(ChunkManager::BLOCK_UPDATER);
}

chunk_hash_map BlockUpdater::chunk_map;
