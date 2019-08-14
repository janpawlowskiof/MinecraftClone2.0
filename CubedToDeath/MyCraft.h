#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include "Shader.h"
#include "Chunk.h"
#include "Player.h"
#include "Texture.h"
#include "TextureArray.h"
#include "ChunkManager.h"
#include <mutex>
#include "Text.h"
#include "Sprite.h"
#include "Command.h"
#include "ParticleSystem.h"

class MyCraft
{
public:
	MyCraft();
	~MyCraft();
	//Starts minecraft
	void Run();
	//Adds vbo and vao to delete queue
	static void QueueBuffersToDelete(unsigned int vbo, unsigned int vao);
	//mouse callback
	static void mouse_callback(GLFWwindow* window, double x, double y);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//podstawowy shader
	static Shader* basic_shader;
	//text shader
	static Shader* text_shader;
	static Shader* sprite_shader;
	static Shader* fluid_shader;
	static Shader* depth_shader;
	static Shader* post_shader;
	static Shader* particle_shader;
	static Shader* particle_depth_shader;
	//mapa konfiguracyjna
	static std::map<std::string, std::string> config_map;
	//okienko
	static GLFWwindow* window;
	//player
	static Player* player;
	//display dimencions
	static int height, width;
	static int render_distance;
	static Text* text;
	static chunk_hash_map chunk_map;
	static bool command_input_enabled;
	static double current_time;
	static double last_time;
	static unsigned int fbo_shadow_map;
	static unsigned int shadow_map_close;
	static unsigned int shadow_map_far_used;
	static unsigned int shadow_map_far_unused;

	static ParticleSystem* ps;
private:
	//initializes opengl
	void InitializeOpenGL();
	//loads config file
	void LoadConfig(std::string path);
	//function of world manager thread
	static void WorldManagerFunction();
	static void ChunkUnloaderFunction();

	static void DeleteBuffers();
	static void Update();
	static void RenderScene();
	static void RenderShadowMaps();
	static void RenderParticles();
	//has mouse been updated previously
	static bool first_mouse;
	//last mouse coordinates
	static double last_x, last_y;
	//main texture
	//static Texture* texture_terrain;
	static TextureArray* texture_terrain_array;
	//chunk manager
	static ChunkManager chunk_manager;
	//thread responsible for managing world
	std::thread world_manager;
	std::thread chunk_unloader;
	//crosshair sprite
	static Sprite* crosshair;
	static Command command;
	static std::string command_input;
	static std::mutex buffers_queue_mutex;
	static std::vector<unsigned int> vbos_delete_queue;
	static std::vector<unsigned int> vaos_delete_queue;
	static glm::mat4 light_space_close_matrix;
	static glm::mat4 light_space_far_used_matrix;
	static glm::mat4 light_space_far_unused_matrix;
	static glm::vec3 light_direction;
	static glm::vec3 light_color;

	static unsigned int quadVAO;
	static unsigned int quadVBO;
};