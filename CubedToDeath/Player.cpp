#include "Player.h"
#include "MyCraft.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ComplexBlock.h"

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
	if (delta_time > 0.5)
		delta_time = 0.01;
	last_time = glfwGetTime();

	//finding chunk on which player stands
	auto iterator = MyCraft::chunk_map.find(std::make_pair(current_chunk_x, current_chunk_z));
	if (iterator == MyCraft::chunk_map.end())
		return;
	current_chunk = iterator->second;

	//movement delta in every given LOCAL direction

	float delta_forward = 0, delta_right = 0, delta_up = 0;
	if (!MyCraft::command_input_enabled)
	{
		delta_forward =
			((glfwGetKey(MyCraft::window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(MyCraft::window, GLFW_KEY_S) == GLFW_PRESS));
		delta_right =
			((glfwGetKey(MyCraft::window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(MyCraft::window, GLFW_KEY_A) == GLFW_PRESS));
		delta_up =
			((glfwGetKey(MyCraft::window, GLFW_KEY_SPACE) == GLFW_PRESS) - (glfwGetKey(MyCraft::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS));
		//delta_up =
		/*if (glfwGetKey(MyCraft::window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			speed.y = 10;
		}*/
	}

	//forward but aligned to the ground
	forward_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(0, 0, -1);
	glm::vec3 right_flat = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::vec3(1, 0, 0);
	//camera real forward
	forward = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(pitch), glm::vec3(-1, 0, 0)) * glm::vec3(0, 0, -1);
	//pos update
	glm::vec3 desired_speed = (forward_flat * delta_forward + right_flat * delta_right + glm::vec3(0, 1, 0) * delta_up) * player_speed;
	float clamp_value = speed_clamp * delta_time;
	speed.x = (1 - clamp_value) * speed.x + clamp_value * desired_speed.x;
	speed.z = (1 - clamp_value) * speed.z + clamp_value * desired_speed.z;
	speed.y = (1 - clamp_value) * speed.y + clamp_value * desired_speed.y;
	//speed.y -= gravity * delta_time;
	position += speed * (float)delta_time;
	//position += forward_flat * delta_forward + right_flat * delta_right + glm::vec3(0, 1, 0) * delta_up;

	current_chunk_x = floor(position.x / 16.0);
	current_chunk_z = floor(position.z / 16.0);

	//calculating view matrix
	//MyCraft::basic_shader->Use();
	view = glm::lookAt(camera_position, camera_position + forward, glm::vec3(0, 1, 0));
	//MyCraft::basic_shader->SetMat4(MyCraft::basic_shader->view_location, view);
	//MyCraft::basic_shader->SetMat4(MyCraft::basic_shader->projection_location, projection);
	//glUniform3f(MyCraft::basic_shader->view_position_location, position.x, position.y, position.z);

	/*MyCraft::fluid_shader->Use();
	MyCraft::fluid_shader->SetMat4(MyCraft::fluid_shader->view_location, view);
	//updating projection matrix
	MyCraft::fluid_shader->SetMat4(MyCraft::fluid_shader->projection_location, projection);
	glUniform3f(MyCraft::fluid_shader->view_position_location, position.x, position.y, position.z);*/

	/*std::vector<CollisionInfo> collision_infos;
	if (coliding = CheckPlayerCollision(position, collision_infos))
	{
		ResolveCollisons(collision_infos);
		//float vertical_resolution_offset = SimpleBlock::ResolveCollisionVerticaly(glm::vec3(collision_info.x + 0.5, collision_info.y + 0.5, collision_info.z + 0.5), position + glm::vec3(0, hitbox_height/2, 0), hitbox_height);
		//position.y += vertical_resolution_offset + 0.01;
		//colliding_id = collision_info.block->id;
	}*/

	camera_position = position + camera_offset;
	if (glfwGetMouseButton(MyCraft::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!lmb_down)
		{
			lmb_down = true;
			RayHitInfo hit_info;
			if (GetHitInfo(hit_info))
			{
				hit_info.hit_chunk->ReplaceBlock(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z, new SimpleBlock(blk_id::air_id), true);
				//hit_info.chunk->visibility_update_needed;
				hit_info.hit_chunk->RecalculateVbos();

				if (hit_info.hit_x % 16 == 0 && hit_info.hit_chunk->east_chunk)
					hit_info.hit_chunk->east_chunk->RecalculateVbos();

				if (hit_info.hit_x % 16 == 15 && hit_info.hit_chunk->west_chunk)
					hit_info.hit_chunk->west_chunk->RecalculateVbos();

				if (hit_info.hit_z % 16 == 0 && hit_info.hit_chunk->south_chunk)
					hit_info.hit_chunk->south_chunk->RecalculateVbos();

				if (hit_info.hit_z % 16 == 15 && hit_info.hit_chunk->north_chunk)
					hit_info.hit_chunk->north_chunk->RecalculateVbos();

				if (MyCraft::ps)
					delete MyCraft::ps;
				MyCraft::ps = new ParticleSystem(glm::vec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), SimpleBlock::GetColor(hit_info.hit_block->id));
				MyCraft::ps->Initialize();
				MyCraft::ps->Start();
			}
		}
	}
	else
	{
		lmb_down = false;
	}
	if (glfwGetMouseButton(MyCraft::window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (!rmb_down)
		{
			rmb_down = true;
			RayHitInfo hit_info;
			if (GetHitInfo(hit_info))
			{
				if (hit_info.hit_block->GetFlag(SimpleBlock::COMPLEX))
				{
					((ComplexBlock*)hit_info.hit_block)->OnPlayerClick();
				}
				else
				{
					SimpleBlock* block = SimpleBlock::CreateNew(selected_block_id, hit_info);
					hit_info.place_chunk->ReplaceBlock(hit_info.place_x, hit_info.place_y, hit_info.place_z, block, true);
					//hit_info.chunk->visibility_update_needed;
					hit_info.place_chunk->RecalculateVbos();

					if (hit_info.place_x == 0 && hit_info.place_chunk->east_chunk)
						hit_info.place_chunk->east_chunk->RecalculateVbos();

					if (hit_info.place_x == 15 && hit_info.place_chunk->west_chunk)
						hit_info.place_chunk->west_chunk->RecalculateVbos();

					if (hit_info.place_z == 0 && hit_info.place_chunk->south_chunk)
						hit_info.place_chunk->south_chunk->RecalculateVbos();

					if (hit_info.place_z == 15 && hit_info.place_chunk->north_chunk)
						hit_info.place_chunk->north_chunk->RecalculateVbos();
				}
			}
		}
	}
	else
	{
		rmb_down = false;
	}
	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
}

//return whether some block was hit by the raycast, and fill HitInfo abject with hit related data
bool Player::GetHitInfo(RayHitInfo& hit_info)
{
	//range in which blocks are checked
	const int check_distance = 6;
	const int min_check_x = (Player::forward.x < 0) ? -check_distance : 0;
	const int max_check_x = (Player::forward.x < 0) ? 0 : check_distance;

	const int min_check_y = (Player::forward.y < 0) ? -check_distance : 0;
	const int max_check_y = (Player::forward.y < 0) ? 0 : check_distance;

	const int min_check_z = (Player::forward.z < 0) ? -check_distance : 0;
	const int max_check_z = (Player::forward.z < 0) ? 0 : check_distance;

	//httinfo object with data related to currently checked block
	RayHitInfo hi;
	int player_local_x = std::floor(camera_position.x) - 16 * current_chunk->chunk_x;
	int player_local_z = std::floor(camera_position.z) - 16 * current_chunk->chunk_z;

	bool block_hit = false;
	//loop over all blocks in range
	for (int x = min_check_x; x <= max_check_x; x++)
		for (int z = min_check_z; z <= max_check_z; z++)
			for (int y = min_check_y; y <= max_check_y; y++)
			{
				int local_x = player_local_x + x;
				int local_z = player_local_z + z;
				int local_y = std::min(std::max((int)std::floor(camera_position.y) + y, 0), 127);

				//getting pointer to block we want to check and its chunk
				Chunk* hit_chunk = current_chunk;
				SimpleBlock* hit_block = current_chunk->GetBlockInArea(local_x, local_y, local_z, hit_chunk);
				//skipping air
				if (hit_block->id == blk_id::air_id)
					continue;

				if (!hit_block->GetFlag(SimpleBlock::COMPLEX))
				{
					//check if hit for simple blocks
					if (SimpleBlock::CheckRayCollision(camera_position, forward, std::floor(camera_position.x + x), std::floor(camera_position.y + y), std::floor(camera_position.z + z), hi))
					{
						//check if current hit is better than the best so far
						if (hi.distance < hit_info.distance)
						{
							hit_info = hi;
							hit_info.hit_block = hit_block;
							hit_info.hit_chunk = hit_chunk;
							block_hit = true;
						}
					}
				}
				else
				{
					///				complex block detection				///
					if (((ComplexBlock*)hit_block)->CheckRayCollision(camera_position, forward, std::floor(camera_position.x + x), std::floor(camera_position.y + y), std::floor(camera_position.z + z), hi))
					{
						//check if current hit is better than the best so far
						if (hi.distance < hit_info.distance)
						{
							hit_info = hi;
							hit_info.hit_block = hit_block;
							hit_info.hit_chunk = hit_chunk;
							block_hit = true;
						}
					}
				}
			}
	if (!block_hit)
		return false;

	//setting info about place block
	int local_place_x = hit_info.place_x - 16 * hit_info.hit_chunk->chunk_x;
	int local_place_z = hit_info.place_z - 16 * hit_info.hit_chunk->chunk_z;
	Chunk* place_chunk = nullptr;
	SimpleBlock* place_block = hit_info.hit_chunk->GetBlockInArea(local_place_x, hit_info.place_y, local_place_z, place_chunk);
	hit_info.place_block = place_block;
	hit_info.place_chunk = place_chunk;
	return true;
}

bool Player::CheckPlayerCollision(glm::vec3 position, std::vector<CollisionInfo>& infos)
{
	int player_local_x = std::floor(position.x) - 16 * current_chunk->chunk_x;
	int player_local_z = std::floor(position.z) - 16 * current_chunk->chunk_z;
	//coliding = SimpleBlock::CheckCylinderVsAABB(position, 0.5, 4, glm::vec3(0, 70, 0) + glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5));

	for (int x = -2; x <= 2; x++)
		for (int z = -2; z <= 2; z++)
			for (int y = 3; y >= -3; y--)
			{
				int local_x = player_local_x + x;
				int local_z = player_local_z + z;
				int local_y = std::min(std::max((int)std::floor(position.y) + y, 0), 127);

				//getting pointer to block we want to check and its chunk
				Chunk* chunk = current_chunk;
				SimpleBlock* block = current_chunk->GetBlockInArea(local_x, local_y, local_z, chunk);
				if (block->id == blk_id::air_id)
					continue;

				int global_x = local_x + chunk->chunk_x * 16, global_z = local_z + chunk->chunk_z * 16;
				glm::vec3 closest_point;
				if (SimpleBlock::CheckCylinderVsAABB(position + glm::vec3(0, hitbox_height / 2, 0), hitbox_radius, hitbox_height, glm::vec3(global_x, local_y, global_z) + glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), closest_point))
				{
					if(closest_point.y)

					infos.push_back(CollisionInfo(global_x, local_y, global_z, block, closest_point));
				}
				//hit_block
			}
	if (infos.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::CheckPlayerCollision(glm::vec3 position)
{
	std::vector<CollisionInfo> infos;
	return CheckPlayerCollision(position, infos);
}

bool compareCollisionInfoHeights(CollisionInfo a, CollisionInfo b)
{
	return a.y > b.y;
}

void Player::ResolveCollisons(std::vector<CollisionInfo>& infos)
{
	//std::cout << "Sorting\n";
	std::sort(infos.begin(), infos.end(), compareCollisionInfoHeights);
	for (auto info : infos)
	{
		if (info.block->GetFlag(SimpleBlock::COMPLEX))
		{
			///TODO:
		}
		else
		{
			float vertical_resolution = SimpleBlock::ResolveCollisionVerticaly(glm::vec3(info.x + 0.5, info.y + 0.5, info.z + 0.5), position + glm::vec3(0, hitbox_height / 2, 0), hitbox_height);

			if (vertical_resolution <= 0.6 && CheckPlayerCollision(position + glm::vec3(0, vertical_resolution, 0)))
			{
				//std::cout << "Skipping" << glfwGetTime() << "\n";
				position.y += vertical_resolution;
				speed.y = 0;
			}
			else
			{
				//std::cout << "Not Skipping" << glfwGetTime() << "\n";
				position += SimpleBlock::ResolveCollision(glm::vec3(info.x + 0.5, info.y + 0.5, info.z + 0.5), position + glm::vec3(0, hitbox_height / 2, 0), hitbox_radius, hitbox_height);
			}
		}
		//std::cout << "y: " << info.y << "\n";
	}
}

glm::mat4 Player::projection;
glm::mat4 Player::view;
glm::vec3 Player::position = glm::vec3(10, 60, 10);
glm::vec3 Player::camera_position = glm::vec3(10, 62, 10);
glm::vec3 Player::camera_offset = glm::vec3(0, 1.75, 0);
glm::vec3 Player::forward = glm::vec3(0, 0, 1);
glm::vec3 Player::forward_flat = glm::vec3(0, 0, 1);
int Player::current_chunk_x, Player::current_chunk_z;
float Player::pitch = 0;
float Player::yaw = 0;
int Player::selected_block_id = blk_id::door_id;
bool Player::coliding = false;
Chunk* Player::current_chunk = nullptr;
int Player::colliding_id = -1;
const float Player::hitbox_radius = 0.5;
const float Player::hitbox_height = 1.9;
float Player::speed_clamp = 10;
float Player::gravity = 25;
glm::vec3 Player::speed;