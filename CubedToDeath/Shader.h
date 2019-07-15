#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader(std::string vertex_path, std::string fragment_path);
	void SetFloat(int uniform_location, float value);
	void SetMat4(int uniform_location, glm::mat4 value);
	void Use();
	int transform_location;
private:
	unsigned int shader_id;
};

