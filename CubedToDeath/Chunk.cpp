#include "Chunk.h"
#include "cubanCigar.h"

Chunk::Chunk(int chunk_x, int chunk_y)
{
	this->chunk_x = chunk_x;
	this->chunk_y = chunk_y;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[] =
	{ -0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f };

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Chunk::Draw()
{
	CubanCigar::basic_shader->Use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
