#include "Chunk.h"
#include "MyCraft.h"
#include "SimpleBlock.h"

void Chunk::InitializeBuffers()
{
	//generating vbo that belongs to chunk
	glGenBuffers(3, vbo);
	glGenVertexArrays(3, vao);

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[FLUID]);
	//generating vao that belongs to chunk
	glBindVertexArray(vao[FLUID]);
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

void Chunk::GenerateStructures()
{
	if (north_chunk && south_chunk && west_chunk && east_chunk &&
		north_chunk->west_chunk && west_chunk->south_chunk && south_chunk->east_chunk && east_chunk->north_chunk)
	{
		//std::cout << "GenSTR " << chunk_x << " " << chunk_z << std::endl;
		//GENERATING STRUCTURES HERE
		float tree_values[18][18];
		for (int x = -1; x < 17; x++)
			for (int z = -1; z < 17; z++)
			{
				tree_values[x + 1][z + 1] = ChunkManager::tree_noise.GetValue(chunk_x * 16 + x, chunk_z * 16 + z);
			}

		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (tree_values[x + 1][z + 1] > tree_values[x][z + 1] &&
					tree_values[x + 1][z + 1] > tree_values[x + 2][z + 1] &&
					tree_values[x + 1][z + 1] > tree_values[x + 1][z + 2] &&
					tree_values[x + 1][z + 1] > tree_values[x + 1][z])
				{
					float tree_placement_value =  clip(ChunkManager::tree_placement_noise.GetValue(chunk_x * 16 + x, chunk_z * 16 + z), 0.0f, 0.6f) / (0.6f);
					if (tree_values[x+1][z+1] > tree_placement_value)
						continue;

					int ground_level = height_values[x][z];

					ReplaceBlock(x, ground_level + 1, z, new SimpleBlock(blk_id::wood_id), false);
					ReplaceBlock(x, ground_level + 2, z, new SimpleBlock(blk_id::wood_id), false);
					ReplaceBlock(x, ground_level + 3, z, new SimpleBlock(blk_id::wood_id), false);
					ReplaceBlock(x, ground_level + 4, z, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x + 1, ground_level + 4, z, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x - 1, ground_level + 4, z, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x + 2, ground_level + 4, z, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x - 2, ground_level + 4, z, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x, ground_level + 4, z + 1, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x, ground_level + 4, z - 1, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x, ground_level + 4, z + 2, new SimpleBlock(blk_id::leaves_id), false);
					ReplaceBlock(x, ground_level + 4, z - 2, new SimpleBlock(blk_id::leaves_id), false);
					//ReplaceBlock(x, ground_level + 3, z + 1, new SimpleBlock(blk_id::leaves_id), false);
					//ReplaceBlock(x, ground_level + 3, z-1, new SimpleBlock(blk_id::leaves_id), false);

				}
			}
		//RecalculateVisibility();
		structures_generated = true;
		visibility_update_needed = true;
	}
}

