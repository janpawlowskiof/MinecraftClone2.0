#include "Chunk.h"
#include "MyCraft.h"
#include "Block.h"
#include "SimpleBlock.h"

void Chunk::InitializeBuffers()
{
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

	buffers_initialized = true;
}

float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

Chunk::Chunk(int chunk_x, int chunk_z)
{
	//std::cout << "Loaded " << chunk_x << " - " << chunk_z << std::endl;

	//init
	this->chunk_x = chunk_x;
	this->chunk_z = chunk_z;

	///			TEST INITIALIZATION			///
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				float mountain_lower_threshold = 0.2f;
				float mountain_upper_threshold = 0.6f;
				float mountain_placement_value = clip(ChunkManager::mountain_placement_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16), mountain_lower_threshold, mountain_upper_threshold);
				float mountain_factor = powf((mountain_placement_value - mountain_lower_threshold) / (mountain_upper_threshold - mountain_lower_threshold), 2);

				float mountain_value = (ChunkManager::test_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16) + 1) * 35 * mountain_factor;

				float tectonical_value = clip(ChunkManager::tectonical_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16), -0.5f, 1) * 10;

				int ground_level = 30 + tectonical_value + mountain_value;
				for (int y = 0; y < ground_level; y++)
				{
					blocks[y][x][z] = new SimpleBlock(blk_id::dirt_id);
				}

				for (int y = ground_level; y < 128; y++)
				{
					blocks[y][x][z] = new SimpleBlock(blk_id::air_id);
				}
			}

	///											///
}

void Chunk::RecalculateVisibility()
{
	triangles_count[SIMPLE] = triangles_count[COMPLEX] = 0;
	bool visible = false;
	//chunks in each direciton
	auto iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x, chunk_z + 1));
	auto north_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;
	iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x, chunk_z - 1));
	auto south_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;
	iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x + 1, chunk_z));
	auto west_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;
	iterator = ChunkManager::chunk_map.find(std::make_pair(chunk_x - 1, chunk_z));
	auto east_chunk = (iterator != ChunkManager::chunk_map.end()) ? iterator->second : nullptr;

	//adding faces to buffors
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				//skipping air
				if (blocks[y][x][z]->id == blk_id::air_id)
					continue;
				//adding simple faces depending on the block position
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

	vertices_simple = new float[triangles_count[SIMPLE] * 3 * 8];
	vertices_complex = new float[triangles_count[COMPLEX] * 3 * 8];
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
					//2 triangles per each face
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
	//RecalculateVisibility();
	//RecalculateTrianglesCount();

	//We declare array for all of the vertices (*3*8) because each triangle has 3 vertices, each of 8 floats
	/*float* vertices_simple = new float[triangles_count[SIMPLE] * 3 * 8];
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
			}*/

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[SIMPLE] * 3 * 8 * sizeof(float), vertices_simple, GL_STATIC_DRAW);
	
	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[COMPLEX] * 3 * 8 * sizeof(float), vertices_complex, GL_STATIC_DRAW);

	//deleting vertices array now that we're done
	delete[] vertices_simple;
	delete[] vertices_complex;
	vertices_simple = vertices_complex = nullptr;

	buffers_update_needed = false;
}

void Chunk::Draw()
{
	//binds its vao and draw itself
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[SIMPLE] * 3);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[COMPLEX] * 3);
}

void Chunk::ReplaceBlock(int local_x, int local_y, int local_z, SimpleBlock* block)
{
	//We cannot just delete the block do we queue it for unloading
	ChunkManager::QueueBlockForUnload(blocks[local_y][local_x][local_z]);
	//replacing the block
	blocks[local_y][local_x][local_z] = block;
}

Chunk::~Chunk()
{
	glDeleteVertexArrays(2, vao);
	glDeleteBuffers(2, vbo);

	if (vertices_simple != nullptr)
		delete[] vertices_simple;
	if (vertices_complex != nullptr)
		delete[] vertices_complex;

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
