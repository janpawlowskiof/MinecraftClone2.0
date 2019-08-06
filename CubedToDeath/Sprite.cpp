#include "Sprite.h"
#include "MyCraft.h"

void Sprite::Draw(int x, int y, int width, int height)
{
	MyCraft::sprite_shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, (float)MyCraft::width, 0.0f, (float)MyCraft::height);
	MyCraft::sprite_shader->SetMat4(MyCraft::sprite_shader->projection_location, projection);
	float vertices[6][4] = {
	{ x, y + height, 0.0, 0.0 },
	{ x, y, 0.0, 1.0 },
	{ x + width, y, 1.0, 1.0 },

	{ x, y + height, 0.0, 0.0 },
	{ x + width, y, 1.0, 1.0 },
	{ x + width, y + height, 1.0, 0.0 } };

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}