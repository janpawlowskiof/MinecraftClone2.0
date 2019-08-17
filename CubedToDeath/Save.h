#pragma once
#include "SimpleBlock.h"
#include <algorithm>
#include <string>
#include "MyCraft.h"

#define HEADERSIZE sizeof(save::SaveHeader)

namespace save
{
	struct SaveHeader
	{
		int height_values[16][16];
		int chunk_x = 0, chunk_z = 0;
		bool structures_generated = false;
		int max_height = 0;
	};

	std::string GetSavePath(int chunk_x, int chunk_z)
	{
		const std::string filename = MyCraft::config_map["world_path"] + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_z) + ".bin";
		return filename;
	}

	void SaveChunk(Chunk* chunk)
	{
		SaveHeader header;
		memcpy(header.height_values, chunk->height_values, 16 * 16 * sizeof(int));
		header.chunk_x = chunk->chunk_x;
		header.chunk_z = chunk->chunk_z;
		header.structures_generated = chunk->structures_generated;
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
				header.max_height = std::max(header.max_height, header.height_values[x][z]);

		std::ofstream save_file;
		save_file.open(GetSavePath(chunk->chunk_x, chunk->chunk_z), std::ios::out | std::ios::trunc | std::ios::binary);
		save_file.write(reinterpret_cast<const char*>(&header), HEADERSIZE);

		for (int y = 0; y < 127; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
					SimpleBlock::SaveBlockToFile(chunk->blocks[y][x][z], save_file);

		save_file.close();
	}

	void LoadBlocks(SimpleBlock* blocks[16][16][128], char* data)
	{

	}

	Chunk* LoadChunkFromFile(int chunk_x, int chunk_z)
	{
		std::ifstream save_file;
		save_file.open(GetSavePath(chunk_x, chunk_z), std::ios::in | std::ios::binary | std::ios::beg);
		if (!save_file.is_open())
			return nullptr;
		Chunk* chunk = new Chunk(chunk_x, chunk_z);
		SaveHeader header;
		save_file.read(reinterpret_cast<char*>(&header), HEADERSIZE);

		for (int y = 0; y < 128; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
					chunk->blocks[y][x][z] = SimpleBlock::LoadBlockFromFile(save_file);
		/*for (int y = header.max_height + 1; y < 127; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
					chunk->blocks[y][x][z] = SimpleBlock::CreateNew(blk_id::air_id);*/

		save_file.close();
		//memcpy(chunk->height_values, header.height_values, 16 * 16 * sizeof(int));
		//chunk->structures_generated = header.structures_generated;

		//chunk->GenerateTerrain();
		chunk->RecalculateVbos();

		std::cout << "Loading chunk " << chunk_x << " x " << chunk_z <<"\n";
		return chunk;
	}
}
