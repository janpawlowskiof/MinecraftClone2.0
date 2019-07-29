#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "SimpleBlock.h"
#include "ComplexBlock.h"

class Chunk
{
private:
	int triangles_count[2];
	float* vertices_simple = nullptr;
	float* vertices_complex = nullptr;
public:
	SimpleBlock* blocks[128][16][16];
	int height_values[16][16];
	unsigned int vbo[2];
	unsigned int vao[2];
	int chunk_x, chunk_z;
	bool buffers_initialized = false;
	bool buffers_update_needed = false;
	bool chunk_waiting_to_unload = false;
	bool structures_generated = false;
	void InitializeBuffers();
	Chunk(int chunk_x, int chunk_y);
	~Chunk();
	//void CountVisibleTriangles();
	void RecalculateVisibility(std::map<std::pair<int, int>, Chunk*> chunk_map);
	void RecalculateTrianglesCount();
	void UpdateVboComplex();
	void UpdateVbos();
	void Draw();
	void ReplaceBlock(int block_x, int block_y, int block_z, SimpleBlock* block, bool world_coordinates = true);

	enum Type
	{
		SIMPLE = 0,
		COMPLEX = 1
	};
};