SimpleBlock* Chunk::GetBlockInArea(int& local_x, int& local_y, int& local_z, Chunk*& chunk)
{
	std::lock_guard<std::mutex> lock(blocks_mutex);

	if (local_z < 0)
	{
		if (local_x < 0)
		{
			chunk = south_chunk->east_chunk;
			local_x = (local_x % 16 + 16)%16;
			local_z = (local_z % 16 + 16)%16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		else if (local_x >= 16)
		{
			chunk = south_chunk->west_chunk;
			local_x = local_x % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		else
		{
			chunk = south_chunk;
			local_x = local_x % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
	}
	else if (local_z >= 16)
	{
		if (local_x < 0)
		{
			chunk = north_chunk->east_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = local_z % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
			//north_chunk->east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z % 16, block, false);
		else if (local_x >= 16)
		{
			chunk = north_chunk->west_chunk;
			local_x = local_x % 16;
			local_z = local_z % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
			//north_chunk->west_chunk->ReplaceBlock(local_x % 16, block_y, local_z % 16, block, false);
		else
		{
			chunk = north_chunk;
			local_x = local_x % 16;
			local_z = local_z % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
			//north_chunk->ReplaceBlock(local_x, block_y, local_z % 16, block, false);
	}
	else
	{
		if (local_x < 0)
		{
			chunk = east_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = local_z % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
			//east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z, block, false);
		else if (local_x >= 16)
		{
			chunk = west_chunk;
			local_x = local_x % 16;
			local_z = local_z % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
			//west_chunk->ReplaceBlock(local_x % 16, block_y, local_z, block, false);
		else
		{
			chunk = this;
			return chunk->blocks[local_y][local_x][local_z];
		}
	}
}

Chunk::Chunk(int chunk_x, int chunk_z)
{
	//std::cout << "scon" << chunk_x << " " << chunk_z << std::endl;

	//init
	this->chunk_x = chunk_x;
	this->chunk_z = chunk_z;

	///			TEST INITIALIZATION			///
	for (int x = 0; x < 16; x++)
		for (int z = 0; z < 16; z++)
		{
			float mountain_lower_threshold = 0.1f;
			float mountain_upper_threshold = 0.6f;
			float ocean_lower_threshold = 0.4f;
			float ocean_upper_threshold = 0.5f;
			int ocean_level = 30;

			float mountain_placement_value = clip(ChunkManager::mountain_placement_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16), mountain_lower_threshold, mountain_upper_threshold);
			float mountain_factor = powf((mountain_placement_value - mountain_lower_threshold) / (mountain_upper_threshold - mountain_lower_threshold), 2);
			float mountain_value = (ChunkManager::test_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16) + 1) * 35 * mountain_factor;

			float tectonical_value = clip(ChunkManager::tectonical_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16), -0.5f, 1) * 10;

			float ocean_placement_value = clip(ChunkManager::ocean_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16), ocean_lower_threshold, ocean_upper_threshold);
			float ocean_factor = powf((ocean_placement_value - ocean_lower_threshold) / (ocean_upper_threshold - ocean_lower_threshold), 2);

			int ground_level = clip(20 + (30 + tectonical_value) * (1 - ocean_factor) + mountain_value, 0, 127);
			height_values[x][z] = ground_level;
			for (int y = 0; y <= ground_level; y++)
			{
				blocks[y][x][z] = new SimpleBlock(blk_id::dirt_id);
			}

			blocks[ground_level][x][z] = new SimpleBlock(blk_id::grass_id);

			for (int y = ground_level+1; y < ocean_level; y++)
			{
				blocks[y][x][z] = SimpleBlock::CreateNew(blk_id::water_id);
			}

			for (int y = std::max(ground_level+1, ocean_level); y < 128; y++)
			{
				blocks[y][x][z] = new SimpleBlock(blk_id::air_id);
			}
		}
	///											///

	north_chunk = ChunkManager::GetChunk(chunk_x, chunk_z + 1);
	if (north_chunk)
	{
		north_chunk->south_chunk = this;
		north_chunk->visibility_update_needed = true;
	}
	south_chunk = ChunkManager::GetChunk(chunk_x, chunk_z - 1);
	if (south_chunk)
	{
		south_chunk->north_chunk = this;
		south_chunk->visibility_update_needed = true;
	}
	west_chunk = ChunkManager::GetChunk(chunk_x + 1, chunk_z);
	if (west_chunk)
	{
		west_chunk->east_chunk = this;
		west_chunk->visibility_update_needed = true;
	}
	east_chunk = ChunkManager::GetChunk(chunk_x - 1, chunk_z);
	if (east_chunk)
	{
		east_chunk->west_chunk = this;
		east_chunk->visibility_update_needed = true;
	}
	RecalculateVisibility();

	//std::cout << "fcon" << chunk_x << " " << chunk_z << std::endl;
}

void Chunk::RecalculateVisibility()
{
	std::lock_guard<std::mutex> lock(blocks_mutex);

	int triangles_count_simple = 0, triangles_count_complex = 0, triangles_count_fluid = 0;
	bool visible = false;
	//chunks in each direciton

	//adding faces to buffors
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				//skipping air
				if (blocks[y][x][z]->id == blk_id::air_id)
					continue;
				//adding simple faces depending on the block position
				if (blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					//adding complex models
					triangles_count_complex += ((ComplexBlock*)blocks[y][x][z])->GetNumberOfTriangles();
				}
				else if (blocks[y][x][z]->GetFlag(SimpleBlock::FLUID))
				{
					if (blocks[y][x][z]->id != blocks[y + 1][x][z]->id)
					{
						blocks[y][x][z]->SetFaceVisible(SimpleBlock::TOP, true);
						triangles_count_fluid += 2;
					}
					else
					{
						blocks[y][x][z]->SetFaceVisible(SimpleBlock::TOP, false);
					}
				}
				else
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
					triangles_count_simple += 2 * visible;

					if (z == 0)
					{
						visible = (south_chunk != nullptr && !south_chunk->blocks[y][x][15]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x][z - 1]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::SOUTH, visible);
					triangles_count_simple += 2 * visible;

					if (x == 15)
					{
						visible = (west_chunk != nullptr && !west_chunk->blocks[y][0][z]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x + 1][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::WEST, visible);
					triangles_count_simple += 2 * visible;

					if (x == 0)
					{
						visible = (east_chunk != nullptr && !east_chunk->blocks[y][15][z]->GetFlag(SimpleBlock::OPAQUE));
					}
					else
					{
						visible = !blocks[y][x - 1][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::EAST, visible);
					triangles_count_simple += 2 * visible;

					if (y >= 127)
					{
						visible = true;
					}
					else
					{
						visible = !blocks[y + 1][x][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::TOP, visible);
					triangles_count_simple += 2 * visible;

					if (y == 0)
					{
						visible = false;
					}
					else
					{
						visible = !blocks[y - 1][x][z]->GetFlag(SimpleBlock::OPAQUE);
					}
					blocks[y][x][z]->SetFaceVisible(SimpleBlock::BOTTOM, visible);
					triangles_count_simple += 2 * visible;
				}
			}
	if (vertices_simple)
	{
		delete[] vertices_simple;
	}
	if (vertices_complex)
	{
		delete[] vertices_complex;
	}
	if (vertices_fluid)
	{
		delete[] vertices_fluid;
	}
	vertices_simple = new float[triangles_count_simple * 3 * 8];
	vertices_complex = new float[triangles_count_complex * 3 * 8];
	vertices_fluid = new float[triangles_count_fluid * 3 * 8];

	//target adress that we will insert our data into
	float* target_simple = vertices_simple;
	float* target_complex = vertices_complex;
	float* target_fluid = vertices_fluid;

	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					target_complex = ((ComplexBlock*)blocks[y][x][z])->CreateModel(target_complex, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
				else if(blocks[y][x][z]->GetFlag(SimpleBlock::FLUID))
				{
					target_fluid = blocks[y][x][z]->CreateModel(target_fluid, x + 16 * chunk_x, y, z + 16 * chunk_z);;
				}
				else
				{
					target_simple = blocks[y][x][z]->CreateModel(target_simple, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
			}
	buffers_update_needed = true;
	visibility_update_needed = false;
	triangles_count[SIMPLE] = triangles_count_simple;
	triangles_count[COMPLEX] = triangles_count_complex;
	triangles_count[FLUID] = triangles_count_fluid;
}


void Chunk::UpdateVbos()
{
	assert(vertices_complex);
	assert(vertices_simple);
	assert(vertices_fluid);
	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[SIMPLE] * 3 * 8 * sizeof(float), vertices_simple, GL_STATIC_DRAW);

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[COMPLEX] * 3 * 8 * sizeof(float), vertices_complex, GL_STATIC_DRAW);

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[FLUID]);
	glBindVertexArray(vao[FLUID]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[FLUID] * 3 * 8 * sizeof(float), vertices_fluid, GL_STATIC_DRAW);

	//deleting vertices array now that we're done
	//delete[] vertices_simple;
	//delete[] vertices_complex;
	//vertices_simple = vertices_complex = nullptr;
	buffers_update_needed = false;
}

void Chunk::DrawSimple()
{
	//binds its vao and draw itself
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[SIMPLE] * 3);
}

void Chunk::DrawComplex()
{
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[COMPLEX] * 3);
}

void Chunk::DrawFluids()
{
	glBindVertexArray(vao[FLUID]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[FLUID] * 3);
}

bool Chunk::InView()
{
	return false;
}

void Chunk::ReplaceBlock(int block_x, int block_y, int block_z, SimpleBlock* block, bool world_coordinates)
{
	std::lock_guard<std::mutex> lock(blocks_mutex);

	//We cannot just delete the block do we queue it for unloading
	//replacing the block
	int local_x;
	int local_z;
	if (world_coordinates)
	{
		local_x = block_x - 16 * chunk_x;
		local_z = block_z - 16 * chunk_z;
	}
	else
	{
		local_x = block_x;
		local_z = block_z;
	}

	if (local_z < 0)
	{
		if (local_x < 0)
			south_chunk->east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z % 16 + 16, block, false);
		else if(local_x >= 16)
			south_chunk->west_chunk->ReplaceBlock(local_x % 16, block_y, local_z % 16 + 16, block, false);
		else
			south_chunk->ReplaceBlock(local_x, block_y, local_z % 16 + 16, block, false);
	}
	else if (local_z >= 16)
	{
		if (local_x < 0)
			north_chunk->east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z % 16, block, false);
		else if (local_x >= 16)
			north_chunk->west_chunk->ReplaceBlock(local_x % 16, block_y, local_z % 16, block, false);
		else
			north_chunk->ReplaceBlock(local_x, block_y, local_z % 16, block, false);
	}
	else
	{
		if (local_x < 0)
			east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z, block, false);
		else if (local_x >= 16)
			west_chunk->ReplaceBlock(local_x % 16, block_y, local_z, block, false);
		else
		{
			ChunkManager::QueueBlockToUnload(blocks[block_y][local_x][local_z]);
			blocks[block_y][local_x][local_z] = block;
			visibility_update_needed = true;
		}
	}
}

Chunk::~Chunk()
{
	//std::cout << "sdest " << chunk_x << +' ' << chunk_z << '\n';
	//Chunk wysy³a dane o swoich buforach do kolejki aby zosta³y poprawnie usuniête
	if (buffers_initialized)
	{
		MyCraft::QueueBuffersToDelete(vbo[SIMPLE], vao[SIMPLE]);
		MyCraft::QueueBuffersToDelete(vbo[COMPLEX], vao[COMPLEX]);
		MyCraft::QueueBuffersToDelete(vbo[FLUID], vao[FLUID]);
	}

	if (vertices_simple != nullptr)
		delete[] vertices_simple;
	if (vertices_complex != nullptr)
		delete[] vertices_complex;
	if (vertices_fluid != nullptr)
		delete[] vertices_fluid;

	for (int y = 0; y < 128; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (!blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
					delete blocks[y][x][z];
				else
					delete ((ComplexBlock*)blocks[y][x][z]);
			}
	//std::cout << "fdest " << chunk_x << " " << chunk_z << '\n';
}
