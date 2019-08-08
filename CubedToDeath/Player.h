#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include "Chunk.h"
#include "HitInfo.h"

class Player
{
private:
	//player in world pos
	float fov;
	float player_speed;
	float mouse_sensitivity;
	//time of previous update
	double last_time;
	bool lmb_down = false;
	bool rmb_down = false;
public:
	//camera pitch
	static float pitch;
	//camera yaw
	static float yaw;
	static glm::vec3 position;
	static glm::vec3 forward;
	static glm::vec3 forward_flat;
	static glm::mat4 projection;
	static glm::mat4 view;
	//return whether some block was hit by the raycast, and fill HitInfo abject with hit related data
	static bool GetHitInfo(HitInfo& hit_info);
	static int current_chunk_x, current_chunk_z;
	static int selected_block_id;
	Player();
	//Updates player position
	void Update(std::map<std::pair<int, int>, Chunk*> chunk_map);
	//Updates camera rotation
	void UpdateMouse(float delta_yaw, float delta_pitch);
};

