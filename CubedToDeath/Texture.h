#pragma once
#include <string>

class Texture
{
public:
	Texture(std::string path);
	void Bind();
private:
	unsigned int texture_id;
};

