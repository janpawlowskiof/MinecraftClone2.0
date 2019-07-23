#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "SimpleBlock.h"
#include "ComplexBlock.h"

class Chunk
{
private:
	unsigned int vbo[2];
	unsigned int vao[2];
	int triangles_count[2];
	SimpleBlock* blocks[128][16][16];
	enum Type
	{
		SIMPLE = 0,
		COMPLEX = 1
	};
public:
	int chunk_x, chunk_z;
	bool buffers_initialized = false;
	bool buffers_update_needed = false;
	bool chunk_waiting_to_unload = false;
	void InitializeBuffers();
	Chunk(int chunk_x, int chunk_y);
	~Chunk();
	//void CountVisibleTriangles();
	void RecalculateVisibility();
	void RecalculateTrianglesCount();
	void UpdateVboComplex();
	void UpdateVbos();
	void Draw();
	void ReplaceBlock(int world_x, int world_y, int world_z, SimpleBlock* block);
};

