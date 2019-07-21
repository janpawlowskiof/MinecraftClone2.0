#include "ChunkManager.h"
#include "Player.h"
#include "MyCraft.h"
#include <stdlib.h>

void ChunkManager::Update()
{
	for (int distance = 0; distance <= MyCraft::render_distance; distance++)
	{
		chunks_loaded_in_update.clear();

		const int current_chunk_x = Player::position.x / 16 + (Player::position.x < 0 ? -1 : 0);
		const int current_chunk_z = Player::position.z / 16 + (Player::position.z < 0 ? -1 : 0);
		
		if (current_chunk_x != last_chunk_x || current_chunk_z != last_chunk_z)
		{
			auto iterator = chunk_map.begin();
			while (iterator != chunk_map.end())
			{
				const Chunk* chunk = iterator->second;
				if (std::abs(chunk->chunk_x - current_chunk_x) >= MyCraft::render_distance || std::abs(chunk->chunk_z - current_chunk_z) >= MyCraft::render_distance)
				{
					iterator = chunk_map.erase(iterator);
					delete chunk;
				}
				else
				{
					++iterator;
				}
			}
			//wczytywanie chunk na którym znajduje siê gracz
			auto chunk = chunk_map.find(std::make_pair(current_chunk_x, current_chunk_z));
			if (chunk == chunk_map.end())
			{
				LoadChunk(current_chunk_x, current_chunk_z);
			}
			//pêtla wczytuj¹ca chunki dooko³a gracza
			for (int distance = 1; distance < MyCraft::render_distance; distance++)
			{
				for (int chunk_x = current_chunk_x - distance; chunk_x <= current_chunk_x + distance; chunk_x++)
				{
					chunk = chunk_map.find(std::make_pair(chunk_x, current_chunk_z + distance));
					if (chunk == chunk_map.end())
					{
						//nie wczytano chunka
						LoadChunk(chunk_x, current_chunk_z + distance);
					}
					chunk = chunk_map.find(std::make_pair(chunk_x, current_chunk_z - distance));
					if (chunk == chunk_map.end())
					{
						//nie wczytano chunka
						LoadChunk(chunk_x, current_chunk_z - distance);
					}
				}
				for (int chunk_z = current_chunk_z - distance + 1; chunk_z <= current_chunk_z + distance - 1; chunk_z++)
				{
					chunk = chunk_map.find({ current_chunk_x - distance, chunk_z });
					if (chunk == chunk_map.end())
					{
						//nie wczytano chunka
						LoadChunk(current_chunk_x - distance, chunk_z);
					}
					chunk = chunk_map.find({ current_chunk_x + distance, chunk_z });
					if (chunk == chunk_map.end())
					{
						//nie wczytano chunka
						LoadChunk(current_chunk_x + distance, chunk_z);
					}
				}
			}
			last_chunk_x = current_chunk_x;
			last_chunk_z = current_chunk_z;

			for (auto chunk : chunks_loaded_in_update)
			{

			}
		}
	}
}

void ChunkManager::LoadChunk(int chunk_x, int chunk_z)
{
	//Generacja lub Wczytanie czunka
	Chunk* chunk = new Chunk(chunk_x, chunk_z);
	chunk_map.emplace(std::make_pair(chunk_x, chunk_z), chunk);
	chunks_loaded_in_update.push_back(chunk);
	chunk->RecalculateVisibility();
	chunk->UpdateVbos();
}

void ChunkManager::Draw()
{
	for (auto iterator = chunk_map.begin(); iterator != chunk_map.end(); iterator++)
	{
		iterator->second->Draw();
	}
}

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
}

std::map<std::pair<int, int>, Chunk*> ChunkManager::chunk_map;