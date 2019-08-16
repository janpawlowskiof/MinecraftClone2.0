#pragma once

#include "SimpleBlock.h"
#include "ChunkManager.h"

class BlockUpdater
{
public:
	static void Update();
private:
	static chunk_hash_map chunk_map;
};

