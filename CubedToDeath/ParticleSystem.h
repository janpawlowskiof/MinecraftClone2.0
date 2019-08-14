#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 position, glm::vec3 color);
	void Initialize();
	void Start();
	void Update();
	void Render();
	void RenderShadowMap();
	~ParticleSystem();
	double time = 0;
private:
	int grid_size = 6;
	float life_length = 1;
	double start_time;
	bool initialized = false;
	unsigned int vbo, vao;
	glm::vec3 color = glm::vec3(1, 0, 0);
	glm::vec3 position = glm::vec3(10,60,0);
};

