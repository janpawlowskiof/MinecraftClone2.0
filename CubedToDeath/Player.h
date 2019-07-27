#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include "Chunk.h"

class Player
{
private:
	//player in world pos
	float fov;
	float player_speed;
	float mouse_sensitivity;
	//time of previous update
	double last_time;
	glm::mat4 projection;
public:
	//camera pitch
	static float pitch;
	//camera yaw
	static float yaw;
	static glm::vec3 position;
	static glm::vec3 forward;
	static int current_chunk_x, current_chunk_z;
	Player();
	//Updates player position
	void Update(std::map<std::pair<int, int>, Chunk*> chunk_map);
	//Updates camera rotation
	void UpdateMouse(float delta_yaw, float delta_pitch);
};

