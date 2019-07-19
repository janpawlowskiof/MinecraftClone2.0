#include "Chunk.h"
#include "cubanCigar.h"
#include "Block.h"

Chunk::Chunk(int chunk_x, int chunk_z)
{
	//init
	this->chunk_x = chunk_x;
	this->chunk_z = chunk_z;

	//generating vbo that belongs to chunk
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	//generating vao that belongs to chunk
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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
				blk::Stone* dirt = new blk::Stone(x + chunk_x * 16, y, z + chunk_z * 16);
				blocks[x][y][z] = dirt;
			}

	for (int y = 5; y < 8; y++)
		for (int x= 0; x < 16; x++)
			for (int z= 0; z < 16; z++)
			{
				blk::Dirt* air = new blk::Dirt(x + chunk_x * 16, y, z + chunk_z * 16);
				blocks[x][y][z] = air;
			}
	for (int y = 8; y < 128; y++)
		for (int x = 0; x < 16; x++)
			for (int z = 0; z < 16; z++)
			{
				blk::Air* air = new blk::Air(x + chunk_x * 16, y, z + chunk_z * 16);
				blocks[x][y][z] = air;
			}
	///											///
	UpdateVBO();
}

void Chunk::CountVisibleTriangles()
{
	visible_blocks.clear();
	triangles_count = 0;

	for (int y = 0; y < 128; ++y){
		for (int x = 0; x < 16; ++x){
			for (int z = 0; z < 16; ++z){
				Block *block = blocks[x][y][z];
				bool visible = false;

				//non-opaque blocks are always set to be visible
				if (!block->opaque) {
					//you can't see air
					if (block->id != 0) visible_blocks.push_back(block);
					continue;
				}

				//if block is a cube, sides that are adjacent to non-opaque or non-cube blocks are set to be visible
				if(block->cube){
					if (z + 1 < 16) {
						if (!blocks[x][y][z + 1]->opaque or !blocks[x][y][z + 1]->cube) {
							block->face_visible[Block::NORTH] = true;
							visible = true;
						}
						else
							block->face_visible[Block::NORTH] = false;
					}
					else {
						block->face_visible[Block::NORTH] = true;
						visible = true;
					}

					if (z - 1 >= 0) {
						if (!blocks[x][y][z - 1]->opaque or !blocks[x][y][z - 1]->cube) {
							block->face_visible[Block::SOUTH] = true;
							visible = true;
						}
						else
							block->face_visible[Block::SOUTH] = false;
					}
					else {
						block->face_visible[Block::SOUTH] = true;
						visible = true;
					}

					if (x + 1 < 16) {
						if (!blocks[x + 1][y][z]->opaque or !blocks[x + 1][y][z]->cube) {
							block->face_visible[Block::WEST] = true;
							visible = true;
						}
						else
							block->face_visible[Block::WEST] = false;
					}
					else {
						block->face_visible[Block::WEST] = true;
						visible = true;
					}

					if (x - 1 >= 0) {
						if (!blocks[x - 1][y][z]->opaque or !blocks[x - 1][y][z]->cube) {
							block->face_visible[Block::EAST] = true;
							visible = true;
						}
						else
							block->face_visible[Block::EAST] = false;
					}
					else {
						block->face_visible[Block::EAST] = true;
						visible = true;
					}

					if (y + 1 < 128) {
						if (!blocks[x][y + 1][z]->opaque or !blocks[x][y + 1][z]->cube) {
							block->face_visible[Block::TOP] = true;
							visible = true;
						}			
						else
							block->face_visible[Block::TOP] = false;
					}
					else {
						block->face_visible[Block::TOP] = true;
						visible = true;
					}

					if (y - 1 >= 0) {
						if (!blocks[x][y - 1][z]->opaque or !blocks[x][y - 1][z]->cube) {
							block->face_visible[Block::BOTTOM] = true;
							visible = true;
						}
							
						else
							block->face_visible[Block::BOTTOM] = false;
					}
					else {
						block->face_visible[Block::BOTTOM] = true;
						visible = true;
					}
					//if block is visible it is added to vector of visible blocks
					if (visible) visible_blocks.push_back(block);
				}
				//if non-cube block is aligned to non-opaque or non-cube block , all it's faces are visible 
				else {
				}

				triangles_count += block->GetNumberOfTriangles();
			}
		}
	}	
}

void Chunk::UpdateVBO()
{
	//We count number of triangles that will be drawn
	CountVisibleTriangles();

	std::cout << triangles_count << " triangles\n";

	//We declare array for all of the vertices (*3*8) because each triangle has 3 vertices, each of 8 floats
	float* vertices = new float[triangles_count*3*8];
	//target adress that we will insert our data into
	float* target = vertices;

	for (Block* block : visible_blocks) {
		target = block->CreateModel(target);
	}

	//transfering our data to the gpu
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, triangles_count * 3 * 8 * sizeof(float), vertices, GL_STATIC_DRAW);
	//deleting vertices array now that we're done
	delete vertices;
}

void Chunk::Draw()
{
	//binds it's vao and draw itself
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, triangles_count * 3);
}
