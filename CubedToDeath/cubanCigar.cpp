#include "cubanCigar.h"

CubanCigar::CubanCigar()
{

}

void CubanCigar::InitializeOpenGL()
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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

//inicjalizuje i odpala grê
void CubanCigar::Run()
{
	//Loading config file into a map
	LoadConfig("config.txt");
	height = std::stoi(config_map["height"]);
	width = std::stoi(config_map["width"]);

	//Initializing opengl stuff
	InitializeOpenGL();

	texture_terrain = new Texture(config_map["texture_terrain_path"]);

	basic_shader = new Shader("res/vertex.txt", "res/fragment.txt");

	player = new Player();

	///	test  ///
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	Chunk chunk(0, 0);
	float counter = 0;
	///		///

	//main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player->Update();
		basic_shader->Use();
		texture_terrain->Bind();
		chunk.Draw();

		glfwSwapBuffers(window);
	}
}

//wczytuje plik konfiguracyjny do mapy
void CubanCigar::LoadConfig(std::string path)
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

void CubanCigar::mouse_callback(GLFWwindow* window, double x, double y)
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

	player->UpdateMouse(xoffset, yoffset);
}

CubanCigar::~CubanCigar()
{
	glfwTerminate();
	delete basic_shader;
	delete player;
	delete window;
	delete texture_terrain;
}

//inicjalizacja statycznych zmiennych
std::map<std::string, std::string> CubanCigar::config_map;
Shader* CubanCigar::basic_shader = nullptr;
GLFWwindow* CubanCigar::window = nullptr;
Player* CubanCigar::player = nullptr;
bool CubanCigar::first_mouse = true;
double CubanCigar::last_x = 0; 
double CubanCigar::last_y = 0;
int CubanCigar::width = 800;
int CubanCigar::height = 600;