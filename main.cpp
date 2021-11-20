#define STB_IMAGE_IMPLEMENTATION

#include "Material.h"
#include "Model.h"
#include "Game.h"
#include "RenderComponent.h"
#include "Terrain.h"
#include "Texture.h"
#include "Constants.h"
#include "PointLight.h"

int main()
{
	Game game = Game();
	game.Init();

	Model car1 = Model();
	car1.LoadModel("Models/othercar.obj");
	Model terrainmodel = Model();
	Texture* terraintexture = new Texture("Textures/plain.png");
	terraintexture->LoadTextureA();

	glm::mat4 model(1.0f);
	Material shinyMaterial = Material(4.0f, 256);
	Material dullMaterial = Material(0.3f, 4);
	RenderComponent *carRender = new RenderComponent(car1, shinyMaterial, model);
	//game.GetSceneRenderer()->RegisterRenderComponent(carRender);
	Terrain terrain = Terrain();
	for (int x = -2; x < 2; x++) {
		for (int z = -2; z < 2; z++) {
			terrainmodel.addMesh(terrain.getChunkMesh(x * CHUNK_SIZE, z * CHUNK_SIZE));
			terrainmodel.addTexture(terraintexture);
			RenderComponent *terrainRender = new RenderComponent(terrainmodel, dullMaterial, model);
			game.GetSceneRenderer()->RegisterRenderComponent(terrainRender);
		}
	}

	game.Loop();

	return 0;
}