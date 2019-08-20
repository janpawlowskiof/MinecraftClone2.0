#pragma once
#include "SimpleBlock.h"
#include "ComplexBlock.h"
#include <algorithm>
#include <string>
#include "MyCraft.h"
#include <vector>
#include <iterator>

#define HEADERSIZE sizeof(save::SaveHeader)
#define cpx(x) ((ComplexBlock*)x)

namespace save
{
	//header info containing chunk specific data
	struct SaveHeader
	{
		int height_values[16][16];
		int chunk_x = 0, chunk_z = 0;
		bool structures_generated = false;
		int max_height = 0;
	};

	//returns path to save file for chunk of given coordinates
	std::string GetSavePath(int chunk_x, int chunk_z)
	{
		const std::string filename = MyCraft::config_map["world_path"] + "/" + std::to_string(chunk_x) + "x" + std::to_string(chunk_z) + ".bin";
		return filename;
	}

	//saves chunk to a file
	void SaveChunk(Chunk* chunk)
	{
		//		1)	a header is written into a file
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

		const int block_save_data_size = 1;

		//			2) each blocks is asked how much data it will write to a file
		///counting buffer size
		int blocks_data_size = 0;
		for (int y = 0; y < 128; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
					if (!chunk->blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
						blocks_data_size += block_save_data_size;
					else
						blocks_data_size += block_save_data_size + cpx(chunk->blocks[y][x][z])->GetBlockSpecificMetaSize();

		//			3) a big enough buffer is created and filled
		std::vector<char> blocks_data;
		blocks_data.resize(blocks_data_size);
		char* blocks_data_pointer = blocks_data.data();

		for (int y = 0; y < 128; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
				{
					blocks_data_pointer = SimpleBlock::SaveBlockToFile(chunk->blocks[y][x][z], blocks_data_pointer);
					if (chunk->blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
					{
						cpx(chunk->blocks[y][x][z])->WriteBlockSpecificMeta(blocks_data_pointer);
					}
				}

		//				4) block data buffer is written to a file
		save_file.write(blocks_data.data(), blocks_data_size);
		save_file.close();
	}

	//loads a chunk of given coordinates from a file
	Chunk* LoadChunkFromFile(int chunk_x, int chunk_z)
	{
		std::ifstream save_file;
		save_file.open(GetSavePath(chunk_x, chunk_z), std::ios::in | std::ios::binary | std::ios::beg);
		//returning nullptr if chunk hasnt been saved before
		if (!save_file.is_open())
			return nullptr;

		//		1) header is read from a file
		Chunk* chunk = new Chunk(chunk_x, chunk_z);
		SaveHeader header;
		save_file.read(reinterpret_cast<char*>(&header), HEADERSIZE);
		chunk->structures_generated;
		memcpy(chunk->height_values, header.height_values, 16 * 16 * sizeof(int));
		save_file.unsetf(std::ios::skipws);

		//		2) amount of block data is checked
		auto position = save_file.tellg();
		save_file.seekg(0, std::ios::end);
		const int block_data_size = save_file.tellg() - position;
		save_file.seekg(position);

		//		3) block data buffer is declared and filled
		std::vector<char> blocks_data;
		blocks_data.resize(block_data_size);
		save_file.read(blocks_data.data(), block_data_size);
		char* blocks_data_pointer = blocks_data.data();

		//		4) blocks are creted
		for (int y = 0; y < 128; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
					chunk->blocks[y][x][z] = nullptr;
		for (int y = 0; y < 128; y++)
			for (int x = 0; x < 16; x++)
				for (int z = 0; z < 16; z++)
				{
					// as some blocks are made out of two parts, we check if current block has been filled and if so, we progress the data pointer by one, thus skipping one block
					if (chunk->blocks[y][x][z] != nullptr)
					{
						blocks_data_pointer++;
					}
					else
					{
						chunk->blocks[y][x][z] = SimpleBlock::LoadBlockFromFile(glm::ivec3(x + 16 * chunk->chunk_x,y,z + 16 * chunk->chunk_z), chunk, blocks_data_pointer);
					}
				}

		save_file.close();
		chunk->RecalculateVbos();
		//std::cout << "Loading chunk " << chunk_x << " x " << chunk_z <<"\n";
		return chunk;
	}
}
