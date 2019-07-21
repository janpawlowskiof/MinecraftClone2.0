#pragma once
#include <list>
#include <map>
#include "Chunk.h"

class ChunkManager
{
private:
	std::list<Chunk*> chunks_loaded_in_update;

public:
	static std::map<std::pair<int, int>, Chunk*> chunk_map;
	int last_chunk_x = 110, last_chunk_z = 110;
	void Update();
	void LoadChunk(int chunk_x, int chunk_z);
	void Draw();
	ChunkManager();
	~ChunkManager();
};

