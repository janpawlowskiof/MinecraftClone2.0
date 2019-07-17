#include "Player.h"
#include "cubanCigar.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player()
{
	player_speed = std::stof(CubanCigar::config_map["player_speed"]);
	mouse_sensitivity = std::stof(CubanCigar::config_map["mouse_sensitivity"]);
	last_time = glfwGetTime();
}

void Player::UpdateMouse(float delta_yaw, float delta_pitch)
{
	yaw += -delta_yaw * mouse_sensitivity;
	if (yaw > 360)
	{
		yaw -= 360;
	}
	else if (yaw < 0)
	{
		yaw += 360;
	}

	pitch += delta_pitch * mouse_sensitivity;
	if (pitch > 89) pitch = 89.999f;
	if (pitch < -89) pitch = -89.999f;
}

void Player::Update()
{
	float delta_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	float delta_forward =
		((glfwGetKey(CubanCigar::window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(CubanCigar::window, GLFW_KEY_S) == GLFW_PRESS)) * player_speed * delta_time;
	float delta_right =
		((glfwGetKey(CubanCigar::window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(CubanCigar::window, GLFW_KEY_A) == GLFW_PRESS)) * player_speed * delta_time;
	float delta_up =
		((glfwGetKey(CubanCigar::window, GLFW_KEY_SPACE) == GLFW_PRESS) - (glfwGetKey(CubanCigar::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) * player_speed * delta_time;

	/*forward.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	forward.y = sin(glm::radians(pitch));
	forward.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	forward = glm::normalize(forward);*/
	glm::vec3 forward_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(0, 0, -1);
	glm::vec3 right_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(1, 0, 0);
	glm::vec3 forward =  glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(pitch), glm::vec3(-1, 0, 0)) * glm::vec3(0, 0, -1);
	
	position += forward_flat * delta_forward + right_flat * delta_right + glm::vec3(0, 1, 0) * delta_up;

	glm::mat4 view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
	CubanCigar::basic_shader->SetMat4(CubanCigar::basic_shader->view_location, view);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	CubanCigar::basic_shader->SetMat4(CubanCigar::basic_shader->projection_location, projection);

	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
}
