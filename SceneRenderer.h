#pragma once

#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"
#include "Window.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Shader.h"

#include "LightingManager.h"
#include "SystemBase.h"
#include "RenderComponent.h"

const float toRadians = 3.14159265f / 180.0f;

class SceneRenderer :
	public SystemBase
{
public:
	SceneRenderer();

	SceneRenderer(Window* window, LightingManager* lightmanager, Skybox* sky, Camera* cam);

	void RegisterRenderComponent(RenderComponent* component);
	void RemoveRenderComponent(RenderComponent* component);

	Window* GetMainWindow() { return mainWindow; }
	Camera* GetCamera() { return camera; }

	void Execute();

	~SceneRenderer();

private:
	GLuint uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformDirectionalLightTransform, uniformOmniLightPos, uniformFarPlane;

	Camera* camera;

	LightingManager* lightingManager;

	Shader* mainShader;
	Shader* directionalShadowShader;
	Shader* omniShadowShader;

	Skybox* skybox;

	Window* mainWindow;


	std::vector<RenderComponent*> components;


	void RenderScene();
	void DirectionalShadowMapPass(DirectionalLight* light);
	void OmniShadowMapPass(PointLight* light);
	void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
};

