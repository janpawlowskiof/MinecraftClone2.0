#include "Chunk.h"
#include "MyCraft.h"
#include "SimpleBlock.h"
#include "ComplexBlock.h"


float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

Chunk::Chunk(int chunk_x, int chunk_z)
{
	//init
	this->chunk_x = chunk_x;
	this->chunk_z = chunk_z;

	for (int x = 0; x < 17; x++)
		for (int z = 0; z < 17; z++)
			moisture_values[x][z] = ChunkManager::moisture_noise.GetValue(x + 16 * chunk_x, z + 16 * chunk_z)/2.0 + 0.5;
}

const int fluid_stride = 11 * sizeof(float);
//const int sizeof(Vertex) = 12 * sizeof(float);

void Chunk::InitializeBuffers()
{
	//generating vbo that belongs to chunk
	glGenBuffers(3, vbo);
	glGenVertexArrays(3, vao);

	//generating vao that belongs to chunk
	//pos
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(1);
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	//tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);
	//texture_info
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_info));
	glEnableVertexAttribArray(4);
	//overlay_info
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, overlay_info));
	glEnableVertexAttribArray(5);
	//overlay color
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(6);

	//generating vao that belongs to chunk
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	//tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(1);
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	//tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);
	//texture_info
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_info));
	glEnableVertexAttribArray(4);
	//overlay_info
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, overlay_info));
	glEnableVertexAttribArray(5);
	//overlay color
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(6);

	//generating vao that belongs to chunk
	glBindBuffer(GL_ARRAY_BUFFER, vbo[FLUID]);
	glBindVertexArray(vao[FLUID]);
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, fluid_stride, (void*)0);
	glEnableVertexAttribArray(0);
	//tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, fluid_stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, fluid_stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, fluid_stride, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	buffers_initialized = true;
}

void Chunk::GenerateTerrain()
{
	float noiseValues[5][17][5];

	for (int x = 0; x < 16; x++)
		for (int z = 0; z < 16; z++)
			height_values[x][z] = 0;

	for (int x = 0; x < 5; ++x)
		for (int y = 0; y < 17; ++y)
			for (int z = 0; z < 5; ++z)
				noiseValues[x][y][z] = ChunkManager::d3_noise.GetValue(x + chunk_x * 4, y * 2, z + chunk_z * 4);

	//3d terrain generation
	for (int xs = 0; xs < 4; ++xs) //each subchunk
		for (int zs = 0; zs < 4; ++zs)
			for (int xb = 0; xb < 4; ++xb) //each block in subchunk
				for (int zb = 0; zb < 4; ++zb) {
					int x = xs * 4 + xb;
					int z = zs * 4 + zb;
					//int ground_level = height_values[x][z];

					float mountaininess = clip(ChunkManager::tectonical_noise.GetValue(x + chunk_x * 16, z + chunk_z * 16)/2.0 + 0.25, 0, 0.5) * 2;
					//float mountaininess = 1;

					for (int ys = 0; ys < 16; ++ys) {
						for (int yb = 0; yb < 8; ++yb) {
							int y = 8 * ys + yb;

							float xd = xb / 4.0;
							float yd = yb / 8.0;
							float zd = zb / 4.0;

							float c000 = noiseValues[xs][ys][zs];
							float c001 = noiseValues[xs][ys][zs + 1];
							float c010 = noiseValues[xs][ys + 1][zs];
							float c011 = noiseValues[xs][ys + 1][zs + 1];
							float c100 = noiseValues[xs + 1][ys][zs];
							float c101 = noiseValues[xs + 1][ys][zs + 1];
							float c110 = noiseValues[xs + 1][ys + 1][zs];
							float c111 = noiseValues[xs + 1][ys + 1][zs + 1];

							float c00 = c000 * (1 - xd) + c100 * xd;
							float c01 = c001 * (1 - xd) + c101 * xd;
							float c10 = c010 * (1 - xd) + c110 * xd;
							float c11 = c011 * (1 - xd) + c111 * xd;

							float c0 = c00 * (1 - yd) + c10 * yd;
							float c1 = c01 * (1 - yd) + c11 * yd;

							float c = c0 * (1 - zd) + c1 * zd;
							c = -1 + (c + 1) * mountaininess;

							float height_influence = 0.95;
							float map_influence = 0.2;

							float density = map_influence * c + height_influence * (128 - y) / 128.0f;

							if (density > (0.33) * (height_influence + map_influence)) {
								blocks[y][x][z] = new SimpleBlock(blk_id::stone_id);
								height_values[x][z] = std::max(height_values[x][z], y);
							}
							else
							{
								if (y > sea_level) {
									blocks[y][x][z] = new SimpleBlock(blk_id::air_id);
									if (blocks[y - 1][x][z]->id == blk_id::stone_id)
									{
										delete blocks[y - 1][x][z];
										blocks[y - 1][x][z] = new SimpleBlock(blk_id::grass_id);
									}
								}
								else
									blocks[y][x][z] = new SimpleBlock(blk_id::water_id);
							}
						}
					}
				}
}

