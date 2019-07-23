#pragma once
#include <list>
#include <map>
#include "Chunk.h"

class ChunkManager
{
public:
	//map of shared pointers
	//NOTE: shared pointers are used to prevent chunk from being deleted form memory while the it is used by other thread
	static std::map<std::pair<int, int>, std::shared_ptr<Chunk>> chunk_map;
	int last_chunk_x = 110, last_chunk_z = 110;
	//updates world
	void Update();
	void LoadChunk(int chunk_x, int chunk_z);
	~ChunkManager();
	enum ThreadId
	{
		MAIN = 0,
		WORLD_MANAGER = 1
	};
	static void GiveThreadPermissionToUnloadBlocks(ThreadId thread);

	//queues block bo be unloaded once it is certaint that no thread uses this block
	static void QueueBlockForUnload(SimpleBlock* block);
	//helper struct
	struct BlockWaitingToUnload
	{
		BlockWaitingToUnload(SimpleBlock* block, bool flags[2])
		{
			this->block = block;
			this->flags[0] = flags[0];
			this->flags[1] = flags[1];
		}
		SimpleBlock* block;
		bool flags[2];
	};
private:
	//list of blocks to bo be unloaded once it is certaint that no thread uses this block
	static std::list<BlockWaitingToUnload> blocks_waiting_to_unload;
};

