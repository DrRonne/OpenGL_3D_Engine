#include "Game.h"



Game::Game()
{
}

// Initialize all systems and its components
void Game::Init()
{
	deltaTime = 0.0f;
	lastTime = 0.0f;

	// Init window
	mainWindow = new Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow->Initialise();

	// Init lightingmanager
	LightingManager* lightManager = new LightingManager();
	DirectionalLight* mainLight = new DirectionalLight(4096, 4096,
												  1.0f, 1.0f, 1.0f,
												  0.1f, 0.3f,
												  0.0f, -15.0f, -10.0f);
	lightManager->SetMainLight(*mainLight);
	/*lightManager->AddPointLight(PointLight(1024, 1024,
		0.1f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.4f,
		2.0f, 2.0f, 0.0f,
		0.3f, 0.01f, 0.01f));*/

	// Init skybox
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	Skybox* skybox = new Skybox(skyboxFaces);

	// Init camera
	camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	// Create SceneRenderer
	SceneRenderer* scenerenderer = new SceneRenderer(mainWindow, lightManager, skybox, camera);
	renderer = scenerenderer;
	Systems.push_back(scenerenderer);
}

// Gameloop, eternal loop until the program closes
void Game::Loop()
{
	while (!mainWindow->getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera->keyControl(mainWindow->getKeys(), deltaTime);
		camera->mouseControl(mainWindow->getXChange(), mainWindow->getYChange());

		for (size_t i = 0; i < Systems.size(); i++)
		{
			Systems[i]->Execute();
		}
	}
}

Game::~Game()
{
	// remove systems?
}