//generates structures
void Chunk::GenerateStructures()
{
	//only if all neighbour and diagonal chunks are loaded
	if (north_chunk && south_chunk && west_chunk && east_chunk &&
		north_chunk->west_chunk && west_chunk->south_chunk && south_chunk->east_chunk && east_chunk->north_chunk)
	{
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
				//tree is generated only on a local extremums of tree noise map
				if (tree_values[x + 1][z + 1] > tree_values[x][z + 1] &&
					tree_values[x + 1][z + 1] > tree_values[x + 2][z + 1] &&
					tree_values[x + 1][z + 1] > tree_values[x + 1][z + 2] &&
					tree_values[x + 1][z + 1] > tree_values[x + 1][z])
				{
					float tree_placement_value = clip(ChunkManager::tree_placement_noise.GetValue(chunk_x * 16 + x, chunk_z * 16 + z), 0.0f, 0.6f) / (0.6f);
					if (tree_values[x + 1][z + 1] > tree_placement_value || height_values[x+1][z+1] < sea_level)
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
					ReplaceBlock(x, ground_level + 4, z - 2, SimpleBlock::CreateNew(blk_id::leaves_id), false);
					//ReplaceBlock(x, ground_level + 5, z, SimpleBlock::CreateNew(blk_id::torch_id), false);
					//ReplaceBlock(x, ground_level + 3, z + 1, new SimpleBlock(blk_id::leaves_id), false);
					//ReplaceBlock(x, ground_level + 3, z-1, new SimpleBlock(blk_id::leaves_id), false);
				}
			}
		//RecalculateVisibility();
		structures_generated = true;
		recalculate_vbos_needed = true;
	}
}

