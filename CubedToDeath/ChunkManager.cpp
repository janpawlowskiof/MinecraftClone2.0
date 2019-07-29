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
	test_noise.SetFrequency(0.015);
	test_noise.SetGradientPerturbAmp(2.0);

	tree_noise.SetFrequency(0.2);
	tree_noise.SetSeed(42319);

	mountain_placement_noise.SetNoiseType(FastNoise::Cubic);
	//mountain_placement_noise.SetFractalOctaves(2);
	mountain_placement_noise.SetFrequency(0.003);
	mountain_placement_noise.SetSeed(1543676);

	tectonical_noise.SetNoiseType(FastNoise::Cubic);
	tectonical_noise.SetFrequency(0.01);
	mountain_placement_noise.SetSeed(847593);

	ocean_noise.SetFrequency(0.001);
	ocean_noise.SetSeed(55645);
}

void ChunkManager::Update()
{
	//loading world around the player
	LoadWorld(Player::current_chunk_x, Player::current_chunk_z);

	GenerateStructures();
	//allow queues to delete blocks
	GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
	GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);
	UnloadChunks();
	UnloadBlocks();
}

void ChunkManager::UnloadBlocks()
{
	//Deleting Blocks Queued for deletion if every thread declared it is ok to do so
	{
		std::lock_guard<std::mutex> lock(block_unload_queue_mutex);
		auto iterator = block_unload_queue.begin();
		while (iterator != block_unload_queue.end())
		{
			//deleting block if each flag is set to true
			if ((*iterator).flags[0] && (*iterator).flags[1])
			{
				delete (iterator->item);
				iterator = block_unload_queue.erase(iterator);
			}
			else
			{
				iterator++;
			}
		}
	}
}

void ChunkManager::UnloadChunks()
{
	//queuing chunks (that are too far from the player) to be unloaded
	if (Player::current_chunk_x != last_chunk_x || Player::current_chunk_z != last_chunk_z)
	{
		//lock since we modify the map
		std::lock_guard<std::mutex> lock(chunk_map_mutex);

		auto iterator = chunk_map.begin();
		while (iterator != chunk_map.end())
		{
			//NOTE: chunks are unloaded at distance greater than render_distance and loader at distance lower than render_distance to prevent constant loads and deletes of chunks
			const auto chunk = iterator->second;
			if ((chunk->chunk_x - Player::current_chunk_x)*(chunk->chunk_x - Player::current_chunk_x) + (chunk->chunk_z - Player::current_chunk_z) * (chunk->chunk_z - Player::current_chunk_z) > MyCraft::render_distance * MyCraft::render_distance)
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
		last_chunk_x = Player::current_chunk_x;
		last_chunk_z = Player::current_chunk_z;
	}

	//NOTE:			since deleting a chunk takes A LOOOOOOOOT of time, 
	//				we copy chunks that we need to delete to second list 
	//				and then delete them in order not to block			
	//				access to chunk_unload_queue					

	//list of chunks that will be deleted now

	//printf("bef: %i ", chunk_unload_queue.size());
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
	//printf("aft: %i\n", chunk_unload_queue.size());

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


void ChunkManager::LoadWorld(int starting_chunk_x, int starting_chunk_z)
{
	LoadChunk(starting_chunk_x, starting_chunk_z);
	//pętla wczytująca chunki dookoła gracza zaczynając od najbliższych
	for (int distance = 1; distance < MyCraft::render_distance; distance++)
	{
		for (int chunk_x = - distance; chunk_x <= distance; chunk_x++)
		{
			if (chunk_x * chunk_x + distance * distance >= MyCraft::render_distance * MyCraft::render_distance)
				continue;
			if(chunk_x)
			//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}

			//allow queues to delete blocks
			GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
			GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);

			LoadChunk(starting_chunk_x + chunk_x, starting_chunk_z + distance);
			LoadChunk(starting_chunk_x + chunk_x, starting_chunk_z - distance);
		}
		for (int chunk_z = - distance + 1; chunk_z <= distance - 1; chunk_z++)
		{
			if (chunk_z * chunk_z + distance * distance >= MyCraft::render_distance * MyCraft::render_distance)
				continue;
			//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}
			//allow queues to delete blocks
			GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
			GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);

			LoadChunk(starting_chunk_x - distance, starting_chunk_z + chunk_z);
			LoadChunk(starting_chunk_x + distance, starting_chunk_z + chunk_z);
		}
	}

	//for (int distance = 1; distance < MyCraft::render_distance; distance++)
	//{
		/*for (int chunk_offset_x = 0; chunk_offset_x < MyCraft::render_distance; chunk_offset_x++)
		{
			for (int chunk_offset_z = 0; chunk_offset_z * chunk_offset_z < MyCraft::render_distance * MyCraft::render_distance - chunk_offset_x * chunk_offset_x; chunk_offset_z++)
			{
				//allow queues to delete blocks
				GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
				GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);

			//funkcja kończy się gdy gracz zmienił chunk aby nie wczytywać terenu dookoła miejsca gdzie gracza już nie ma
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}
				//allow queues to delete blocks
				GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
				GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);


				LoadChunk(Player::current_chunk_x + chunk_offset_x, Player::current_chunk_z + chunk_offset_z);
				if (chunk_offset_x != 0)
				{
					LoadChunk(Player::current_chunk_x - chunk_offset_x, Player::current_chunk_z + chunk_offset_z);
				}
				if (chunk_offset_z != 0)
				{
					LoadChunk(Player::current_chunk_x + chunk_offset_x, Player::current_chunk_z - chunk_offset_z);
					if (chunk_offset_x != 0)
					{
						LoadChunk(Player::current_chunk_x - chunk_offset_x, Player::current_chunk_z - chunk_offset_z);
					}
				}
				//LoadChunk(chunk_x, chunk_z);
			}

		}*/
	
	//}
}

