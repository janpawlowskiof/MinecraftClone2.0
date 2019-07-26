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
	//sets float uniform
	void SetFloat(const int uniform_location, const float value);
	//sets mat4 uniform
	void SetMat4(const int uniform_location, const glm::mat4 value);
	//uses this shader
	void Use();
	//uniforms location
	int model_location;
	int view_location;
	int projection_location;
	int color_location;
private:
	unsigned int shader_id;
};

