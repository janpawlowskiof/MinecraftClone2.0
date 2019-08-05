#pragma once
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <iostream>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Sprite.h"

struct Character
{
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class Text {
public:
	Text(std::string path);
	~Text();
	void RenderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);
private:
	std::map<char, Character> Characters;
	FT_Library ft;
	FT_Face face;
	unsigned int vao, vbo;
	Sprite* sprite;
};