void ChunkManager::GenerateStructures()
{
	auto iterator = chunk_map.begin();
	while (iterator != chunk_map.end())
	{
		Chunk* chunk = iterator->second;
		if (!chunk->structures_generated)
		{
			Chunk* north_chunk, * south_chunk, * east_chunk, * west_chunk;
			if ((north_chunk = GetChunk(chunk->chunk_x, chunk->chunk_z + 1)) != nullptr
				&& (south_chunk = GetChunk(chunk->chunk_x, chunk->chunk_z - 1)) != nullptr
				&& (east_chunk = GetChunk(chunk->chunk_x - 1, chunk->chunk_z)) != nullptr
				&& (west_chunk = GetChunk(chunk->chunk_x + 1, chunk->chunk_z)) != nullptr)
			{
				//GENERATING STRUCTURES HERE
				float tree_values[18][18];
				for(int x = -1; x < 17; x++)
					for (int z = -1; z < 17; z++)
					{
						tree_values[x + 1][z + 1] = tree_noise.GetValue(chunk->chunk_x * 16 + x, chunk->chunk_z * 16 + z);
					}

				for (int x = 0; x < 16; x++)
					for (int z = 0; z < 16; z++)
					{
						if (tree_values[x + 1][z + 1] > tree_values[x][z + 1] &&
							tree_values[x + 1][z + 1] > tree_values[x + 2][z + 1] &&
							tree_values[x + 1][z + 1] > tree_values[x + 1][z + 2] &&
							tree_values[x + 1][z + 1] > tree_values[x + 1][z])
						{
							int ground_level = chunk->height_values[x][z];

							chunk->ReplaceBlock(x, ground_level, z, new SimpleBlock(blk_id::wood_id), false);
							chunk->ReplaceBlock(x, ground_level + 1, z, new SimpleBlock(blk_id::wood_id), false);
							chunk->ReplaceBlock(x, ground_level + 2, z, new SimpleBlock(blk_id::wood_id), false);
							chunk->ReplaceBlock(x, ground_level + 3, z, new SimpleBlock(blk_id::wood_id), false);
							chunk->ReplaceBlock(x, ground_level + 4, z, new SimpleBlock(blk_id::wood_id), false);
						}
					}
				chunk->RecalculateVisibility(chunk_map);
				chunk->structures_generated = true;
			}
		}

		GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
		GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);
		UnloadBlocks();

		iterator++;
	}
}

void ChunkManager::LoadChunk(int chunk_x, int chunk_z)
{
	{
		std::lock_guard<std::mutex> lock(chunk_map_mutex);
		auto iterator = chunk_map.find(std::make_pair(chunk_x, chunk_z));
		if (iterator != chunk_map.end())
			return;
	}

	//generowanie lub wczytywanie
	Chunk* chunk = new Chunk(chunk_x, chunk_z);
	//Generacja lub Wczytanie czunka
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
FastNoise ChunkManager::tree_noise;
FastNoise ChunkManager::mountain_placement_noise;
FastNoise ChunkManager::tectonical_noise;
FastNoise ChunkManager::ocean_noise;
std::mutex ChunkManager::chunk_map_mutex;
std::mutex ChunkManager::block_unload_queue_mutex;
std::mutex ChunkManager::chunk_unload_queue_mutex;
int ChunkManager::last_chunk_x = 10;
int ChunkManager::last_chunk_z = 10;