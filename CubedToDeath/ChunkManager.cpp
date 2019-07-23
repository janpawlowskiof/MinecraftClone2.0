#include "ChunkManager.h"
#include "Player.h"
#include "MyCraft.h"
#include <stdlib.h>

void ChunkManager::Update()
{
	const int current_chunk_x = Player::position.x / 16 + (Player::position.x < 0 ? -1 : 0);
	const int current_chunk_z = Player::position.z / 16 + (Player::position.z < 0 ? -1 : 0);

	//deloading chunks that are too far from the player
	if (current_chunk_x != last_chunk_x || current_chunk_z != last_chunk_z)
	{
		auto iterator = chunk_map.begin();
		while (iterator != chunk_map.end())
		{
			//NOTE: chunks are unloaded at distance greater than render_distance and loader at distance lower than render_distance to prevent constant loads and deletes of chunks
			const auto chunk = iterator->second;
			if (std::abs(chunk->chunk_x - current_chunk_x) > MyCraft::render_distance || std::abs(chunk->chunk_z - current_chunk_z) > MyCraft::render_distance)
			{
				///			SAVING CHUNK HERE		///
				chunk->chunk_waiting_to_unload = true;
				//iterator = chunk_map.erase(iterator);
			}
			++iterator;
		}
		//wczytywanie chunk na którym znajduje siê gracz
		auto chunk = chunk_map.find(std::make_pair(current_chunk_x, current_chunk_z));
		if (chunk == chunk_map.end())
		{
			LoadChunk(current_chunk_x, current_chunk_z);
		}
		//pêtla wczytuj¹ca chunki dooko³a gracza zaczynaj¹c od najbli¿szych
		for (int distance = 1; distance < MyCraft::render_distance; distance++)
		{
			for (int chunk_x = current_chunk_x - distance; chunk_x <= current_chunk_x + distance; chunk_x++)
			{
				chunk = chunk_map.find(std::make_pair(chunk_x, current_chunk_z + distance));
				if (chunk == chunk_map.end())
				{
					//trzeba wczytaæ chunk
					LoadChunk(chunk_x, current_chunk_z + distance);
				}
				chunk = chunk_map.find(std::make_pair(chunk_x, current_chunk_z - distance));
				if (chunk == chunk_map.end())
				{
					//trzeba wczytaæ chunk
					LoadChunk(chunk_x, current_chunk_z - distance);
				}
			}
			for (int chunk_z = current_chunk_z - distance + 1; chunk_z <= current_chunk_z + distance - 1; chunk_z++)
			{
				chunk = chunk_map.find({ current_chunk_x - distance, chunk_z });
				if (chunk == chunk_map.end())
				{
					//trzeba wczytaæ chunk
					LoadChunk(current_chunk_x - distance, chunk_z);
				}
				chunk = chunk_map.find({ current_chunk_x + distance, chunk_z });
				if (chunk == chunk_map.end())
				{
					//trzeba wczytaæ chunk
					LoadChunk(current_chunk_x + distance, chunk_z);
				}
			}
		}
		//updating last posiotion;
		last_chunk_x = current_chunk_x;
		last_chunk_z = current_chunk_z;
	}
	GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);

	//Deleting Blocks Queued for deletion if every thread declared it is ok to do so
	auto iterator = blocks_waiting_to_unload.begin();
	while (iterator != blocks_waiting_to_unload.end())
	{
		//deleting block if each flag is set to true
		if ((*iterator).flags[0] && (*iterator).flags[1])
		{
			delete (*iterator).block;
			iterator = blocks_waiting_to_unload.erase(iterator);
		}
		else
		{
			iterator++;
		}
	}
}

void ChunkManager::LoadChunk(int chunk_x, int chunk_z)
{
	//Generacja lub Wczytanie czunka
	Chunk* chunk = new Chunk(chunk_x, chunk_z);
	chunk->RecalculateVisibility();
	chunk->buffers_update_needed = true;
	chunk_map.emplace(std::make_pair(chunk_x, chunk_z), chunk);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::GiveThreadPermissionToUnloadBlocks(ThreadId thread_id)
{
	//sets flag for each block
	for (auto &element : blocks_waiting_to_unload)
	{
		element.flags[thread_id] = true;
	}
}

void ChunkManager::QueueBlockForUnload(SimpleBlock* block)
{
	//each flag is false by default
	bool flags[2] = { false, false };
	blocks_waiting_to_unload.push_back(ChunkManager::BlockWaitingToUnload(block, flags));
}

std::list<ChunkManager::BlockWaitingToUnload> ChunkManager::blocks_waiting_to_unload;
std::map<std::pair<int, int>, std::shared_ptr<Chunk>> ChunkManager::chunk_map;