#include "Chunk.h"
#include "cubanCigar.h"
#include "Block.h"

Chunk::Chunk(int chunk_x, int chunk_y)
{
	//init
	this->chunk_x = chunk_x;
	this->chunk_y = chunk_y;

	//generating vbo that belongs to chunk
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data2[] =
	{ -0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0, 0, 1,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0, 0, 1,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0, 0, 1,
	};

	
	/*float data[] =
	{
	-2.0f,-2.0f,-2.0f,
	-2.0f, 2.0f, -2.0f,
	2,-2.0f,-2.0f,

	-2.0f,-2.0f,2.0f,
	-2.0f, 2.0f, 2.0f,
	2,-2.0f, 2.0f,

	-2.0f,-2.0f,-2.0f,
	-2.0f,-2.0f,2.0f,
	-2.0f,2.0f,-2.0f,

	2.0f,-2.0f,-2.0f,
	2.0f,-2.0f,2.0f,
	2.0f,2.0f,-2.0f,
	};*/
	
	blocks::Dirt dirt;
	blocks::Stone stone;
	blocks::Grass grass;
	stone.world_x = 1;
	grass.world_x = 1;
	grass.world_y = 1;

	std::vector<float> dirt_vertices = dirt.GetModel();
	vertices_count = dirt_vertices.size();
	std::vector<float> stone_vertices = stone.GetModel();
	vertices_count += stone_vertices.size();
	std::vector<float> grass_vertices = grass.GetModel();
	vertices_count += grass_vertices.size();
	std::vector<float> vertices;
	vertices.reserve(vertices_count);
	vertices.insert(vertices.end(), dirt_vertices.data(), dirt_vertices.data() + sizeof(dirt_vertices.data()));
	vertices.insert(vertices.end(), stone_vertices.data(), stone_vertices.data() + sizeof(dirt_vertices.data()));
	vertices.insert(vertices.end(), grass_vertices.data(), grass_vertices.data() + sizeof(dirt_vertices.data()));

	glBufferData(GL_ARRAY_BUFFER, grass_vertices.size() * sizeof(float), grass_vertices.data(), GL_STATIC_DRAW);

	//generating vao that belongs to chunk
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Chunk::Draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices_count);
}
