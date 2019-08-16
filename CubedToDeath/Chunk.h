#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <map>
#include "SimpleBlock.h"
#include <mutex>

class Chunk
{
private:
	int triangles_count[3];
	float* vertices_simple = nullptr;
	float* vertices_complex = nullptr;
	float* vertices_fluid = nullptr;
	std::mutex blocks_mutex;
	std::mutex vertices_mutex;
public:
	SimpleBlock* blocks[128][16][16];
	int height_values[16][16];
	Chunk* north_chunk = nullptr, * south_chunk = nullptr, * west_chunk = nullptr, * east_chunk = nullptr;
	unsigned int vbo[3];
	unsigned int vao[3];
	int chunk_x, chunk_z;
	bool buffers_initialized = false;
	bool vbos_update_needed = false;
	bool vbo_complex_update_needed = false;
	bool recalculate_vbos_needed = true;
	//bool recalculate_complex_vbo_needed = false;
	bool structures_generated = false;
	void InitializeBuffers();
	void GenerateStructures();
	SimpleBlock* GetBlockInArea(int &local_x, int &local_y, int& local_z, Chunk*&chunk);
	Chunk(int chunk_x, int chunk_y);
	~Chunk();
	//void CountVisibleTriangles();
	void RecalculateVbos();
	void RecalculateComplexVbo();
	void UpdateVbos();
	void UpdateVboComplex();
	void DrawSimple();
	void DrawComplex();
	void DrawFluids();
	bool InView();
	void ReplaceBlock(int block_x, int block_y, int block_z, SimpleBlock* block, bool world_coordinates = true);
	enum Type
	{
		SIMPLE = 0,
		COMPLEX = 1,
		FLUID = 2,
	};
};

