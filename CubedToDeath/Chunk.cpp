#include "Chunk.h"
#include "MyCraft.h"
#include "Block.h"
#include "SimpleBlock.h"

Chunk::Chunk(int chunk_x, int chunk_z)
{
	//init
	this->chunk_x = chunk_x;
	this->chunk_z = chunk_z;

	//generating vbo that belongs to chunk
	glGenBuffers(2, vbo);
	glGenVertexArrays(2, vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	
	//generating vao that belongs to chunk
	glBindVertexArray(vao[SIMPLE]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	//generating vao that belongs to chunk
	glBindVertexArray(vao[COMPLEX]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	///			TEST INITIALIZATION			///
	for (int y = 0; y < 5; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				//blk::Stone* dirt = new blk::Stone(x + chunk_x * 16, y, z + chunk_z * 16);
				//blocks[x][y][z] = dirt;
				blocks[y][x][z] = new SimpleBlock(blk_id::dirt_id);
			}

	for (int y = 5; y < 8; y++)
		for (int x= 0; x < 16; x++)
			for (int z= 0; z < 16; z++)
			{
				//blk::Dirt* air = new blk::Dirt(x + chunk_x * 16, y, z + chunk_z * 16);
				//blocks[x][y][z] = air;
				blocks[y][x][z] = new SimpleBlock(blk_id::stone_id);
			}
	for (int y = 8; y < 128; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				//blk::Air* air = new blk::Air(x + chunk_x * 16, y, z + chunk_z * 16);
				//blocks[x][y][z] = air;
				blocks[y][x][z] = new SimpleBlock(blk_id::air_id);
			}

	ReplaceBlock(new blk::Torch(), 3, 8, 3);
	ReplaceBlock(new SimpleBlock(blk_id::dirt_id), 0, 8, 5);
	//ReplaceBlock(new SimpleBlock(blk_id::dirt_id), 15, 8, 5);

	///											///
}

void Chunk::RecalculateVisibility()
{
	triangles_count[SIMPLE] = triangles_count[COMPLEX] = 0;
	bool visible = false;
	//chunks in each direciton
	auto iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x, chunk_z + 1));
	Chunk* north_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;
	iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x, chunk_z - 1));
	Chunk* south_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;
	iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x + 1, chunk_z));
	Chunk* west_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;
	iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x - 1, chunk_z));
	Chunk* east_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;

	//adding faces to buffors
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				//skipping air
				if (blocks[y][x][z]->id == blk_id::air_id)
					continue;
				//adding simple faces
				if (!blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					if (z == 15)
					{
						visible = (north_chunk != nullptr && !north_chunk->blocks[y][x][0]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x][z + 1]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::NORTH, visible);
					triangles_count[SIMPLE] += 2 * visible;

					if (z == 0)
					{
						visible = (south_chunk != nullptr && !south_chunk->blocks[y][x][15]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x][z - 1]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::SOUTH, visible);
					triangles_count[SIMPLE] += 2 * visible;

					if (x == 15)
					{
						visible = (west_chunk != nullptr && !west_chunk->blocks[y][0][z]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x + 1][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::WEST, visible);
					triangles_count[SIMPLE] += 2 * visible;

					if (x == 0)
					{
						visible = (east_chunk != nullptr && !east_chunk->blocks[y][15][z]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x - 1][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::EAST, visible);
					triangles_count[SIMPLE] += 2 * visible;

					if (y == 128)
					{
						visible = true;
					}
					else
					{
						visible = !blocks[y + 1][x][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::TOP, visible);
					triangles_count[SIMPLE] += 2 * visible;

					if (y == 0)
					{
						visible = true;
					}
					else
					{
						visible = !blocks[y - 1][x][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::BOTTOM, visible);
					triangles_count[SIMPLE] += 2 * visible;
				}
				else
				{
					//adding complex models
					triangles_count[COMPLEX] += ((ComplexBlock*)blocks[y][x][z])->GetNumberOfTriangles();
				}
			}
}

void Chunk::RecalculateTrianglesCount()
{
	triangles_count[SIMPLE] = triangles_count[COMPLEX] = 0;
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (!blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					if (blocks[y][x][z]->id == blk_id::air_id)
						continue;
					triangles_count[SIMPLE] += 
						2 * blocks[y][x][z]->GetFaceVisible(SimpleBlock::NORTH) + 
						2 * blocks[y][x][z]->GetFaceVisible(SimpleBlock::SOUTH) +
						2 * blocks[y][x][z]->GetFaceVisible(SimpleBlock::WEST) +
						2 * blocks[y][x][z]->GetFaceVisible(SimpleBlock::EAST) +
						2 * blocks[y][x][z]->GetFaceVisible(SimpleBlock::TOP) +
						2 * blocks[y][x][z]->GetFaceVisible(SimpleBlock::BOTTOM);
				}
				else
				{
					triangles_count[COMPLEX] += ((ComplexBlock*)blocks[y][x][z])->GetNumberOfTriangles();
				}
			}
}

void Chunk::UpdateVboComplex()
{
	//We declare array for all of the vertices (*3*8) because each triangle has 3 vertices, each of 8 floats
	float* vertices_complex = new float[triangles_count[COMPLEX] * 3 * 8];
	//target adress that we will insert our data into
	float* target_complex = vertices_complex;

	///LEPIEJ LISTA COMPLEXÓW
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					target_complex = ((ComplexBlock*)blocks[y][x][z])->CreateModel(target_complex, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
			}

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[COMPLEX] * 3 * 8 * sizeof(float), vertices_complex, GL_STATIC_DRAW);

	//deleting vertices array now that we're done
	delete vertices_complex;
}

void Chunk::UpdateVbos()
{
	RecalculateTrianglesCount();

	//We declare array for all of the vertices (*3*8) because each triangle has 3 vertices, each of 8 floats
	float* vertices_simple = new float[triangles_count[SIMPLE] * 3 * 8];
	float* vertices_complex = new float[triangles_count[COMPLEX] * 3 * 8];
	//target adress that we will insert our data into
	float* target_simple = vertices_simple;
	float* target_complex = vertices_complex;

	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (!blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					target_simple = blocks[y][x][z]->CreateModel(target_simple, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
				else
				{
					target_complex = ((ComplexBlock*)blocks[y][x][z])->CreateModel(target_complex, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
			}

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[SIMPLE] * 3 * 8 * sizeof(float), vertices_simple, GL_STATIC_DRAW);
	
	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[COMPLEX] * 3 * 8 * sizeof(float), vertices_complex, GL_STATIC_DRAW);

	//deleting vertices array now that we're done
	delete vertices_simple;
	delete vertices_complex;
}

void Chunk::Draw()
{
	//binds its vao and draw itself
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[SIMPLE] * 3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[COMPLEX] * 3);
}

void Chunk::ReplaceBlock(SimpleBlock* block, int local_x, int local_y, int local_z)
{
	delete blocks[local_y][local_x][local_z];
	blocks[local_y][local_x][local_z] = block;
}

Chunk::~Chunk()
{
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);

	for (int y = 0; y < 128; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if(!blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
					delete blocks[y][x][z];
				else
					delete ((ComplexBlock*)blocks[y][x][z]);
			}
}
