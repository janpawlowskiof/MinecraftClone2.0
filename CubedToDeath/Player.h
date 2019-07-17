#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Player
{
private:
	//player in world pos
	glm::vec3 position = glm::vec3(0, 0, 2);
	//camera yaw
	float yaw = 0;
	//camera pitch
	float pitch = 0;

	float fov;
	float player_speed;
	float mouse_sensitivity;
	//time of previous update
	float last_time;

	glm::mat4 projection;
public:
	Player();
	//Updates player position
	void Update();
	//Updates camera rotation
	void UpdateMouse(float delta_yaw, float delta_pitch);
};

