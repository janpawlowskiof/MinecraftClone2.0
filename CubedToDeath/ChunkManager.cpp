#include "ChunkManager.h"
#include "Player.h"
#include "MyCraft.h"
#include <stdlib.h>

ChunkManager::ChunkManager()
{
	//noise map initialization

	test_noise.SetNoiseType(FastNoise::Cubic);
	test_noise.SetSeed(1337);
	//test_noise.SetFractalOctaves(2);
	//test_noise.SetFractalLacunarity(1.5);
	//test_noise.SetFractalGain(1.1);
	test_noise.SetFrequency(0.01);
	test_noise.SetGradientPerturbAmp(2.0);

	mountain_placement_noise.SetNoiseType(FastNoise::Cubic);
	//mountain_placement_noise.SetFractalOctaves(2);
	mountain_placement_noise.SetFrequency(0.003);

	tectonical_noise.SetNoiseType(FastNoise::Simplex);
	tectonical_noise.SetFrequency(0.01);
}

void ChunkManager::Update()
{
	int current_chunk_x = Player::position.x / 16 + (Player::position.x < 0 ? -1 : 0);
	int current_chunk_z = Player::position.z / 16 + (Player::position.z < 0 ? -1 : 0);

	//queuing chunks (that are too far from the player) to be unloaded
	{
		//lock since we modify the map
		std::lock_guard<std::mutex> lock(chunk_map_mutex);

		auto iterator = chunk_map.begin();
		while (iterator != chunk_map.end())
		{
			//NOTE: chunks are unloaded at distance greater than render_distance and loader at distance lower than render_distance to prevent constant loads and deletes of chunks
			const auto chunk = iterator->second;
			if (std::abs(chunk->chunk_x - current_chunk_x) > MyCraft::render_distance || std::abs(chunk->chunk_z - current_chunk_z) > MyCraft::render_distance)
			{
				///			SAVING CHUNK HERE		///
				QueueChunkToUnload(iterator->second);
				iterator = chunk_map.erase(iterator);
			}
			else
			{
				++iterator;
			}
		}

	}
	//loading world around the player
	LoadWorld(current_chunk_x, current_chunk_z);

	//allow queues to delete blocks
	GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
	GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);

	//Deleting Blocks Queued for deletion if every thread declared it is ok to do so
	{
		auto iterator = block_unload_queue.begin();
		while (iterator != block_unload_queue.end())
		{
			std::lock_guard<std::mutex> lock(block_unload_queue_mutex);
			//deleting block if each flag is set to true
			if ((*iterator).flags[0] && (*iterator).flags[1])
			{
				delete (*iterator).item;
				iterator = block_unload_queue.erase(iterator);
			}
			else
			{
				iterator++;
			}
		}
	}

	//NOTE:			since deleting a chunk takes A LOOOOOOOOT of time, 
	//				we copy chunks that we need to delete to second list 
	//				and then delete them in order not to block			
	//				access to chunk_unload_queue					

	//list of chunks that will be deleted now
	std::list<ItemQueuedToUnload<Chunk>> chunks_to_delete;
	//Deleting Chunks Queued for deletion if every thread declared it is ok to do so
	{
		//locking the queue
		std::lock_guard<std::mutex> lock(chunk_unload_queue_mutex);
		auto iterator = chunk_unload_queue.begin();
		while (iterator != chunk_unload_queue.end())
		{
			//deleting block if each flag is set to true
			if ((*iterator).flags[0] && (*iterator).flags[1])
			{
				//removing element form original queue and pasting it into the new one
				chunks_to_delete.push_back(iterator->item);
				iterator = chunk_unload_queue.erase(iterator);
			}
			else
			{
				iterator++;
			}
		}
	}
	//actually deleting the chunk
	auto iterator = chunks_to_delete.begin();
	while (iterator != chunks_to_delete.end())
	{
		//deleting block if each flag is set to true
		{
			delete iterator->item;
			iterator = chunks_to_delete.erase(iterator);
		}
	}
}

