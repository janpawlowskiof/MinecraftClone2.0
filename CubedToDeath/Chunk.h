#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class Chunk
{
private:
	unsigned int vbo;
	int chunk_x, chunk_y;

	unsigned int vao;
	int vertices_count = 0;
public:
	Chunk(int chunk_x, int chunk_y);
	void Draw();
};

