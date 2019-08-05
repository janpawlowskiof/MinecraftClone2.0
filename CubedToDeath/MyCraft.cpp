#include "MyCraft.h"

MyCraft::MyCraft()
{
}

bool program_should_close = false;

void MyCraft::InitializeOpenGL()
{
	//standardowa inicjalizacja
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
	glViewport(0, 0, width, height);

	//input things 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetKeyCallback(window, key_callback);

	//setting opengl flags
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

bool LineInView(glm::vec2 p1, glm::vec2 p2)
{
	if ((p1.x >= -1 && p1.x <= 1 && p1.y >= -1 && p1.y <= 1) ||
		(p2.x >= -1 && p2.x <= 1 && p2.y >= -1 && p2.y <= 1))
		return true;

	return false;
}

//inicjalizuje i odpala grê
void MyCraft::Run()
{
	//Loading config file into a map
	LoadConfig("config.txt");
	height = std::stoi(config_map["height"]);
	width = std::stoi(config_map["width"]);
	render_distance = std::stoi(config_map["render_distance"]);

	//Initializing opengl stuff
	InitializeOpenGL();

	std::cout << glGetString(GL_RENDERER) << std::endl;
	//std::cout << glGetString(GL_VENDOR) << std::endl;
	command.Initialize();

	texture_terrain = new Texture(config_map["texture_terrain_path"]);
	basic_shader = new Shader("res/basic.vert", "res/basic.frag");
	text_shader = new Shader("res/text.vert", "res/text.frag");
	sprite_shader = new Shader("res/background.vert", "res/background.frag");
	player = new Player();
	text = new Text("fonts/clacon.ttf");
	crosshair = new Sprite("res/crosshair.png");
	world_manager = std::thread(WorldManagerFunction);
	chunk_unloader = std::thread(ChunkUnloaderFunction);

	///	test  ///
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	double last_time = glfwGetTime();
	double current_time;
	vbos_delete_queue.reserve(3000);
	vaos_delete_queue.reserve(3000);
	//main loop
	while (!(program_should_close = glfwWindowShouldClose(window)))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		chunk_map = ChunkManager::GetChunkMap();
		basic_shader->Use();
		texture_terrain->Bind();
		player->Update(chunk_map);
		Update();

		//zezwolenie na usuniêcie nieu¿ywanych bloków i chunków
		ChunkManager::GiveThreadPermissionToUnloadBlocks(ChunkManager::MAIN);
		ChunkManager::GiveThreadPermissionToUnloadChunks(ChunkManager::MAIN);

		double current_time = glfwGetTime();
		glClear(GL_DEPTH_BUFFER_BIT);
		crosshair->Draw(width / 2 - 40, height / 2 - 40, 80, 80);
		text->RenderText(text_shader, "Postion: " + std::to_string(Player::position.x) + ", " + std::to_string(Player::position.y) + ", " + std::to_string(Player::position.z), 25.0f, 25.0f, 0.5f, glm::vec3(0.9, 0.9, 0.9));
		text->RenderText(text_shader, "Fps: " + std::to_string((int)(1.0 / (current_time - last_time))), 25.0f, height - 50.0f, 0.5f, glm::vec3(0.9, 0.9, 0.9));

		if (command_input_enabled)
			text->RenderText(text_shader, "Input: " + command_input, 25.0f, 50.0f, 0.5f, glm::vec3(0.9, 0.9, 0.9));

		last_time = current_time;
		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			std::cout << "Break time!" << std::endl;
		}
	}

	//³¹czenie w¹tków na koniec programu
	world_manager.join();
	chunk_unloader.join();
}

void MyCraft::QueueBuffersToDelete(unsigned int vbo, unsigned int vao)
{
	std::lock_guard<std::mutex> lock(buffers_queue_mutex);
	vbos_delete_queue.push_back(vbo);
	vaos_delete_queue.push_back(vao);
}

