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
#include "ChunkManager.h"
#include <mutex>
#include "Text.h"
#include "Sprite.h"

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
	//podstawowy shader
	static Shader* basic_shader;
	//text shader
	static Shader* text_shader;
	static Shader* sprite_shader;
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
private:
	//initializes opengl
	void InitializeOpenGL();
	//loads config file
	void LoadConfig(std::string path);
	//function of world manager thread
	static void WorldManagerFunction();
	static void ChunkUnloaderFunction();
	//draws world
	static void Update();
	//has mouse been updated previously
	static bool first_mouse;
	//last mouse coordinates
	static double last_x, last_y;
	//main texture
	Texture* texture_terrain;
	//chunk manager
	static ChunkManager chunk_manager;
	//thread responsible for managing world
	std::thread world_manager;
	std::thread chunk_unloader;

	Sprite* crosshair;

	static std::mutex buffers_queue_mutex;
	static std::vector<unsigned int> vbos_delete_queue;
	static std::vector<unsigned int> vaos_delete_queue;
};