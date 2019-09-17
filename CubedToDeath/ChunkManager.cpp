#include "ChunkManager.h"
#include "Player.h"
#include "MyCraft.h"
#include <stdlib.h>
#include "ComplexBlock.h"
#include "Save.h"

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

	d3_noise.SetNoiseType(FastNoise::Cubic);
	d3_noise.SetSeed(18797);
	d3_noise.SetFrequency(0.08);
	//d3_noise.SetGradientPerturbAmp(2.0);

	tree_noise.SetNoiseType(FastNoise::Value);
	tree_noise.SetFrequency(0.2);
	tree_noise.SetSeed(42319);

	tree_placement_noise.SetNoiseType(FastNoise::Simplex);
	tree_placement_noise.SetFrequency(0.005);
	tree_placement_noise.SetSeed(427);

	moisture_noise.SetNoiseType(FastNoise::Simplex);
	//mountain_placement_noise.SetFractalOctaves(2);
	moisture_noise.SetFrequency(0.006);
	moisture_noise.SetSeed(1543676);

	tectonical_noise.SetNoiseType(FastNoise::Simplex);
	tectonical_noise.SetFrequency(0.005);
	moisture_noise.SetSeed(847593);

	ocean_noise.SetFrequency(0.001);
	ocean_noise.SetSeed(55645);
}

void ChunkManager::Update()
{
	//loading world around the player
	LoadWorld(Player::current_chunk_x, Player::current_chunk_z);
	//allow queues to delete blocks
	GiveThreadPermissionToUnloadBlocks(WORLD_MANAGER);
	GiveThreadPermissionToUnloadChunks(WORLD_MANAGER);
	//UpdateVisibility();
	UnloadBlocks();
}

///				obsolete			///
/*void ChunkManager::UpdateVisibility()
{
	std::lock_guard<std::mutex> lock(chunk_map_mutex);
	auto iterator = chunk_map.begin();
	while (iterator != chunk_map.end())
	{
		if (iterator->second->visibility_update_needed)
		{
			iterator->second->RecalculateVisibility();
		}
		iterator++;
	}
}*/

