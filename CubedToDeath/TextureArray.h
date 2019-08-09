#pragma once
#include <string>

class TextureArray
{
public:
	TextureArray(std::string path);
	void Bind();
private:
	unsigned int texture_id;
	const int mip_map_count = 4;
	const int texture_size = 16;
	const int layer_count = 10;
	void LoadLayer(int layer, std::string path);
};

