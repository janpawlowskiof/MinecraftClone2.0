#include "cubanCigar.h"

CubanCigar::CubanCigar()
{
	LoadConfig("config.txt");
}

CubanCigar::~CubanCigar()
{
	glfwTerminate();
}

//inicjalizuje i odpala grê
void CubanCigar::Run()
{
	//standardowa inicjalizacja
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//domyœlne wymiary
	height = std::stoi(config_map["height"]);
	width = std::stoi(config_map["width"]);

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

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glVertex2f(0.1f, 0.1f);
		glVertex2f(0.9f, 0.1f);
		glVertex2f(0.5f, 0.9f);
		glfwPollEvents();
	}
}

//inicjalizacja!!!!
std::map<std::string, std::string> CubanCigar::config_map;
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
