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

	float player_speed;
	float mouse_sensitivity;
	float last_time;
public:
	Player();
	void Update();
	void UpdateMouse(float delta_yaw, float delta_pitch);
};

