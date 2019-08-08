#include "ParticleSystem.h"
#include "MyCraft.h"

ParticleSystem::ParticleSystem(glm::vec3 position, glm::vec3 color)
{
	this->position = position;
	this->color = color;
}

void ParticleSystem::Initialize()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	initialized = true;


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	const float vertices[] = {
		 0.0f,  0.0f,  1.0f, 0, 0, 1,
		 1.0f,  0.0f,  1.0f, 0, 0, 1,
		 1.0f,  1.0f,  1.0f, 0, 0, 1,

		 0.0f,  0.0f,  1.0f, 0, 0, 1,
		 1.0f,  1.0f,  1.0f, 0, 0, 1,
		 0.0f,  1.0f,  1.0f, 0, 0, 1,

		 0.0f,  0.0f,  0.0f, 0, 0, -1,
		 1.0f,  1.0f,  0.0f, 0, 0, -1,
		 1.0f,  0.0f,  0.0f, 0, 0, -1,

		 0.0f,  0.0f,  0.0f, 0, 0, -1,
		 0.0f,  1.0f,  0.0f, 0, 0, -1,
		 1.0f,  1.0f,  0.0f, 0, 0, -1,

		 0.0f,  1.0f,  0.0f, 0, 1, 0,
		 0.0f,  1.0f,  1.0f, 0, 1, 0,
		 1.0f,  1.0f,  1.0f, 0, 1, 0,

		 0.0f,  1.0f,  0.0f, 0, 1, 0,
		 1.0f,  1.0f,  1.0f, 0, 1, 0,
		 1.0f,  1.0f,  0.0f, 0, 1, 0,

		 0.0f,  0.0f,  0.0f, 0, -1, 0,
		 1.0f,  0.0f,  1.0f, 0, -1, 0,
		 0.0f,  0.0f,  1.0f, 0, -1, 0,

		 0.0f,  0.0f,  0.0f, 0, -1, 0,
		 1.0f,  0.0f,  0.0f, 0, -1, 0,
		 1.0f,  0.0f,  1.0f, 0, -1, 0,

		 1.0f,  0.0f,  0.0f, 1, 0, 0,
		 1.0f,  1.0f,  1.0f, 1, 0, 0,
		 1.0f,  0.0f,  1.0f, 1, 0, 0,

		 1.0f,  0.0f,  0.0f, 1, 0, 0,
		 1.0f,  1.0f,  0.0f, 1, 0, 0,
		 1.0f,  1.0f,  1.0f, 1, 0, 0,

		 0.0f,  0.0f,  0.0f, -1, 0, 0,
		 0.0f,  0.0f,  1.0f, -1, 0, 0,
		 0.0f,  1.0f,  1.0f, -1, 0, 0,

		 0.0f,  0.0f,  0.0f, -1, 0, 0,
		 0.0f,  1.0f,  1.0f, -1, 0, 0,
		 0.0f,  1.0f,  0.0f, -1, 0, 0,
	};

	glBufferData(GL_ARRAY_BUFFER, 12 * 3 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);
}

void ParticleSystem::Start()
{
	start_time = MyCraft::current_time;
	time = 0;
}

void ParticleSystem::Update()
{
	time = MyCraft::current_time - start_time;
}

void ParticleSystem::Render()
{
	glUniform1f(MyCraft::particle_shader->time_location, time);
	glUniform3f(MyCraft::particle_shader->color_location, color.x, color.y, color.z);
	auto model = glm::mat4(1);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(1.0f/ grid_size));
	Shader::SetMat4(MyCraft::particle_shader->model_location, model);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	//for(int i = 0; i < 125; i++)
	//	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6 * 2 * 3, grid_size* grid_size* grid_size);
}

void ParticleSystem::RenderShadowMap()
{
	glUniform1f(MyCraft::particle_depth_shader->time_location, time);
	auto model = glm::mat4(1);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(1.0f / grid_size));
	Shader::SetMat4(MyCraft::particle_depth_shader->model_location, model);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	//for (int i = 0; i < 125; i++)
	//	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6 * 2 * 3, grid_size * grid_size * grid_size);
}

ParticleSystem::~ParticleSystem()
{
	if (initialized)
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
}
