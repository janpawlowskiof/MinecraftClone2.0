#include "Player.h"
#include "cubanCigar.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player()
{
	//config settings
	player_speed = std::stof(CubanCigar::config_map["player_speed"]);
	mouse_sensitivity = std::stof(CubanCigar::config_map["mouse_sensitivity"]);
	fov = std::stof(CubanCigar::config_map["fov"]);
	//initial last time setting
	last_time = glfwGetTime();
	//initial projecition matrix calculation
	projection = glm::perspective(glm::radians(fov), (float)CubanCigar::width / CubanCigar::height, 0.1f, 100.0f);
}

void Player::UpdateMouse(float delta_yaw, float delta_pitch)
{
	//updates yaw and pitch keepeing them in range
	yaw += -delta_yaw * mouse_sensitivity;
	if (yaw > 360) yaw -= 360;
	else if (yaw < 0) yaw += 360;

	pitch += delta_pitch * mouse_sensitivity;
	if (pitch > 89) pitch = 89.999f;
	if (pitch < -89) pitch = -89.999f;
}

void Player::Update()
{
	//time form prev update
	double delta_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	//movement delta in every given LOCAL direction
	float delta_forward =
		((glfwGetKey(CubanCigar::window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(CubanCigar::window, GLFW_KEY_S) == GLFW_PRESS)) * player_speed * delta_time;
	float delta_right =
		((glfwGetKey(CubanCigar::window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(CubanCigar::window, GLFW_KEY_A) == GLFW_PRESS)) * player_speed * delta_time;
	float delta_up =
		((glfwGetKey(CubanCigar::window, GLFW_KEY_SPACE) == GLFW_PRESS) - (glfwGetKey(CubanCigar::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) * player_speed * delta_time;

	//forward but aligned to the ground
	glm::vec3 forward_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(0, 0, -1);
	glm::vec3 right_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(1, 0, 0);
	//camera real forward
	glm::vec3 forward =  glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(pitch), glm::vec3(-1, 0, 0)) * glm::vec3(0, 0, -1);
	//pos update
	position += forward_flat * delta_forward + right_flat * delta_right + glm::vec3(0, 1, 0) * delta_up;

	//calculating view matrix
	glm::mat4 view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
	CubanCigar::basic_shader->SetMat4(CubanCigar::basic_shader->view_location, view);
	//updating projection matrix
	CubanCigar::basic_shader->SetMat4(CubanCigar::basic_shader->projection_location, projection);

	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
}
