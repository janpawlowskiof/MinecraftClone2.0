#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <algorithm>
#include "Shader.h"
#include "Chunk.h"

class CubanCigar
{
public:
	CubanCigar();
	~CubanCigar();
	void Run();
	static Shader* basic_shader;
private:
	GLFWwindow* window;
	static std::map<std::string, std::string> config_map;
	void LoadConfig(std::string path);
	int height, width;
};