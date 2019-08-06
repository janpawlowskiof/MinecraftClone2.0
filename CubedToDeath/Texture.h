#pragma once
#include <string>

class Texture
{
public:
	Texture(std::string path, int index);
	void Bind();
	unsigned int texture_id;
private:
};

