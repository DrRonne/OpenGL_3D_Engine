#include "RenderComponent.h"



RenderComponent::RenderComponent()
{
}

RenderComponent::RenderComponent(Model m, Material mat, glm::mat4 mm)
{
	model = m;
	material = mat;
	model_matrix = mm;
}


void RenderComponent::SetModelMatrix(glm::mat4 mm)
{
	model_matrix = mm;
}

RenderComponent::~RenderComponent()
{
}
