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
	Shader(std::string vertex_path, std::string geometry_path ,std::string fragment_path);
	//sets float uniform
	static void SetFloat(const int uniform_location, const float value);
	//sets mat4 uniform
	static void SetMat4(const int uniform_location, const glm::mat4 value);
	//uses this shader
	void Use();
	//uniforms location
	int model_location;
	int view_location;
	int projection_location;
	int color_location;
	int view_position_location;
	int time_location;
	int light_space_close_location;
	int light_space_far_location;
	//int texture_terrain;
	//int texture_depth;
	int light_direction_location;
	int light_color_location;
	int transform_matrix_location;
private:
	unsigned int shader_id;
	void InitializeUniforms();
};