//returns a block in the area of the chunk (in it or in its neighbours or diagonals)
//also set the chunk to the parent chunk of returned block
SimpleBlock* Chunk::GetBlockInArea(int& local_x, int& local_y, int& local_z, Chunk*& chunk)
{
	std::lock_guard<std::mutex> lock(blocks_mutex);

	if (local_z < 0)
	{
		if (local_x < 0)
		{
			if (south_chunk == nullptr || south_chunk->east_chunk == nullptr)
				return nullptr;
			chunk = south_chunk->east_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		else if (local_x >= 16)
		{
			if (south_chunk == nullptr || south_chunk->west_chunk == nullptr)
				return nullptr;
			chunk = south_chunk->west_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		else
		{
			if (south_chunk == nullptr)
				return nullptr;
			chunk = south_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
	}
	else if (local_z >= 16)
	{
		if (local_x < 0)
		{
			if (north_chunk == nullptr || north_chunk->east_chunk== nullptr)
				return nullptr;
			chunk = north_chunk->east_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		//north_chunk->east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z % 16, block, false);
		else if (local_x >= 16)
		{
			if (north_chunk == nullptr || north_chunk->west_chunk== nullptr)
				return nullptr;
			chunk = north_chunk->west_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		//north_chunk->west_chunk->ReplaceBlock(local_x % 16, block_y, local_z % 16, block, false);
		else
		{
			if (north_chunk == nullptr)
				return nullptr;
			chunk = north_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		//north_chunk->ReplaceBlock(local_x, block_y, local_z % 16, block, false);
	}
	else
	{
		if (local_x < 0)
		{
			if (east_chunk == nullptr)
				return nullptr;
			chunk = east_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
			return chunk->blocks[local_y][local_x][local_z];
		}
		//east_chunk->ReplaceBlock(local_x % 16 + 16, block_y, local_z, block, false);
		else if (local_x >= 16)
		{
			if (west_chunk == nullptr)
				return nullptr;
			chunk = west_chunk;
			local_x = (local_x % 16 + 16) % 16;
			local_z = (local_z % 16 + 16) % 16;
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

//recalculates all of the vbos WITHOUT submitting them to the gpu
void Chunk::RecalculateVbos()
{
	const int lod_level = 1;
	std::lock_guard<std::mutex> lock(blocks_mutex);

	int triangles_count_simple = 0, triangles_count_complex = 0, triangles_count_fluid = 0;
	bool visible = false;
	//chunks in each direciton

	/*Chunk* chunk = this;
	SimpleBlock* block;
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x += lod_level)
			for (int z = 0; z < 16; z += lod_level)
			{
				int current_x, current_y, current_z;
				current_y = y + 1;
				if ((block = GetBlockInArea(x, current_y, z, chunk))->GetFlag(SimpleBlock::OPAQUE))
				{
					block->SetFaceVisible(SimpleBlock::TOP, true);
					triangles_count_simple += 2;
				}
				else
				{
					block->SetFaceVisible(SimpleBlock::TOP, false);
				}
				chunk = this;
				current_y = y - 1;
				if ((block = GetBlockInArea(x, current_y, z, chunk))->GetFlag(SimpleBlock::OPAQUE))
				{
					block->SetFaceVisible(SimpleBlock::BOTTOM, true);
					triangles_count_simple += 2;
				}
				else
				{
					block->SetFaceVisible(SimpleBlock::BOTTOM, false);
				}
				chunk = this;
				current_z = z + lod_level;
				if ((block = GetBlockInArea(x, y, current_z, chunk))->GetFlag(SimpleBlock::OPAQUE))
				{
					block->SetFaceVisible(SimpleBlock::NORTH, true);
					triangles_count_simple += 2;
				}
				else
				{
					block->SetFaceVisible(SimpleBlock::NORTH, false);
				}
				chunk = this;
				current_z = z - lod_level;
				if ((block = GetBlockInArea(x, y, current_z, chunk))->GetFlag(SimpleBlock::OPAQUE))
				{
					block->SetFaceVisible(SimpleBlock::SOUTH, true);
					triangles_count_simple += 2;
				}
				else
				{
					block->SetFaceVisible(SimpleBlock::SOUTH, false);
				}
				chunk = this;
				current_x = x + lod_level;
				if ((block = GetBlockInArea(current_x, y, z, chunk))->GetFlag(SimpleBlock::OPAQUE))
				{
					block->SetFaceVisible(SimpleBlock::WEST, true);
					triangles_count_simple += 2;
				}
				else
				{
					block->SetFaceVisible(SimpleBlock::WEST, false);
				}
				chunk = this;
				current_x = x - lod_level;
				if ((block = GetBlockInArea(current_x, y, z, chunk))->GetFlag(SimpleBlock::OPAQUE))
				{
					block->SetFaceVisible(SimpleBlock::EAST, true);
					triangles_count_simple += 2;
				}
				else
				{
					block->SetFaceVisible(SimpleBlock::EAST, false);
				}
			}*/

	//adding faces to buffors
	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x ++)
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

	std::lock_guard<std::mutex> lock_vertices(vertices_mutex);
	/*if (vertices_simple)
	{
		delete[] vertices_simple;
	}*/
	/*if (vertices_complex)
	{
		delete[] vertices_complex;
	}*/
	if (vertices_fluid)
	{
		delete[] vertices_fluid;
	}
	//vertices_simple = new float[triangles_count_simple * 3 * sizeof(Vertex) / sizeof(float)];
	vertices_simple.clear();
	vertices_simple.reserve(triangles_count_simple * 3);
	vertices_complex.clear();
	vertices_complex.reserve(triangles_count_complex * 3);
	//vertices_complex = new float[triangles_count_complex * 3 * 9];
	vertices_fluid = new float[triangles_count_fluid * 3 * fluid_stride / sizeof(float)];

	//target adress that we will insert our data into
	//float* target_simple = vertices_simple;
	//float* target_complex = vertices_complex;
	float* target_fluid = vertices_fluid;

	bool drawn[128][16][16] = { 0 };

	//triangles_count_simple = 0;

	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++) 
			for (int z = 0; z < 16; z++)
			{
				if (blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					((ComplexBlock*)blocks[y][x][z])->CreateModel(vertices_complex, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
				else if (blocks[y][x][z]->GetFlag(SimpleBlock::FLUID))
				{
					glm::vec3 color00(0, 1 - moisture_values[x][z], 0.9);
					glm::vec3 color01(0, 1 - moisture_values[x][z + 1], 0.9);
					glm::vec3 color10(0, 1 - moisture_values[x + 1][z], 0.9);
					glm::vec3 color11(0, 1 - moisture_values[x + 1][z + 1], 0.9);
					target_fluid = blocks[y][x][z]->CreateFluidModel(target_fluid, x + 16 * chunk_x, y, z + 16 * chunk_z, color00, color01, color10, color11);
				}
				else
				{
					//if (drawn[y][x][z])
					//	continue;
					//int z_offset = 0;
					/*while (z + z_offset + 1 < 16 && blocks[y][x][z]->id == blocks[y][x][z + z_offset]->id)
					{
						z_offset++;
					}*/
					//triangles_count_simple += 2;
					glm::vec3 color00(0.6 * (moisture_values[x][z]), 0.6, 0.3*(1 - moisture_values[x][z]));
					glm::vec3 color01(0.6 * (moisture_values[x][z + 1]), 0.6, 0.3 * (1 - moisture_values[x][z + 1]));
					glm::vec3 color10(0.6 * (moisture_values[x + 1][z]), 0.6, 0.3 * (1 - moisture_values[x + 1][z]));
					glm::vec3 color11(0.6 * (moisture_values[x + 1][z + 1]), 0.6, 0.3 * (1 - moisture_values[x + 1][z + 1]));
					blocks[y][x][z]->CreateSolidModel(vertices_simple, x + 16 * chunk_x, y, z + 16 * chunk_z, color00, color01, color10, color11, 1, 1);
					//target_simple = blocks[y][x][z]->CreateSolidModel(target_simple, x + 16 * chunk_x, y, z + 16 * chunk_z, color00, color01, color10, color11, 1, z_offset + 1);
					//z += z_offset;
				}
			}
	vbos_update_needed = true;
	recalculate_vbos_needed = false;
	triangles_count[SIMPLE] = triangles_count_simple;
	triangles_count[COMPLEX] = triangles_count_complex;
	triangles_count[FLUID] = triangles_count_fluid;
}

//recalculates the complex blocks vbo WITHOUT submitting it to the gpu
void Chunk::RecalculateComplexVbo()
{
	std::lock_guard<std::mutex> lock(blocks_mutex);

	if (chunk_x == 0 && chunk_z == -1)
		std::cout << "Eyyy\n";

	int triangles_count_complex = 0;

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
			}
			
	std::lock_guard<std::mutex> lock_vertices(vertices_mutex);
	//if (vertices_complex)
	//{
	//	delete[] vertices_complex;
	//}
	//vertices_complex = new float[triangles_count_complex * 3 * 9];

	//float* target_complex = vertices_complex;
	vertices_complex.clear();
	vertices_complex.reserve(triangles_count_complex);

	for (int y = 0; y < 127; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				if (blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
				{
					((ComplexBlock*)blocks[y][x][z])->CreateModel(vertices_complex, x + 16 * chunk_x, y, z + 16 * chunk_z);
				}
			}
	//recalculate_complex_vbo_needed = true;
	vbo_complex_update_needed = true;
	triangles_count[COMPLEX] = triangles_count_complex;
}

//resubmits the vertex data of complex blocks only to the gpu
void Chunk::UpdateVboComplex()
{
	std::lock_guard<std::mutex> lock_vertices(vertices_mutex);
	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glBufferData(GL_ARRAY_BUFFER, vertices_complex.size() * sizeof(Vertex), vertices_complex.data(), GL_STATIC_DRAW);
	vbo_complex_update_needed = false;
}


//resubmits the vertex data to the gpu
void Chunk::UpdateVbos()
{
	std::lock_guard<std::mutex> lock_vertices(vertices_mutex);
	//assert(vertices_complex);
	//assert(vertices_simple);
	assert(vertices_fluid);
	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glBufferData(GL_ARRAY_BUFFER, vertices_simple.size() * sizeof(Vertex), vertices_simple.data(), GL_STATIC_DRAW);

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glBufferData(GL_ARRAY_BUFFER, vertices_complex.size() * sizeof(Vertex), vertices_complex.data(), GL_DYNAMIC_DRAW);

	//transfering our data to the gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo[FLUID]);
	glBindVertexArray(vao[FLUID]);
	glBufferData(GL_ARRAY_BUFFER, triangles_count[FLUID] * 3 * fluid_stride, vertices_fluid, GL_STATIC_DRAW);

	//deleting vertices array now that we're done
	//delete[] vertices_simple;
	//delete[] vertices_complex;
	//vertices_simple = vertices_complex = nullptr;
	vbos_update_needed = false;
}

//draws simple blocks
void Chunk::DrawSimple()
{
	//binds its vao and draw itself
	glBindBuffer(GL_ARRAY_BUFFER, vbo[SIMPLE]);
	glBindVertexArray(vao[SIMPLE]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[SIMPLE] * 3);
}

//draws complex blocks
void Chunk::DrawComplex()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COMPLEX]);
	glBindVertexArray(vao[COMPLEX]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[COMPLEX] * 3);
}

//draws fluids
void Chunk::DrawFluids()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[FLUID]);
	glBindVertexArray(vao[FLUID]);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count[FLUID] * 3);
}

//finds all neighbour blocks
void Chunk::FindNeighbours()
{
	north_chunk = ChunkManager::GetChunk(chunk_x, chunk_z + 1);
	if (north_chunk)
	{
		north_chunk->south_chunk = this;
		north_chunk->recalculate_vbos_needed = true;
	}
	south_chunk = ChunkManager::GetChunk(chunk_x, chunk_z - 1);
	if (south_chunk)
	{
		south_chunk->north_chunk = this;
		south_chunk->recalculate_vbos_needed = true;
	}
	west_chunk = ChunkManager::GetChunk(chunk_x + 1, chunk_z);
	if (west_chunk)
	{
		west_chunk->east_chunk = this;
		west_chunk->recalculate_vbos_needed = true;
	}
	east_chunk = ChunkManager::GetChunk(chunk_x - 1, chunk_z);
	if (east_chunk)
	{
		east_chunk->west_chunk = this;
		east_chunk->recalculate_vbos_needed = true;
	}
}

void Chunk::InitializeComplexBlocks()
{
	for (int y = 0; y < 128; y++)
		for (int x = 0; x < 16; x++)
			for(int z = 0; z < 16; z++)
			{
				if (blocks[y][x][z]->GetFlag(SimpleBlock::COMPLEX))
					((ComplexBlock*)blocks[y][x][z])->OnLoadFinalization();
			}
}

//why its here idk
bool Chunk::InView()
{
	return false;
}

void Chunk::ReplaceBlock(int block_x, int block_y, int block_z, SimpleBlock* block, bool world_coordinates)
{
	std::unique_lock<std::mutex> lock(blocks_mutex);
	//blocks_mutex.lock();

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
		else if (local_x >= 16)
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
			auto destroyed_block = blocks[block_y][local_x][local_z];
			blocks[block_y][local_x][local_z] = block;
			if (destroyed_block != nullptr)
			{
				ChunkManager::QueueBlockToUnload(destroyed_block);
				if (destroyed_block->GetFlag(SimpleBlock::COMPLEX))
				{
					lock.unlock();
					((ComplexBlock*)destroyed_block)->OnDestroy();
				}
			}
			recalculate_vbos_needed = true;
		}
	}

	save_needed = true;
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

	/*if (vertices_simple != nullptr)
		delete[] vertices_simple;
	if (vertices_complex != nullptr)
		delete[] vertices_complex;*/
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

int Chunk::sea_level = 51;