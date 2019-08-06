#pragma once
#include <string>
#include "Texture.h"
#include <glad\glad.h>

class Sprite
{
public:
	Sprite(std::string path)
	{
		texture = new Texture(path, 0);
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	}

	void Draw(int x, int y, int width, int height);

	~Sprite()
	{
		delete texture;
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	Texture* texture;
private:
	unsigned int vao, vbo;
};