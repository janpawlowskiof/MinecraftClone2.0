#include "Player.h"
#include "MyCraft.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player()
{
	//config settings
	player_speed = std::stof(MyCraft::config_map["player_speed"]);
	mouse_sensitivity = std::stof(MyCraft::config_map["mouse_sensitivity"]);
	fov = std::stof(MyCraft::config_map["fov"]);
	//initial last time setting
	last_time = glfwGetTime();
	//initial projecition matrix calculation
	projection = glm::perspective(glm::radians(fov), (float)MyCraft::width / MyCraft::height, 0.1f, 1000.0f);
}

void Player::UpdateMouse(float delta_yaw, float delta_pitch)
{
	//updates yaw and pitch keepeing them in range
	yaw += -delta_yaw * mouse_sensitivity;
	if (yaw > 360) yaw -= 360;
	else if (yaw < 0) yaw += 360;

	pitch += delta_pitch * mouse_sensitivity;
	if (pitch > 89) pitch = 89.0f;
	if (pitch < -89) pitch = -89.0f;
}

void Player::Update(std::map<std::pair<int, int>, Chunk*> chunk_map)
{
	//time form prev update
	double delta_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	//movement delta in every given LOCAL direction
	float delta_forward =
		((glfwGetKey(MyCraft::window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(MyCraft::window, GLFW_KEY_S) == GLFW_PRESS)) * player_speed * delta_time;
	float delta_right =
		((glfwGetKey(MyCraft::window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(MyCraft::window, GLFW_KEY_A) == GLFW_PRESS)) * player_speed * delta_time;
	float delta_up =
		((glfwGetKey(MyCraft::window, GLFW_KEY_SPACE) == GLFW_PRESS) - (glfwGetKey(MyCraft::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) * player_speed * delta_time;

	//forward but aligned to the ground
	forward_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(0, 0, -1);
	glm::vec3 right_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(1, 0, 0);
	//camera real forward
	forward = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(pitch), glm::vec3(-1, 0, 0)) * glm::vec3(0, 0, -1);
	//pos update
	position += forward_flat * delta_forward + right_flat * delta_right + glm::vec3(0, 1, 0) * delta_up;

	current_chunk_x = floor(position.x / 16.0);
	current_chunk_z = floor(position.z / 16.0);

	//calculating view matrix
	glm::mat4 view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
	MyCraft::basic_shader->SetMat4(MyCraft::basic_shader->view_location, view);
	//updating projection matrix
	MyCraft::basic_shader->SetMat4(MyCraft::basic_shader->projection_location, projection);


	if (glfwGetKey(MyCraft::window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		auto iterator = chunk_map.find(std::make_pair(current_chunk_x, current_chunk_z));
		if (iterator != chunk_map.end())
		{
			auto chunk = iterator->second;
			chunk->ReplaceBlock(floor(position.x), floor(position.y), floor(position.z), new SimpleBlock(blk_id::air_id));
			chunk->RecalculateVisibility();
		}
	}
	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
}

glm::vec3 Player::position = glm::vec3(10, 60, 10);
glm::vec3 Player::forward = glm::vec3(0, 0, 1);
glm::vec3 Player::forward_flat = glm::vec3(0, 0, 1);
int Player::current_chunk_x, Player::current_chunk_z;
float Player::pitch = 0;
float Player::yaw = 0;