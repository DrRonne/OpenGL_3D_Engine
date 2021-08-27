#pragma once

#include <vector>

#include "Skybox.h"
#include "Shader.h"

#include "LightingManager.h"
#include "SystemBase.h"
#include "SceneRenderer.h"

class Game
{
public:
	Game();

	void Init();

	void Loop();

	SceneRenderer* GetSceneRenderer() { return renderer; }

	~Game();

private:
	Window* mainWindow;
	Camera* camera; // This should not be here, just here for testing right now

	GLfloat deltaTime;
	GLfloat lastTime;

	SceneRenderer* renderer;

	std::vector<SystemBase*> Systems;
};

