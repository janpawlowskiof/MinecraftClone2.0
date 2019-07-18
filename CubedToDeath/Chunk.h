#pragma once
#include "Block.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class Chunk
{
private:
	unsigned int vbo;
	int chunk_x, chunk_z;
	Block* blocks[16][128][16];

	unsigned int vao;
	int triangles_count = 0;
public:
	Chunk(int chunk_x, int chunk_y);
	void RecalculateVisibility();
	void UpdateVBO();
	void Draw();
};