void ChunkManager::UnloadBlocks()
{
	//Deleting Queued Blocks if every thread declared it is ok to do so
	{
		std::lock_guard<std::mutex> lock(block_unload_queue_mutex);
		auto iterator = block_unload_queue.begin();
		while (iterator != block_unload_queue.end())
		{
			//deleting block if each flag is set to true
			if ((*iterator).flags[0] && (*iterator).flags[1] && (*iterator).flags[2])
			{
				if(iterator->item->GetFlag(SimpleBlock::COMPLEX))
					delete (ComplexBlock*)(iterator->item);
				else
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
			if ((chunk->chunk_x - Player::current_chunk_x) * (chunk->chunk_x - Player::current_chunk_x) + (chunk->chunk_z - Player::current_chunk_z) * (chunk->chunk_z - Player::current_chunk_z) >= (MyCraft::render_distance + 1) * (MyCraft::render_distance + 1))
			{
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
			if ((*iterator).flags[0] && (*iterator).flags[1] && (*iterator).flags[2])
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
			if(iterator->item->save_needed)
				save::SaveChunk(iterator->item);
			delete iterator->item;
			iterator = chunks_to_delete.erase(iterator);
		}
	}
}


void ChunkManager::LoadWorld(int starting_chunk_x, int starting_chunk_z)
{
	//loading chunk player is standing on
	LoadChunk(starting_chunk_x, starting_chunk_z);

	//pętla wczytująca chunki dookoła gracza zaczynając od najbliższych
	for (int distance = 1; distance < MyCraft::render_distance; distance++)
	{
		if (MyCraft::program_should_close)
			return;
		//wczytywanie chunków
		for (int chunk_x = -distance; chunk_x <= distance; chunk_x++)
		{
			if (chunk_x * chunk_x + distance * distance >= MyCraft::render_distance * MyCraft::render_distance)
				continue;
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
		for (int chunk_z = -distance + 1; chunk_z <= distance - 1; chunk_z++)
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

		
		//Generating stuctures on chunks one close than what is generated
		if (distance < 1)
			continue;
		int structure_distance = distance - 1;
		for (int chunk_x = -structure_distance; chunk_x <= structure_distance; chunk_x++)
		{
			if (chunk_x * chunk_x + structure_distance  * structure_distance  >= MyCraft::render_distance  * MyCraft::render_distance )
				continue;
			//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}

			if (Chunk * chunk = GetChunk(starting_chunk_x + chunk_x, starting_chunk_z + structure_distance))
				if (!chunk->structures_generated)
					chunk->GenerateStructures();
			if (Chunk* chunk = GetChunk(starting_chunk_x + chunk_x, starting_chunk_z - structure_distance))
				if (!chunk->structures_generated)
					chunk->GenerateStructures();
		}
		for (int chunk_z = -structure_distance  + 1; chunk_z <= structure_distance  - 1; chunk_z++)
		{
			if (chunk_z * chunk_z + structure_distance  * structure_distance  >= MyCraft::render_distance  * MyCraft::render_distance )
				continue;
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}

			if (Chunk * chunk = GetChunk(starting_chunk_x - structure_distance, starting_chunk_z + chunk_z))
				if (!chunk->structures_generated)
					chunk->GenerateStructures();
			if (Chunk * chunk = GetChunk(starting_chunk_x + structure_distance, starting_chunk_z + chunk_z))
				if (!chunk->structures_generated)
					chunk->GenerateStructures();
		}

		//updating visibility on chunks on closer than where structures are generated
		if (distance < 2)
			continue;
		int visibility_distance = distance - 2;
		for (int chunk_x = -visibility_distance; chunk_x <= visibility_distance; chunk_x++)
		{
			if (chunk_x * chunk_x + visibility_distance * visibility_distance >= MyCraft::render_distance * MyCraft::render_distance)
				continue;
			//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}

			if (Chunk * chunk = GetChunk(starting_chunk_x + chunk_x, starting_chunk_z + visibility_distance))
				if (chunk->recalculate_vbos_needed)
					chunk->RecalculateVbos();
			if (Chunk * chunk = GetChunk(starting_chunk_x + chunk_x, starting_chunk_z - visibility_distance))
				if (chunk->recalculate_vbos_needed)
					chunk->RecalculateVbos();
		}
		for (int chunk_z = -visibility_distance + 1; chunk_z <= visibility_distance - 1; chunk_z++)
		{
			if (chunk_z * chunk_z + visibility_distance * visibility_distance >= MyCraft::render_distance * MyCraft::render_distance)
				continue;
			if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
			{
				return;
			}

			if (Chunk * chunk = GetChunk(starting_chunk_x - visibility_distance, starting_chunk_z + chunk_z))
				if (chunk->recalculate_vbos_needed)
					chunk->RecalculateVbos();
			if (Chunk * chunk = GetChunk(starting_chunk_x + visibility_distance, starting_chunk_z + chunk_z))
				if (chunk->recalculate_vbos_needed)
					chunk->RecalculateVbos();
		}
	}

	//updating visibility on the pre last ring visible
	int visibility_distance = MyCraft::render_distance - 2;
	for (int chunk_x = -visibility_distance; chunk_x <= visibility_distance; chunk_x++)
	{
		if (chunk_x * chunk_x + visibility_distance * visibility_distance >= MyCraft::render_distance * MyCraft::render_distance)
			continue;
		//funkcja koñczy siê gdy gracz zmieni³ chunk aby nie wczytywaæ terenu dooko³a miejsca gdzie gracza ju¿ nie ma
		if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
		{
			return;
		}

		if (Chunk * chunk = GetChunk(starting_chunk_x + chunk_x, starting_chunk_z + visibility_distance))
			if (chunk->recalculate_vbos_needed)
				chunk->RecalculateVbos();
		if (Chunk * chunk = GetChunk(starting_chunk_x + chunk_x, starting_chunk_z - visibility_distance))
			if (chunk->recalculate_vbos_needed)
				chunk->RecalculateVbos();
	}
	for (int chunk_z = -visibility_distance + 1; chunk_z <= visibility_distance - 1; chunk_z++)
	{
		if (chunk_z * chunk_z + visibility_distance * visibility_distance >= MyCraft::render_distance * MyCraft::render_distance)
			continue;
		if (Player::current_chunk_x != starting_chunk_x || Player::current_chunk_z != starting_chunk_z)
		{
			return;
		}

		if (Chunk * chunk = GetChunk(starting_chunk_x - visibility_distance, starting_chunk_z + chunk_z))
			if (chunk->recalculate_vbos_needed)
				chunk->RecalculateVbos();
		if (Chunk * chunk = GetChunk(starting_chunk_x + visibility_distance, starting_chunk_z + chunk_z))
			if (chunk->recalculate_vbos_needed)
				chunk->RecalculateVbos();
	}
}

void ChunkManager::LoadChunk(int chunk_x, int chunk_z)
{
	{
		//check if block has been alreade loaded, and return if thats the case
		std::lock_guard<std::mutex> lock(chunk_map_mutex);
		auto iterator = chunk_map.find(std::make_pair(chunk_x, chunk_z));
		if (iterator != chunk_map.end())
		{
			return;
		}
	}

	//check if chunk has been saved previously
	Chunk* chunk = save::LoadChunkFromFile(chunk_x, chunk_z);
	if (chunk == nullptr)
	{
		//generating chunk if there is no save of that chunk
		chunk = GenerateChunk(chunk_x, chunk_z);
	}

	//finding neighbours and recalculating visibility
	chunk->FindNeighbours();
	chunk->RecalculateVbos();
	chunk->InitializeComplexBlocks();

	//blokada i umieszczenie nowego chunka w mapie
	std::lock_guard<std::mutex> lock(chunk_map_mutex);
	chunk_map.emplace(std::make_pair(chunk_x, chunk_z), chunk);
}

Chunk* ChunkManager::GenerateChunk(int chunk_x, int chunk_z)
{
	//generowanie chunka oraz stworzenie jego zapisu
	Chunk* chunk = new Chunk(chunk_x, chunk_z);
	chunk->GenerateTerrain();
	save::SaveChunk(chunk);
	return chunk;
}

//returns A COPY of chunk map
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

//saves all of the loaded chunks if they need to be saved
void ChunkManager::CleanUp()
{
	for (auto iterator : chunk_map)
	{
		auto chunk = iterator.second;
		if(chunk->save_needed)
			save::SaveChunk(chunk);
		delete chunk;
	}
}

//queues all of the chunks to be unloaded
void ChunkManager::QueueChunkToUnload(Chunk* chunk)
{
	std::lock_guard<std::mutex> lock(chunk_unload_queue_mutex);
	chunk_unload_queue.push_back(ChunkManager::ItemQueuedToUnload<Chunk>(chunk));

	if (chunk->south_chunk)
		chunk->south_chunk->north_chunk = nullptr;
	if (chunk->north_chunk)
		chunk->north_chunk->south_chunk = nullptr;
	if (chunk->east_chunk)
		chunk->east_chunk->west_chunk = nullptr;
	if (chunk->west_chunk)
		chunk->west_chunk->east_chunk = nullptr;
}

std::list<ChunkManager::ItemQueuedToUnload<SimpleBlock>> ChunkManager::block_unload_queue;
std::list<ChunkManager::ItemQueuedToUnload<Chunk>> ChunkManager::chunk_unload_queue;
chunk_hash_map ChunkManager::chunk_map;
FastNoise ChunkManager::test_noise;
FastNoise ChunkManager::tree_noise;
FastNoise ChunkManager::tree_placement_noise;
FastNoise ChunkManager::moisture_noise;
FastNoise ChunkManager::tectonical_noise;
FastNoise ChunkManager::ocean_noise;
FastNoise ChunkManager::d3_noise;
std::mutex ChunkManager::chunk_map_mutex;
std::mutex ChunkManager::block_unload_queue_mutex;
std::mutex ChunkManager::chunk_unload_queue_mutex;
int ChunkManager::last_chunk_x = 10;
int ChunkManager::last_chunk_z = 10;