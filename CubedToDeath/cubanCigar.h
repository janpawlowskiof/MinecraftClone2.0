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
#include "Player.h"

class CubanCigar
{
public:
	CubanCigar();
	~CubanCigar();
	void Run();
	static void mouse_callback(GLFWwindow* window, double x, double y);
	static Shader* basic_shader;
	static std::map<std::string, std::string> config_map;
	static GLFWwindow* window;
	static Player* player;
	static int height, width;
private:
	void LoadConfig(std::string path);
	static bool first_mouse;
	static double last_x, last_y;
};