void ChunkManager::LoadWorld(int& starting_chunk_x, int& starting_chunk_z)
{
	int current_chunk_x;
	int current_chunk_z;
	//wczytywanie chunk na którym znajduje siê gracz
	auto chunk = chunk_map.find(std::make_pair(starting_chunk_x, starting_chunk_z));
	if (chunk == chunk_map.end())
	{
		LoadChunk(starting_chunk_x, starting_chunk_z);
	}
	//pêtla wczytuj¹ca chunki dooko³a gracza zaczynaj¹c od najbli¿szych
	for (int distance = 1; distance < MyCraft::render_distance; distance++)
	{
		for (int chunk_x = starting_chunk_x - distance; chunk_x <= starting_chunk_x + distance; chunk_x++)
		{
			int current_chunk_x = Player::position.x / 16 + (Player::position.x < 0 ? -1 : 0);
			int current_chunk_z = Player::position.z / 16 + (Player::position.z < 0 ? -1 : 0);

			//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
			if (current_chunk_x != starting_chunk_x || current_chunk_z != starting_chunk_z)
			{
				starting_chunk_x = current_chunk_x;
				starting_chunk_z = current_chunk_z;
				return;
			}

			chunk = chunk_map.find(std::make_pair(chunk_x, starting_chunk_z + distance));
			if (chunk == chunk_map.end())
			{
				//trzeba wczytaæ chunk
				LoadChunk(chunk_x, starting_chunk_z + distance);
			}
			chunk = chunk_map.find(std::make_pair(chunk_x, starting_chunk_z - distance));
			if (chunk == chunk_map.end())
			{
				//trzeba wczytaæ chunk
				LoadChunk(chunk_x, starting_chunk_z - distance);
			}
		}
		for (int chunk_z = starting_chunk_z - distance + 1; chunk_z <= starting_chunk_z + distance - 1; chunk_z++)
		{
			int current_chunk_x = Player::position.x / 16 + (Player::position.x < 0 ? -1 : 0);
			int current_chunk_z = Player::position.z / 16 + (Player::position.z < 0 ? -1 : 0);

			//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
			if (current_chunk_x != starting_chunk_x || current_chunk_z != starting_chunk_z)
			{
				starting_chunk_x = current_chunk_x;
				starting_chunk_z = current_chunk_z;
				return;
			}


			chunk = chunk_map.find({ starting_chunk_x - distance, chunk_z });
			if (chunk == chunk_map.end())
			{
				//trzeba wczytaæ chunk
				LoadChunk(starting_chunk_x - distance, chunk_z);
			}
			chunk = chunk_map.find({ starting_chunk_x + distance, chunk_z });
			if (chunk == chunk_map.end())
			{
				//trzeba wczytaæ chunk
				LoadChunk(starting_chunk_x + distance, chunk_z);
			}
		}
	}
}

void ChunkManager::LoadChunk(int chunk_x, int chunk_z)
{
	//Generacja lub Wczytanie czunka
	Chunk* chunk = new Chunk(chunk_x, chunk_z);
	chunk->RecalculateVisibility(chunk_map);
	chunk->buffers_update_needed = true;

	//blokada i umieszczenie nowego chunka w mapie
	std::lock_guard<std::mutex> lock(chunk_map_mutex);
	chunk_map.emplace(std::make_pair(chunk_x, chunk_z), chunk);
}

chunk_hash_map ChunkManager::GetChunkMap()
{
	std::lock_guard<std::mutex> lock(chunk_map_mutex);
	return chunk_hash_map(chunk_map);
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::GiveThreadPermissionToUnloadBlocks(ThreadId thread)
{
	std::lock_guard<std::mutex> lock(block_unload_queue_mutex);
	//sets flag for each block
	for (auto& element : block_unload_queue)
	{
		element.flags[thread] = true;
	}
}

void ChunkManager::GiveThreadPermissionToUnloadChunks(ThreadId thread)
{
	std::lock_guard<std::mutex> lock(chunk_unload_queue_mutex);
	//sets flag for each block
	for (auto& element : chunk_unload_queue)
	{
		element.flags[thread] = true;
	}
}

void ChunkManager::QueueBlockToUnload(SimpleBlock* block)
{
	std::lock_guard<std::mutex> lock(block_unload_queue_mutex);
	block_unload_queue.push_back(ChunkManager::ItemQueuedToUnload<SimpleBlock>(block));
}

void ChunkManager::QueueChunkToUnload(Chunk* block)
{
	std::lock_guard<std::mutex> lock(chunk_unload_queue_mutex);
	chunk_unload_queue.push_back(ChunkManager::ItemQueuedToUnload<Chunk>(block));
}

std::list<ChunkManager::ItemQueuedToUnload<SimpleBlock>> ChunkManager::block_unload_queue;
std::list<ChunkManager::ItemQueuedToUnload<Chunk>> ChunkManager::chunk_unload_queue;
chunk_hash_map ChunkManager::chunk_map;
FastNoise ChunkManager::test_noise;
FastNoise ChunkManager::mountain_placement_noise;
FastNoise ChunkManager::tectonical_noise;
std::mutex ChunkManager::chunk_map_mutex;
std::mutex ChunkManager::block_unload_queue_mutex;
std::mutex ChunkManager::chunk_unload_queue_mutex;