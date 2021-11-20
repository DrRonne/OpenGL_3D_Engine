#include "SceneRenderer.h"


SceneRenderer::SceneRenderer()
{
	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformEyePosition = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformDirectionalLightTransform = 0;
	uniformOmniLightPos = 0;
	uniformFarPlane = 0;
}

SceneRenderer::SceneRenderer(Window* window, LightingManager* lightmanager, Skybox* sky, Camera* cam)
{
	uniformProjection = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformEyePosition = 0;
	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformDirectionalLightTransform = 0;
	uniformOmniLightPos = 0;
	uniformFarPlane = 0;

	lightingManager = lightmanager;

	// Init shaders
	mainShader = new Shader();
	mainShader->CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
	directionalShadowShader = new Shader();
	directionalShadowShader->CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader = new Shader();
	omniShadowShader->CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");

	skybox = sky;

	camera = cam;

	mainWindow = window;
}

void SceneRenderer::RemoveRenderComponent(RenderComponent* component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == component)
		{
			components.erase(components.begin() + i);
			return;
		}
	}
}

void SceneRenderer::RegisterRenderComponent(RenderComponent* component)
{
	components.push_back(component);
}

void SceneRenderer::Execute()
{
	DirectionalLight mainLight = lightingManager->GetMainLight();
	PointLight** pointlights = lightingManager->GetPointLights();
	SpotLight** spotlights = lightingManager->GetSpotLights();

	DirectionalShadowMapPass(&mainLight);
	for (size_t i = 0; i < lightingManager->GetPointLightCount(); i++)
	{
		OmniShadowMapPass(pointlights[i]);
	}
	for (size_t i = 0; i < lightingManager->GetSpotLightCount(); i++)
	{
		OmniShadowMapPass(spotlights[i]);
	}

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow->getBufferWidth() / mainWindow->getBufferHeight(), 0.1f, 1000.0f);

	RenderPass(projection, camera->calculateViewMatrix());

	glUseProgram(0);

	mainWindow->swapBuffers();
}

void SceneRenderer::RenderScene()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		RenderComponent* component = components[i];
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(component->GetModelMatrix()));
		component->GetMaterial().UseMaterial(uniformSpecularIntensity, uniformShininess);
		component->GetModel().RenderModel();
	}
}

void SceneRenderer::DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader->UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader->GetModelLocation();
	directionalShadowShader->SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader->Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::OmniShadowMapPass(PointLight* light)
{
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	omniShadowShader->UseShader();
	uniformModel = omniShadowShader->GetModelLocation();
	uniformOmniLightPos = omniShadowShader->GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader->GetFarPlaneLocation();

	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader->SetOmniLightMatrices(light->CalculateLightTransform());

	omniShadowShader->Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneRenderer::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glViewport(0, 0, 1366, 768);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->DrawSkybox(viewMatrix, projectionMatrix);

	mainShader->UseShader();

	uniformModel = mainShader->GetModelLocation();
	uniformProjection = mainShader->GetProjectionLocation();
	uniformView = mainShader->GetViewLocation();
	uniformEyePosition = mainShader->GetEyePositionLocation();
	uniformSpecularIntensity = mainShader->GetSpecularIntensityLocation();
	uniformShininess = mainShader->GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	DirectionalLight mainLight = lightingManager->GetMainLight();

	mainShader->SetDirectionalLight(&mainLight);
	mainShader->SetPointLights(*lightingManager->GetPointLights(), lightingManager->GetPointLightCount(), 3, 0);
	mainShader->SetSpotLights(*lightingManager->GetSpotLights(), lightingManager->GetSpotLightCount(), 3 + lightingManager->GetPointLightCount(), lightingManager->GetPointLightCount());
	mainShader->SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);


	mainShader->SetTexture(1);
	mainShader->SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera->getCameraPosition();
	lowerLight.y -= 0.3f;
	//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	mainShader->Validate();
	RenderScene();
}

SceneRenderer::~SceneRenderer()
{
}
