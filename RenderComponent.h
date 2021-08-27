#pragma once

#include <glm\glm.hpp>

#include "Component.h"
#include "Model.h"
#include "Material.h"

class RenderComponent :
	public Component
{
public:
	RenderComponent();
	RenderComponent(Model m, Material mat, glm::mat4 mm);

	Model GetModel() { return model; }
	Material GetMaterial() { return material; }

	glm::mat4 GetModelMatrix() { return model_matrix; }
	void SetModelMatrix(glm::mat4 mm);

	~RenderComponent();

private:
	Model model;
	Material material;
	glm::mat4 model_matrix;
};

