#include "Shader.h"
#include <iostream>

Shader::Shader(std::string vertex_path, std::string fragment_path)
{
	//kompilowanie shadera
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream file;
	try
	{
		file.open(vertex_path);
		std::stringstream stream;
		stream << file.rdbuf();
		vertex_code = stream.str();
		file.close();

		file.open(fragment_path);
		stream.str("");
		stream.clear();
		stream << file.rdbuf();
		fragment_code = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	const GLchar* vertex_source[1];
	vertex_source[0] = vertex_code.c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, vertex_source, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// fragment Shader
	const GLchar* fragment_source[1];
	fragment_source[0] = fragment_code.c_str();
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, fragment_source, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// shader Program
	shader_id = glCreateProgram();
	glAttachShader(shader_id, vertex);
	glAttachShader(shader_id, fragment);
	glLinkProgram(shader_id);
	// print linking errors if any
	glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	model_location = glad_glGetUniformLocation(shader_id, "model");
	view_location = glad_glGetUniformLocation(shader_id, "view");
	projection_location = glad_glGetUniformLocation(shader_id, "projection");
}


void Shader::SetFloat(const int uniform_location, const float value)
{
	glUniform1f(uniform_location, value);
}

void Shader::SetMat4(const int uniform_location, const glm::mat4 value)
{
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Use()
{
	glUseProgram(shader_id);
}
