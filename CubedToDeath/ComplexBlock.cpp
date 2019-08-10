#include "ComplexBlock.h"
#include <glm\ext\matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

ComplexBlock::~ComplexBlock()
{
}

float* blk::Torch::CreateModel(float* target, int world_x, int world_y, int world_z)
{
	const float width = 1.0f / 16.0f;

	glm::vec3 p000 = glm::vec3(0.5 - width, 0, 0.5 - width);
	glm::vec3 p001 = glm::vec3(0.5 - width, 0, 0.5 + width);
	glm::vec3 p010 = glm::vec3(0.5 - width, 1, 0.5 - width);
	glm::vec3 p011 = glm::vec3(0.5 - width, 1, 0.5 + width);
	glm::vec3 p100 = glm::vec3(0.5 + width, 0, 0.5 - width);
	glm::vec3 p101 = glm::vec3(0.5 + width, 0, 0.5 + width);
	glm::vec3 p110 = glm::vec3(0.5 + width, 1, 0.5 - width);
	glm::vec3 p111 = glm::vec3(0.5 + width, 1, 0.5 + width);

	glm::vec3 n001 = glm::vec3(0, 0, 1);
	glm::vec3 n010 = glm::vec3(0, 1, 0);
	glm::vec3 n100 = glm::vec3(1, 0, 0);

	glm::mat4 translate = glm::mat4(1);
	glm::mat4 rotate = glm::mat4(1);
	switch (direction)
	{
	case BOTTOM:
		//transformation = glm::rot
		break;
	case NORTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		break;
	case SOUTH:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case WEST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, -1));
		break;
	case EAST:
		rotate = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0.0f, 1));
		break;
	}
	n001 = rotate * glm::vec4(n001, 1);
	n010 = rotate * glm::vec4(n010, 1);
	n100 = rotate * glm::vec4(n100, 1);

	translate = glm::translate(translate, glm::vec3(world_x, world_y, world_z));
	rotate = glm::translate(glm::vec3(0.5f, 0.5f, 0.5f)) * rotate * glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));

	p000 = translate * rotate * glm::vec4(p000, 1);
	p001 = translate * rotate * glm::vec4(p001, 1);
	p010 = translate * rotate * glm::vec4(p010, 1);
	p011 = translate * rotate * glm::vec4(p011, 1);
	p100 = translate * rotate * glm::vec4(p100, 1);
	p101 = translate * rotate * glm::vec4(p101, 1);
	p110 = translate * rotate * glm::vec4(p110, 1);
	p111 = translate * rotate * glm::vec4(p111, 1);

	const float tex_unit = 1.0f / 16.0f;
	const float left_bound = 0.5f - tex_unit;
	const float right_bound = 0.5f + tex_unit;

	const float vertices[] = {
		xyz(p001), left_bound , (1) , xyz(n001), tex_id::torch_side,
		xyz(p101), right_bound , (1) , xyz(n001),tex_id::torch_side,
		xyz(p111),right_bound , 0 , xyz(n001),tex_id::torch_side,

		xyz(p001), left_bound, (1) , xyz(n001),tex_id::torch_side,
		xyz(p111), right_bound , 0 , xyz(n001),tex_id::torch_side,
		xyz(p011), left_bound, 0 , xyz(n001),tex_id::torch_side,

		xyz(p000), left_bound, (1) , xyz(-n001),tex_id::torch_side,
		xyz(p110), right_bound , 0 , xyz(-n001),tex_id::torch_side,
		xyz(p100), right_bound , (1) , xyz(-n001),tex_id::torch_side,

		xyz(p000), left_bound, (1) , xyz(-n001),tex_id::torch_side,
		xyz(p010), left_bound, 0 , xyz(-n001),tex_id::torch_side,
		xyz(p110) ,right_bound, 0 , xyz(-n001),tex_id::torch_side,

		xyz(p100), left_bound, (1) , xyz(n100),tex_id::torch_side,
		xyz(p111), right_bound , 0 ,xyz(n100),tex_id::torch_side,
		xyz(p101), right_bound , (1) , xyz(n100),tex_id::torch_side,

		xyz(p100), left_bound, (1) , xyz(n100),tex_id::torch_side,
		xyz(p110), left_bound, 0 ,	xyz(n100),tex_id::torch_side,
		xyz(p111), right_bound , 0 ,xyz(n100),tex_id::torch_side,

		xyz(p011), right_bound , 0 , xyz(-n100),tex_id::torch_side,
		xyz(p000), left_bound, 1 ,xyz(-n100),tex_id::torch_side,
		xyz(p001), right_bound , 1 , xyz(-n100),tex_id::torch_side,

		xyz(p010), right_bound , 0 , xyz(-n100),tex_id::torch_side,
		xyz(p000), left_bound, (1) , xyz(-n100),tex_id::torch_side,
		xyz(p011), left_bound, 0 , xyz(-n100),tex_id::torch_side,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