//wczytuje plik konfiguracyjny do mapy
void MyCraft::LoadConfig(std::string path)
{
	std::ifstream file(path);

	if (file.is_open())
	{
		std::string line;
		while (getline(file, line)) {
			//usuwanie spacji
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if (line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			std::cout << name << " " << value << '\n';
			config_map.emplace(name, value);
		}
	}
	else
	{
		std::cerr << "Couldn't open config file\n";
	}
}

void MyCraft::WorldManagerFunction()
{
	while (!program_should_close)
		chunk_manager.Update();
}

void MyCraft::ChunkUnloaderFunction()
{
	while (!program_should_close)
		ChunkManager::UnloadChunks();
}

void MyCraft::Update()
{
	{
		//blokada na bufory (vbo i vao)
		std::lock_guard<std::mutex> lock(buffers_queue_mutex);
		const int size = vbos_delete_queue.size();

		//usuwanie wszystkich niepotrzebnych buforów naraz
		glDeleteVertexArrays(size, vaos_delete_queue.data());
		glDeleteBuffers(size, vbos_delete_queue.data());

		//czyszczenie list
		vaos_delete_queue.clear();
		vbos_delete_queue.clear();
	}


	//iterates over every loaded chunk
	auto iterator = chunk_map.begin();
	while (iterator != chunk_map.end())
	{
		auto chunk = iterator->second;

		/*float angleA = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16, 0, chunk->chunk_z * 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleB = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16 + 16, 0, chunk->chunk_z * 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleC = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16 + 16, 0, chunk->chunk_z * 16 + 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleD = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16, 0, chunk->chunk_z * 16 + 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleE = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16, 127, chunk->chunk_z * 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleF = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16 + 16, 127, chunk->chunk_z * 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleG = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16 + 16, 127, chunk->chunk_z * 16 + 16) - Player::position + Player::forward * 16.0f), Player::forward));
		float angleH = glm::acos(glm::dot(
			glm::normalize(glm::vec3(chunk->chunk_x * 16, 127, chunk->chunk_z * 16 + 16) - Player::position + Player::forward * 16.0f), Player::forward));

		float max_angle = glm::radians(55.0f);
		if (!(angleA <= max_angle || angleB <= max_angle || angleC <= max_angle || angleD <= max_angle ||
			angleE <= max_angle || angleF <= max_angle || angleG <= max_angle || angleH <= max_angle))
		{
			iterator++;
			continue;
		}*/


		//initializing chunks that need to be initialized
		if (!chunk->buffers_initialized)
			chunk->InitializeBuffers();
		//if (chunk->visibility_update_needed)
		//	chunk->RecalculateVisibility();
		//updates vbos on chunks that reqire doing so
		if (chunk->buffers_update_needed)
			chunk->UpdateVbos();
		//draws th echunk
		chunk->Draw();

		iterator++;
	}
}

void MyCraft::mouse_callback(GLFWwindow* window, double x, double y)
{
	if (first_mouse)
	{
		last_x = x;
		last_y = y;
		first_mouse = false;
	}

	float xoffset = x - last_x;
	float yoffset = y - last_y;
	last_x = x;
	last_y = y;

	//updates player camera position
	player->UpdateMouse(xoffset, yoffset);
}

void MyCraft::character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (command_input_enabled)
	{
		command_input += (char)codepoint;
	}
}

void MyCraft::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		if (command_input_enabled)
		{
			command_input_enabled = false;
		}
		else
		{
			command_input_enabled = true;
			command_input = "";
		}
	}
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		command_input.pop_back();
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		command.Execute(command_input);
	}
}

MyCraft::~MyCraft()
{
	glfwTerminate();
	delete basic_shader;
	delete text_shader;
	delete sprite_shader;
	delete player;
	delete window;
	delete texture_terrain;
	delete text;
	delete crosshair;
}

//inicjalizacja statycznych zmiennych
std::map<std::string, std::string> MyCraft::config_map;
Shader* MyCraft::basic_shader = nullptr;
Shader* MyCraft::text_shader = nullptr;
Shader* MyCraft::sprite_shader = nullptr;
GLFWwindow* MyCraft::window = nullptr;
Player* MyCraft::player = nullptr;
ChunkManager MyCraft::chunk_manager;
bool MyCraft::first_mouse = true;
double MyCraft::last_x = 0;
double MyCraft::last_y = 0;
int MyCraft::width = 800;
int MyCraft::height = 600;
int MyCraft::render_distance = 6;
chunk_hash_map MyCraft::chunk_map;
std::mutex MyCraft::buffers_queue_mutex;
std::vector<unsigned int> MyCraft::vaos_delete_queue;
std::vector<unsigned int> MyCraft::vbos_delete_queue;
Text* MyCraft::text = nullptr;
bool MyCraft::command_input_enabled = false;
std::string MyCraft::command_input;
Command MyCraft::command;