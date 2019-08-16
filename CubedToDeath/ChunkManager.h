#pragma once
#include <list>
#include <map>
#include <mutex>
#include "Chunk.h"
#include "FastNoise.h"
#include <thread>

typedef std::map<std::pair<int, int>, Chunk*> chunk_hash_map;

class ChunkManager
{
public:
	ChunkManager();
	//noise maps for terrain generation
	static FastNoise test_noise;
	static FastNoise mountain_placement_noise;
	static FastNoise tectonical_noise;
	static FastNoise ocean_noise;
	static FastNoise tree_noise;
	static FastNoise tree_placement_noise;
	static FastNoise d3_noise;
	static int last_chunk_x, last_chunk_z;
	//updates world
	void Update();
	//loads given chunk
	static void LoadChunk(int chunk_x, int chunk_z);

	//return a copy of a current version of chunk_map
	static chunk_hash_map GetChunkMap();
	static Chunk* GetChunk(int chunk_x, int chunk_z)
	{
		auto iterator = chunk_map.find(std::make_pair(chunk_x, chunk_z));
		return (iterator != chunk_map.end()) ? iterator->second : nullptr;
	}
	~ChunkManager();
	
	//ids of threads in program
	enum ThreadId
	{
		MAIN = 0,
		WORLD_MANAGER = 1,
		BLOCK_UPDATER = 2,
	};

	//give permission to delete removed blocks
	static void GiveThreadPermissionToUnloadBlocks(ThreadId thread);
	//give permission to delete removed chunks
	static void GiveThreadPermissionToUnloadChunks(ThreadId thread);
	//queues a block to be unloaded once it is certaint that no thread uses this block
	static void QueueBlockToUnload(SimpleBlock* block);
	///should be private
	//helper struct
	static void UnloadChunks();
private:
	template <typename T>
	struct ItemQueuedToUnload
	{
		ItemQueuedToUnload<T>(T* item)
		{
			this->item = item;
			//no permission by default
			this->flags[0] = false;
			this->flags[1] = false;
			this->flags[2] = false;
		}
		T* item;
		//permission from each thread
		bool flags[3];
	};
	//mutex for queue of blocks waiting to be unloaded
	static std::mutex block_unload_queue_mutex;
	//mutex for queue of chunks waiting to be unloaded
	static std::mutex chunk_unload_queue_mutex;
	//mutex for the chunk map
	static std::mutex chunk_map_mutex;

	//static void UpdateVisibility();

	//unloads chunks queued chunks to be deleted
	static void UnloadBlocks();
	//loads the world around given chunk coordinates
	void LoadWorld(int starting_chunk_x, int starting_chunk_z);
	//static void GenerateStructures();

	//queues a chunk to be unloaded once it is certaint that no thread uses this chunk
	static void QueueChunkToUnload(Chunk* chunk);
	static std::list<ItemQueuedToUnload<Chunk>> chunk_unload_queue;
	//list of blocks to bo be unloaded once it is certaint that no thread uses this block
	static std::list<ItemQueuedToUnload<SimpleBlock>> block_unload_queue;
	//list of chunks to bo be unloaded once it is certaint that no thread uses this block

	//map of chunk pointers
	static chunk_hash_map chunk_map;
};

