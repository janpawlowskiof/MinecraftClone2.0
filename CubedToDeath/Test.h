#pragma once
#include "glm/glm.hpp"
#include "Vertex.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

unsigned int vbo_test;
unsigned int vao_test;
const int simple_stride = 13 * sizeof(float);

void init_test()
{
	glGenBuffers(1, &vbo_test);
	glGenVertexArrays(1, &vao_test);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_test);
	glBindVertexArray(vbo_test);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, simple_stride, (void*)0);
	glEnableVertexAttribArray(0);
	//tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, simple_stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, simple_stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//tex id
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, simple_stride, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//overlay id
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, simple_stride, (void*)(9 * sizeof(float)));
	//overlay color
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, simple_stride, (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(5);
}

void draw_test()
{
	glBindVertexArray(vao_test);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_test);

	glm::vec3 pos00(0, 100, 0), pos01(0, 110, 0), pos10(10, 110, 0), pos11(10, 110, 0);
	/*Vertex v1 = Vertex(pos00, glm::vec2(0, 0), glm::vec3(1, 0, 0), 1, 1, glm::vec3(-1));
	Vertex v2 = Vertex(pos01, glm::vec2(0, 1), glm::vec3(0, 1, 0), 1, 1, glm::vec3(-1));
	Vertex v3 = Vertex(pos11, glm::vec2(1, 1), glm::vec3(0.5, 0.5, 0), 1, 1, glm::vec3(-1));
	std::vector<Vertex> vertices;
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v1);
	vertices.push_back(v3);
	vertices.push_back(v2);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());*/
}