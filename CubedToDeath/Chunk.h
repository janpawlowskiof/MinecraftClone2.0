#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Chunk
{
private:
	unsigned int vbo;
	int chunk_x, chunk_y;

	unsigned int vao;

public:
	Chunk(int chunk_x, int chunk_y);
	void Draw();
};

