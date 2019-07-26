#include "MyCraft.h"
#include <ft2build.h>
#include FT_FREETYPE_H  

MyCraft::MyCraft()
{
}

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

	//setting opengl flags
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
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

	texture_terrain = new Texture(config_map["texture_terrain_path"]);
	basic_shader = new Shader("res/vertex.txt", "res/fragment.txt");
	player = new Player();
	world_manager = std::thread(WorldManagerFunction);
	chunk_unloader = std::thread(ChunkUnloaderFunction);

	///	test  ///
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	double counter = glfwGetTime();
	vbos_delete_queue.reserve(2000);
	vaos_delete_queue.reserve(2000);
	//main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		chunk_map = ChunkManager::GetChunkMap();
		player->Update();
		basic_shader->Use();
		texture_terrain->Bind();
		Update();

		//zezwolenie na usuniêcie nieu¿ywanych bloków i chunków
		ChunkManager::GiveThreadPermissionToUnloadBlocks(ChunkManager::MAIN);
		ChunkManager::GiveThreadPermissionToUnloadChunks(ChunkManager::MAIN);
		glfwSwapBuffers(window);

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			std::cout << "Break time!" << std::endl;
		}
	}

	//³¹czenie w¹tków na koniec programu
	world_manager.join();
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
	while (!glfwWindowShouldClose(window))
		chunk_manager.Update();
}

void MyCraft::ChunkUnloaderFunction()
{
	while (!glfwWindowShouldClose(window))
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
		//initializing chunks that need to be initialized
		if (!chunk->buffers_initialized)
			chunk->InitializeBuffers();
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

MyCraft::~MyCraft()
{
	glfwTerminate();
	delete basic_shader;
	delete player;
	delete window;
	delete texture_terrain;
}

//inicjalizacja statycznych zmiennych
std::map<std::string, std::string> MyCraft::config_map;
Shader* MyCraft::basic_shader = nullptr;
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