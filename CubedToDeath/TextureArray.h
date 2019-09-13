#pragma once
#include <string>

class TextureArray
{
public:
	TextureArray(std::string path);
	void Bind();
private:
	unsigned int texture_id;
	const int texture_size = 128;
	const int layer_count = 80;
	void LoadLayer(int layer, std::string path);
};

