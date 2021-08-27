#define STB_IMAGE_IMPLEMENTATION

#include "Material.h"
#include "Model.h"
#include "Game.h"
#include "RenderComponent.h"

int main()
{
	Game game = Game();
	game.Init();

	Model car1 = Model();
	car1.LoadModel("Models/othercar.obj");
	glm::mat4 model(1.0f);
	Material shinyMaterial = Material(4.0f, 256);
	RenderComponent *carRender = new RenderComponent(car1, shinyMaterial, model);
	game.GetSceneRenderer()->RegisterRenderComponent(carRender);

	game.Loop();

	return 0;
}